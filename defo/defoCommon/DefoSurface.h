#ifndef _DEFOSURFACE_H
#define _DEFOSURFACE_H

#include <QHash>

#include <nspline2D.h>

#include "DefoSpline.h"
#include "DefoPoint.h"

class DefoSplineXYPair
{
public:
  DefoSplineXYPair(const double& x = 0, const double& y = 0)
    :x_(x), y_(y) {}
  int ix_, iy_;
  double x_, y_;
};

class DefoSplineXYDefoPair
{
public:
  DefoSplineXYDefoPair()
    :x_(0), y_(0), hasx_(false), hasy_(false) {}
  void setX(const double &x) { x_ = x; hasx_ = true; }
  void setY(const double &y) { y_ = y; hasy_ = true; }
  double x_, y_;
  bool hasx_, hasy_;
};

class DefoSurfaceSummary {

 public:
    std::pair<double,double> posAtMinZFromXSplines;
    double minZFromXSplines;
    std::pair<double,double> posAtMaxZFromXSplines;
    double maxZFromXSplines;
    std::pair<double,double> posAtMinZFromYSplines;
    double minZFromYSplines;
    std::pair<double,double> posAtMaxZFromYSplines;
    double maxZFromYSplines;
};

///
/// a container for surfae reco results
///
class DefoSurface {

 public:
  DefoSurface();
  DefoSurface(const DefoSurface& other);
  void makeSummary();
  const DefoSurfaceSummary& getSummary() const;

  void setPoints( DefoPointCollection const& points ) { points_ = points; }
  DefoPointCollection const& getPoints( void ) { return points_; }
  DefoSplineField const& getSplineField( void ) const { return splineField_; }
  void setSplineField( DefoSplineField const& field ) { splineField_ = field; isSplineField_ = true; }
  DefoPointFields const& getPointFields( void ) const { return pointFields_; }
  void setPointFields( DefoPointFields const& fields ) { pointFields_ = fields; isPoints_ = true; }

  void dumpSplineField(std::string filename) const;
  void dumpSpline2DField(std::string filename);
  void dumpSummary( std::string& filename ) const;

  void createPointFields( void );

  void fitSpline2D();
  void calibrateZ(double calibZ);

 private:
  DefoPointCollection points_;
  DefoSplineField splineField_;
  DefoPointFields pointFields_;
  
  bool isSplineField_;
  bool isPoints_;

  QHash<DefoSplineXYPair,DefoSplineXYDefoPair> defoPointMap_;
  DefoSurfaceSummary summary_;

  NSpline2D spline2Dx_;
  NSpline2D spline2Dy_;
};

#endif
