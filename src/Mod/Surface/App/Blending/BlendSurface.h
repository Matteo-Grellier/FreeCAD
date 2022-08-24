/***************************************************************************
 *   Copyright (c) 2022 Matteo Grellier <matteogrellier@gmail.com>         *
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

#ifndef BLEND_SURFACE_H
#define BLEND_SURFACE_H


#include <Geom_BoundedSurface.hxx>
#include <GeomFill_FillingStyle.hxx>
#include <ShapeExtend_WireData.hxx>

#include <Mod/Part/App/FeaturePartSpline.h>
#include <Mod/Surface/SurfaceGlobal.h>

namespace Surface
{
/*!
* Create a BezierCurve interpolating a list of BlendPoints
*/
class SurfaceExport BlendSurface
{
public:

    

    BlendSurface() = default;
    
    ~BlendSurface() = default;
    BlendSurface(TopoDS_Shape,TopoDS_Shape);


    Geom_BSplineSurface RuleSurface();

    Geom_BSplineCurve getCurve(int, const TopoDS_Shape& shape);
private:
    TopoDS_Shape shape1 ;
    const TopoDS_Shape& shape2 ;
};

}// namespace Surface

#endif

