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
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <GeomConvert.hxx>
#include <BRepAdaptor_CompCurve.hxx>
#include <GeomConvert.hxx>
#include <GeomFill_BSplineCurves.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <BRep_Tool.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <Approx_Curve3d.hxx>
#include <TopoDS.hxx>
#endif
#include "Blending/BlendSurface.h"
#include <GeomFill.hxx>


using namespace Surface;

BlendSurface::BlendSurface(TopoDS_Shape shape1, TopoDS_Shape shape2){
    
}

Handle(Geom_BSplineCurve) BlendSurface::getCurve(TopoDS_Shape shape)
{
    Handle(Geom_BSplineCurve) curve;
    if (shape.ShapeType() == TopAbs_EDGE)
    {
        TopoDS_Edge edge = TopoDS::Edge(shape);
        BRepAdaptor_Curve acurve = BRepAdaptor_Curve(edge);
        if (acurve.GetType() == GeomAbs_BSplineCurve) {
            //curve = GeomConvert().CurveToBSplineCurve(acurve.Curve());
            curve = acurve.BSpline();
            return curve;
        }
        else {
            Standard_Real First, Last;
            Handle(Geom_Curve) myCurve = BRep_Tool::Curve(edge, First, Last);
            Handle(Geom_TrimmedCurve) myTrimmed = new Geom_TrimmedCurve(myCurve, First, Last);
            Handle(Geom_BSplineCurve) myBSplCurve = GeomConvert::CurveToBSplineCurve(myTrimmed);
            return myBSplCurve;
        }
        //Handle(Geom_BSplineCurve) curve = Handle(Geom_BSplineCurve)::DownCast(acurve);
    }
    else if (shape.ShapeType() == TopAbs_WIRE) {
        TopoDS_Wire wire = TopoDS::Wire(shape);
        Handle(BRepAdaptor_CompCurve) curve = new BRepAdaptor_CompCurve(wire);
        Approx_Curve3d approx(curve, 0.001, GeomAbs_C1, 200, 12);
        if (approx.IsDone() && approx.HasResult()) {
            Handle(Geom_BSplineCurve) anApproximatedCurve = approx.Curve();
            return anApproximatedCurve;
        }
    }
}

Handle(Geom_BSplineSurface) Surface::BlendSurface::RuledSurface()
{
    Handle(Geom_BSplineCurve) curve1 = getCurve(_shape1);
    Handle(Geom_BSplineCurve) curve2 = getCurve(_shape2);
    Handle(Geom_Surface) aSurf = GeomFill::Surface(curve1, curve2);
    return Handle(Geom_BSplineSurface)::DownCast(aSurf);
}

Handle(Geom_BSplineCurve) Surface::BlendSurface::IsoCurve(int index, int nbSamples)
{
    //fp + relativeValue * (lp - fp);
    Standard_Real u1, u2, v1, v2;
    Standard_Real First, Last;

    _initialSurface->Bounds(u1, u2, v1, v2);

    int relativeToReal = u1 + nbSamples * (u2 - u1);
 
    Handle(Geom_Curve) isoCurve = _initialSurface.get()->UIso(relativeToReal);
    Handle(Geom_TrimmedCurve) myTrimmed = new Geom_TrimmedCurve(isoCurve, First, Last);
    Handle(Geom_BSplineCurve) myBSplCurve = GeomConvert::CurveToBSplineCurve(myTrimmed);
    return myBSplCurve;
}
