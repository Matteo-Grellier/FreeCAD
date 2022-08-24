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
#include <BSplCLib.hxx>
#include <Geom_BezierCurve.hxx>
#include <Precision.hxx>
#include <Standard_Version.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <gp_Pnt.hxx>
#include <math_Gauss.hxx>
#include <math_Matrix.hxx>
#include <TopoDS.hxx>
#endif
#include "Blending/BlendSurface.h"
#include <Base/Vector3D.h>
#include <Mod/Part/App/Geometry.h>

using namespace Surface;

BlendSurface(TopoDS_Shape shape1,TopoDS_Shape shape2);
{
    
}

Geom_BSplineCurve getCurve(int i, TopoDS_Shape shape){
    if (shape == TopoDS_Edge)
    {
        TopoDS_Edge edge = TopoDS::Edge(shape);
        BRepAdaptor_Curve curve(edge);
        return bspline;
    }else if(shape == TopoDS_Wire){
        TopoDS_Wire wire = TopoDS::Wire(shape);
        BRepAdaptor_Curve curve(wire);
        return bspline;
    }
    
}