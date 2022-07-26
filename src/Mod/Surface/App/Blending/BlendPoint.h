/***************************************************************************
 *   Copyright (c) 2014 Matteo Grellier <matteogrellier@gmail.com>         *
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

#ifndef BLEND_POINT_H
#define BLEND_POINT_H

#include <App/PropertyLinks.h>
#include <App/PropertyStandard.h>
#include <App/PropertyUnits.h>
#include <Mod/Part/App/PartFeature.h>
#include <Mod/Surface/SurfaceGlobal.h>

namespace Surface
{

/*!
*   Create a list of vectors formed by a point and some derivatives 
*   obtained from a curve or surface
*/ 
class SurfaceExport BlendPoint
{
public:
    std::vector<Base::Vector3d> vectors;
    /*!
    *  Constructor
    *\param std::vector<Base::Vector3d>
    */
    BlendPoint(std::vector<Base::Vector3d> vectorList);

    /*!
    *  Resizes the blendpoint vectors
    *\param scaling factor
    */
    void multiply(double f);

private:
};
}// namespace Surface

#endif

