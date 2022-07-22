/***************************************************************************
 *   Copyright (c) 2022 Matteo Grellier <matteogrellier@gmail.com>         *
 *                                                                         *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include "../PreCompiled.h"
#ifndef _PreComp_
#include <Precision.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <Standard_Version.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS.hxx>
#include <Base/Tools.h>
#include <Geom_Curve.hxx>
#include <gp_Pnt.hxx>
#endif
#include "FeatureBlendCurve.h"
#include "Mod/Surface/App/Blending/BlendPoint.h"
#include "Mod/Surface/App/Blending/BlendCurve.h"
#include <GeomAPI_PointsToBSplineSurface.hxx>

using namespace Surface;

const App::PropertyFloatConstraint::Constraints StartParameterConstraint = {0.0, 1.0, 0.05};
const App::PropertyFloatConstraint::Constraints EndParameterConstraint = {0.0, 1.0, 0.05};

PROPERTY_SOURCE(Surface::FeatureBlendCurve, Part::Spline)

FeatureBlendCurve::FeatureBlendCurve() : lockOnChangeMutex(false)
{
    ADD_PROPERTY_TYPE(StartEdge, (nullptr), "FirstEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(StartContinuity, (2), "FirstEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(StartParameter, (0.0f), "FirstEdge", App::Prop_None, "");
    StartParameter.setConstraints(&StartParameterConstraint);
    ADD_PROPERTY_TYPE(StartSize, (1.0f), "FirstEdge", App::Prop_None, "");

    ADD_PROPERTY_TYPE(EndEdge, (nullptr), "SecondEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(EndContinuity, (2), "SecondEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(EndParameter, (0.0f), "SecondEdge", App::Prop_None, "");
    EndParameter.setConstraints(&EndParameterConstraint);
    ADD_PROPERTY_TYPE(EndSize, (1.0f), "SecondEdge", App::Prop_None, "");
    Handle(Geom_BezierCurve) maxDegreeCurve;
    maxDegree = maxDegreeCurve->MaxDegree();
    Standard_Integer maxPossiblyDegreeEnd = 25 - StartContinuity.getValue();
    Standard_Integer maxPossiblyDegreeStart = 25 - EndContinuity.getValue();
}

short FeatureBlendCurve::mustExecute() const
{
    if (StartEdge.isTouched())
        return 1;
    if (StartParameter.isTouched())
        return 1;
    if (StartContinuity.isTouched())
        return 1;
    if (StartSize.isTouched())
        return 1;
    if (EndEdge.isTouched())
        return 1;
    if (EndParameter.isTouched())
        return 1;
    if (EndContinuity.isTouched())
        return 1;
    if (EndSize.isTouched())
        return 1;
    return 0;
}

BlendPoint FeatureBlendCurve::GetBlendPoint(App::DocumentObject *link, double param, int continuity, double size, std::vector<std::string> edgeSV)
{
    //App::DocumentObject *edgePart = link.getValue();
    if (!link || !link->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId())) {
        Standard_Failure::Raise("Failed to get feature object");
    }
    //const auto &edgeName = edgeSV;
    if (edgeSV.size() != 1) {
        Standard_Failure::Raise("no edge name");
    }
        //return error;//error

    TopoDS_Shape edge = static_cast<Part::Feature *>(link)
                             ->Shape.getShape()
                             .getSubShape(edgeSV[0].c_str());
    if (edge.IsNull() || edge.ShapeType() != TopAbs_EDGE) {
        Standard_Failure::Raise("failed to get edge");
    }

    const TopoDS_Edge &e = TopoDS::Edge(edge);
    BRepAdaptor_Curve adapt(e);
    double fp = adapt.FirstParameter();
    double lp = adapt.LastParameter();

    double RealPar = RelativeToRealParameters(param, fp, lp);

    std::vector<Base::Vector3d> constraints;
    gp_Pnt Pt;

    adapt.D0(RealPar, Pt);
    Base::Vector3d bv(Pt.X(), Pt.Y(), Pt.Z());
    constraints.push_back(bv);

    for (size_t i = 1; i <= continuity; i++) {
        gp_Vec v1 = adapt.DN(RealPar, i);
        Base::Vector3d bbv1(v1.X(), v1.Y(), v1.Z());
        constraints.push_back(bbv1);
    }

    

    BlendPoint bp(constraints);
    bp.multiply(size);


    return bp;
}

App::DocumentObjectExecReturn *FeatureBlendCurve::execute(void)
{
    //App::PropertyLinkSub edge1 = StartEdge;

    App::DocumentObject *edgePart = StartEdge.getValue();
    App::DocumentObject *edgePart2 = EndEdge.getValue();
    std::vector<std::string> edgeSV1 = StartEdge.getSubValues();
    std::vector<std::string> edgeSV2 = EndEdge.getSubValues();

    BlendPoint bp1 = GetBlendPoint(edgePart, StartParameter.getValue(), StartContinuity.getValue(), StartSize.getValue(), edgeSV1);
    BlendPoint bp2 = GetBlendPoint(edgePart2, EndParameter.getValue(), EndContinuity.getValue(), EndSize.getValue(), edgeSV2);

    std::vector<BlendPoint> blendPointsList;

    blendPointsList.push_back(bp1);
    blendPointsList.push_back(bp2);


    BlendCurve curve(blendPointsList);


    Handle(Geom_BezierCurve) bc(curve.Interpolate());
    BRepBuilderAPI_MakeEdge mkEdge(bc);

    Shape.setValue(mkEdge.Edge());

    return StdReturn;
}



double FeatureBlendCurve::RelativeToRealParameters(double relativeValue, double fp, double lp)
{
    return fp + relativeValue * (lp - fp);
}

void FeatureBlendCurve::onChanged(const App::Property *prop)
{
    // using a mutex and lock to protect a recursive calling when setting the new values
    if (lockOnChangeMutex)
        return;
    Base::StateLocker lock(lockOnChangeMutex);

    if (prop == &StartContinuity) {
        auto changedStartProp = dynamic_cast<const App::PropertyInteger *>(prop);

        if (changedStartProp->getValue() > (maxDegree - 2 - EndContinuity.getValue())) {
            
            StartContinuity.setValue(maxDegree - 2 - EndContinuity.getValue());
        }
        else if (changedStartProp->getValue() < 0) {
            
            StartContinuity.setValue(0);
        }
        
    }
    else if (prop == &EndContinuity) {
        auto changedEndProp = dynamic_cast<const App::PropertyInteger *>(prop);

        if (changedEndProp->getValue() > (maxDegree - 2 - StartContinuity.getValue())) {
            EndContinuity.setValue(maxDegree - 2 - StartContinuity.getValue());
        }
        else if (changedEndProp->getValue() < 0) {
            EndContinuity.setValue(0);
        }
        
    }
       


    Part::Spline::onChanged(prop);
}