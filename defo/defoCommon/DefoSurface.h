#ifndef _DEFOSURFACE_H
#define _DEFOSURFACE_H

#include "DefoSpline.h"
#include "DefoPoint.h"

///
/// a container for surfae reco results
///
class DefoSurface {

 public:
  DefoSurface();
  void setPoints( DefoPointCollection const& points ) { points_ = points; }
  DefoPointCollection const& getPoints( void ) { return points_; }
  DefoSplineField const& getSplineField( void ) const { return splineField_; }
  void setSplineField( DefoSplineField const& field ) { splineField_ = field; isSplineField_ = true; }
  DefoPointFields const& getPointFields( void ) const { return pointFields_; }
  void setPointFields( DefoPointFields const& fields ) { pointFields_ = fields; isPoints_ = true; }
  void dumpSplineField( void ) const;
  void createPointFields( void );

 private:
  DefoPointCollection points_;
  DefoSplineField splineField_;
  DefoPointFields pointFields_;
  
  bool isSplineField_;
  bool isPoints_;
};

#endif
