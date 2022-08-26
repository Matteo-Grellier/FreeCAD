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

#ifndef FEATURE_BLEND_SURFACE_H
#define FEATURE_BLEND_SURFACE_H

#include <App/PropertyLinks.h>
#include <App/PropertyStandard.h>
#include <App/PropertyUnits.h>
#include <Mod/Part/App/FeaturePartSpline.h>
#include <Mod/Surface/SurfaceGlobal.h>

namespace Surface
{

class SurfaceExport FeatureBlendSurface: public Part::Spline
{
    PROPERTY_HEADER_WITH_OVERRIDE(Surface::FeatureBlendSurface);

public:

    FeatureBlendSurface();

    App::PropertyLinkSub StartEdge;
    App::PropertyLinkSub StartFace;
    App::PropertyIntegerConstraint StartContinuity;
    App::PropertyFloatList StartSize;

    App::PropertyLinkSub EndEdge;
    App::PropertyLinkSub EndFace;
    App::PropertyIntegerConstraint EndContinuity;
    App::PropertyFloatList EndSize;

    App::PropertyInteger nbSamples;

    TopoDS_Shape getEdge(App::PropertyLinkSub);

    TopoDS_Shape getFace(App::PropertyLinkSub);
    
    void sizeDiff(App::PropertyFloatList StartSize, App::PropertyFloatList EndSize);

    App::DocumentObjectExecReturn *execute(void) override;
    short mustExecute() const override;

    const char *getViewProviderName(void) const override
    {
        return "SurfaceGui::ViewProviderBlendCurve";
    }

private:
    bool lockOnChangeMutex;

protected:
    virtual void onChanged(const App::Property *prop) override;
};

}//Namespace Surface

#endif
