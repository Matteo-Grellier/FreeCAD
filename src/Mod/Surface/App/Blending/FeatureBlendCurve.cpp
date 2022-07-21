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
#include <Geom_Curve.hxx>
#include <gp_Pnt.hxx>
#endif
#include "FeatureBlendCurve.h"
#include "Mod/Surface/App/Blending/BlendPoint.h"
#include "Mod/Surface/App/Blending/BlendCurve.h"
#include "Mod/Surface/App/Blending/BlendCurve.h"
#include <GeomAPI_PointsToBSplineSurface.hxx>

using namespace Surface;

const App::PropertyFloatConstraint::Constraints StartParameterConstraint = {0.0, 1.0, 0.01};
const App::PropertyFloatConstraint::Constraints EndParameterConstraint = {0.0, 1.0, 0.01};

PROPERTY_SOURCE(Surface::FeatureBlendCurve, Part::Spline)

FeatureBlendCurve::FeatureBlendCurve()
{
    ADD_PROPERTY_TYPE(StartEdge, (nullptr), "FirstEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(StartContinuity, (2), "FirstEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(StartParameter, (1.0f), "FirstEdge", App::Prop_None, "");
    StartParameter.setConstraints(&StartParameterConstraint);
    ADD_PROPERTY_TYPE(StartSize, (1.0f), "FirstEdge", App::Prop_None, "");

    ADD_PROPERTY_TYPE(EndEdge, (nullptr), "SecondEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(EndContinuity, (2), "SecondEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(EndParameter, (1.0f), "SecondEdge", App::Prop_None, "");
    EndParameter.setConstraints(&EndParameterConstraint);
    ADD_PROPERTY_TYPE(EndSize, (1.0f), "SecondEdge", App::Prop_None, "");
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

App::DocumentObjectExecReturn *FeatureBlendCurve::execute(void)
{
    App::DocumentObject *edgePart1 = StartEdge.getValue();
    if (!edgePart1 || !edgePart1->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId()))
        return new App::DocumentObjectExecReturn("No shape linked.");
    const auto &edge1Name = StartEdge.getSubValues();
    if (edge1Name.size() != 1)
        return new App::DocumentObjectExecReturn("Not exactly one sub-shape linked.");

    TopoDS_Shape edge1 = static_cast<Part::Feature *>(edgePart1)
                             ->Shape.getShape()
                             .getSubShape(edge1Name[0].c_str());
    if (edge1.IsNull() || edge1.ShapeType() != TopAbs_EDGE)
        return new App::DocumentObjectExecReturn("Sub-shape is not a edge.");

    App::DocumentObject *edgePart2 = EndEdge.getValue();
    if (!edgePart2 || !edgePart2->getTypeId().isDerivedFrom(Part::Feature::getClassTypeId()))
        return new App::DocumentObjectExecReturn("No shape linked.");
    const auto &edge2Name = EndEdge.getSubValues();
    if (edge2Name.size() != 1)
        return new App::DocumentObjectExecReturn("Not exactly one sub-shape linked.");

    TopoDS_Shape edge2 = static_cast<Part::Feature *>(edgePart2)
                             ->Shape.getShape()
                             .getSubShape(edge2Name[0].c_str());
    if (edge2.IsNull() || edge2.ShapeType() != TopAbs_EDGE)
        return new App::DocumentObjectExecReturn("Sub-shape is not a edge.");

    const TopoDS_Edge &e1 = TopoDS::Edge(edge1);
    BRepAdaptor_Curve adapt1(e1);
    double fp1 = adapt1.FirstParameter();
    double lp1 = adapt1.LastParameter();

    const TopoDS_Edge &e2 = TopoDS::Edge(edge2);
    BRepAdaptor_Curve adapt2(e2);
    double fp2 = adapt2.FirstParameter();
    double lp2 = adapt2.LastParameter();


    double RealPar1 = RelativeToRealParameters(StartParameter.getValue(), fp1, lp1);
    double RealPar2 = RelativeToRealParameters(EndParameter.getValue(), fp2, lp2);

    std::vector<Base::Vector3d> constraints1;
    std::vector<Base::Vector3d> constraints2;

    gp_Pnt Pt1;
    gp_Pnt Pt2;

    adapt1.D0(RealPar1, Pt1);
    Base::Vector3d bv(Pt1.X(), Pt1.Y(), Pt1.Z());
    constraints1.push_back(bv);

    for (size_t i = 1; i <= StartContinuity.getValue(); i++) {
        gp_Vec v1 = adapt1.DN(RealPar1, i);
        Base::Vector3d bbv1(v1.X(), v1.Y(), v1.Z());
        constraints1.push_back(bbv1);
    }

    adapt2.D0(RealPar2, Pt2);
    Base::Vector3d bv2(Pt2.X(), Pt2.Y(), Pt2.Z());
    constraints2.push_back(bv2);
    

    for (size_t i = 1; i <= EndContinuity.getValue(); i++) {
        gp_Vec v2 = adapt2.DN(RealPar2, i);
        Base::Vector3d bbv2(v2.X(), v2.Y(), v2.Z());
        constraints2.push_back(bbv2);
    }

    BlendPoint bp1(constraints1);
    BlendPoint bp2(constraints2);

    bp1.multiply(StartSize.getValue());
    bp2.multiply(EndSize.getValue());

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
    Part::Spline::onChanged(prop);
}