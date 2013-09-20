#include "DefoSurfacePlotViewSettings.h"

DefoSurfacePlotViewSettings::DefoSurfacePlotViewSettings()
{
    reset();
}

void DefoSurfacePlotViewSettings::reset()
{
    xyScale_ = 0.9;
    zScale_ = 1.5;

    nIsolines_ = 10;

    is3D_ = true;
    isShade_ = true;
    isMesh_ = true;
    isAxes_ = true;
    isIsolines_ = true;
    isLegend_ = true;

    xRot_ = 20.0;
    yRot_ = 0.0;
    zRot_ = 45.0;
}
