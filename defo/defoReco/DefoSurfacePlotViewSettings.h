#ifndef DEFOSURFACEPLOTVIEWSETTINGS_H
#define DEFOSURFACEPLOTVIEWSETTINGS_H

class DefoSurfacePlotViewSettings
{
public:
    DefoSurfacePlotViewSettings();
    void reset();

    double xyScale_;
    double zScale_;

    int nIsolines_;

    bool is3D_;
    bool isShade_;
    bool isMesh_;
    bool isAxes_;
    bool isIsolines_;
    bool isLegend_;

    double xRot_, yRot_, zRot_;
};

#endif // DEFOSURFACEPLOTVIEWSETTINGS_H
