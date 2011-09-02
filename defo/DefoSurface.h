
#ifndef _DEFOSURFACE_H
#define _DEFOSURFACE_H

#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>

#include "DefoSpline.h"
#include "DefoPoint.h"

///
/// a container for surfae reco results
///
class DefoSurface {

  friend class boost::serialization::access;

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

  template<class Archive>
  void serialize( Archive & ar, const unsigned int version ) { 
    ar & points_; 
    ar & splineField_;
    ar & pointFields_;
    ar & isSplineField_;
    ar & isPoints_;
    if( version ) {}; // empty
  }

};

BOOST_CLASS_VERSION( DefoSurface, 0 )

#endif
