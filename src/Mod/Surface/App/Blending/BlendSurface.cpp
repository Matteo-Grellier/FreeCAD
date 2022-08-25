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
#include <BRepAdaptor_Curve.hxx>
#include <TopoDS.hxx>
#endif
#include "Blending/BlendSurface.h"


using namespace Surface;

BlendSurface::BlendSurface(TopoDS_Shape shape1, TopoDS_Shape shape2){
    
}

Handle(Geom_BSplineCurve) BlendSurface::getCurve(int i, TopoDS_Shape shape)
{
    Handle(Geom_BSplineCurve) curve;
    if (shape.ShapeType() == TopAbs_EDGE)
    {
        TopoDS_Edge edge = TopoDS::Edge(shape);
        Handle(BRepAdaptor_Curve) acurve = new BRepAdaptor_Curve(edge);
        Handle(Geom_BSplineCurve) curve = Handle(Geom_BSplineCurve)::DownCast(acurve);
    }
    else if (shape.ShapeType() == TopAbs_WIRE) {
        TopoDS_Wire wire = TopoDS::Wire(shape);
        Handle(BRepAdaptor_CompCurve) curve = new BRepAdaptor_CompCurve(wire);
        /*Approx_Curve3d approx(curve, 0.001, GeomAbs_C1, 200, 12);
        if (approx.IsDone() && approx.HasResult()) {
            Handle(Geom_BSplineCurve) anApproximatedCurve = approx.Curve();
            return anApproximatedCurve;
        }*/
        
    }
    Handle(Geom_BSplineCurve) BsCurve = Handle(Geom_BSplineCurve)::DownCast(curve);
    return BsCurve;
    
}