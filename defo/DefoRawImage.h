
#ifndef _DEFORAWIMAGE_H
#define _DEFORAWIMAGE_H

#include <cmath>

#include <QImage>

#include "DefoPoint.h"
#include "DefoSpline.h"


///
/// for defining a quadrangle area within an image
/// using four points
///
class DefoRawImageArea {
  
 public:
  DefoRawImageArea() : points_( std::vector<DefoPoint>( 4 ) ) {}
  void clear( void ) { points_.resize( 0 ); }
  DefoPoint const& getTopLeft( void ) { return points_.at( 0 ); }
  DefoPoint const& getTopRight( void ) { return points_.at( 1 ); }
  DefoPoint const& getBottomLeft( void ) { return points_.at( 2 ); }
  DefoPoint const& getBottomRight( void ) { return points_.at( 3 ); }
  void setTopLeft( DefoPoint const& p ) { points_.at( 0 ) = p; }
  void setTopRight( DefoPoint const& p ) { points_.at( 1 ) = p; }
  void setBottomLeft( DefoPoint const& p ) { points_.at( 2 ) = p; }
  void setBottomRight( DefoPoint const& p ) { points_.at( 3 ) = p; }

 private:
  bool consistencyCheck( void );
  std::vector<DefoPoint> points_;

};



///
/// wrapper for qimage
/// with some custom functionality
///
class DefoRawImage {

 public:
  DefoRawImage() {}
  DefoRawImage( const char* fileName ) { image_.load( fileName ); }
  QImage& getImage( void ) { return image_; }
  void save( const char* fileName ) { image_.save( fileName ); }
  void draw( DefoSplineField const& );

 private:
  QImage image_;

};


#endif
