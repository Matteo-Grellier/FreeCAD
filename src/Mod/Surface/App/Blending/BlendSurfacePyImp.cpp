///***************************************************************************
// *   Copyright (c) 2022 Matteo Grellier <matteogrellier@gmail.com>         *
// *                                                                         *
// *   This file is part of the FreeCAD CAx development system.              *
// *                                                                         *
// *   This library is free software; you can redistribute it and/or         *
// *   modify it under the terms of the GNU Library General Public           *
// *   License as published by the Free Software Foundation; either          *
// *   version 2 of the License, or (at your option) any later version.      *
// *                                                                         *
// *   This library  is distributed in the hope that it will be useful,      *
// *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
// *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
// *   GNU Library General Public License for more details.                  *
// *                                                                         *
// *   You should have received a copy of the GNU Library General Public     *
// *   License along with this library; see the file COPYING.LIB. If not,    *
// *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
// *   Suite 330, Boston, MA  02111-1307, USA                                *
// *                                                                         *
// ***************************************************************************/

#include "PreCompiled.h"

#include "Blending/BlendSurfacePy.h"
#include "Blending/BlendSurfacePy.cpp"
#include "Blending/BlendPointPy.h"
#include <Base/VectorPy.h>
#include <Mod/Part/App/BezierCurvePy.h>

using namespace Surface;

std::string BlendSurfacePy::representation() const
{
    return "BlendSurface";
}

PyObject *BlendSurfacePy::PyMake(struct _typeobject *, PyObject *, PyObject *)// Python wrapper
{
    // create a new instance of BlendCurvePy
    return nullptr;
}

int BlendSurfacePy::PyInit(PyObject *args, PyObject * /*kwds*/)
{
    return 0;
}

PyObject *BlendSurfacePy::getCustomAttributes(const char * /*attr*/) const
{
    return nullptr;
}

int BlendSurfacePy::setCustomAttributes(const char * /*attr*/, PyObject * /*obj*/)
{
    return 0;
}
