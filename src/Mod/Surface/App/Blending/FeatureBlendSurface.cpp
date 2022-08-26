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

#include "PreCompiled.h"
#ifndef _PreComp_
#include <BRepAdaptor_Curve.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <Base/Tools.h>
#include <Geom_Curve.hxx>
#include <Precision.hxx>
#include <Standard_Version.hxx>
#include <TopoDS.hxx>
#include <gp_Pnt.hxx>
#endif
#include "FeatureBlendSurface.h"

using namespace Surface;

// const App::PropertyFloatConstraint::Constraints StartParameterConstraint = {0.0, 1.0, 0.05};
// const App::PropertyFloatConstraint::Constraints EndParameterConstraint = {0.0, 1.0, 0.05};
const App::PropertyIntegerConstraint::Constraints StartContinuityConstraint = {0, 4, 1};
const App::PropertyIntegerConstraint::Constraints EndContinuityConstraint = {0, 4, 1};

PROPERTY_SOURCE(Surface::FeatureBlendSurface, Part::Spline)

FeatureBlendSurface::FeatureBlendSurface() : lockOnChangeMutex(false)
{
    ADD_PROPERTY_TYPE(StartEdge, (nullptr), "FirstEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(StartFace, (nullptr), "FirstFace", App::Prop_None, "");
    StartContinuity.setConstraints(&StartContinuityConstraint);
    ADD_PROPERTY_TYPE(StartContinuity, (2), "FirstContinuity", App::Prop_None, "");
    // StartParameter.setConstraints(&StartParameterConstraint);
    ADD_PROPERTY_TYPE(StartSize, (1.0f), "FirstEdge", App::Prop_None, "");

    ADD_PROPERTY_TYPE(EndEdge, (nullptr), "SecondEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(EndFace, (nullptr), "SecondFace", App::Prop_None, "");
    // EndParameter.setConstraints(&EndParameterConstraint);
    ADD_PROPERTY_TYPE(EndContinuity, (2), "SecondEdge", App::Prop_None, "");
    EndContinuity.setConstraints(&EndContinuityConstraint);
    ADD_PROPERTY_TYPE(EndSize, (1.0f), "SecondEdge", App::Prop_None, "");
    ADD_PROPERTY_TYPE(nbSamples, (1.0f), "numberSamples", App::Prop_None, "");
}

short FeatureBlendSurface::mustExecute() const
{
    if (StartEdge.isTouched())
        return 1;
    if (StartFace.isTouched())
        return 1;
    if (StartContinuity.isTouched())
        return 1;
    if (StartSize.isTouched())
        return 1;
    if (EndEdge.isTouched())
        return 1;
    if (EndFace.isTouched())
        return 1;
    if (EndContinuity.isTouched())
        return 1;
    if (EndSize.isTouched())
        return 1;
    return 0;
}

TopoDS_Shape FeatureBlendSurface::getEdge(App::PropertyLinkSub link){
    auto linked = link.getValue();

    TopoDS_Shape axEdge;
    if (link.getSubValues().size() > 0 && link.getSubValues()[0].length() > 0) {
        axEdge = Feature::getTopoShape(linked, link.getSubValues()[0].c_str(), true /*need element*/).getShape();
    }
    else {
        axEdge = Feature::getShape(linked);
    }
    return axEdge;
}

TopoDS_Shape FeatureBlendSurface::getFace(App::PropertyLinkSub link)
{
    auto linked = link.getValue();

    TopoDS_Shape axFace;
    if (link.getSubValues().size() > 0 && link.getSubValues()[0].length() > 0) {
        axFace = Feature::getTopoShape(linked, link.getSubValues()[0].c_str(), true /*need element*/).getShape();
    }
    else {
        axFace = Feature::getShape(linked);

    }
    return axFace;
    ;
}

void FeatureBlendSurface::sizeDiff(App::PropertyFloatList StartSize, App::PropertyFloatList EndSize){
    if (StartSize.getSize() < EndSize.getSize()){
        EndSize.setSize(StartSize.getSize());
    }
    else{
        StartSize.setSize(EndSize.getSize());
    }
}

App::DocumentObjectExecReturn *FeatureBlendSurface::execute(void)
{
  return StdReturn;
}
void FeatureBlendSurface::onChanged(const App::Property *prop)
{
    // using a mutex and lock to protect a recursive calling when setting the new values
    if (lockOnChangeMutex)
        return;
}
