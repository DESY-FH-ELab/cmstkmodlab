
#include "DefoRawImage.h"


///
/// check if point coordinates comply with the scheme:
/// |  0 topLeft | 1 topRight  |
/// |2 bottomLeft|3 bottomRight|
///
/// (note: y goes top->down in the image)
///
bool DefoRawImageArea::consistencyCheck( void ) {
  
  if( points_.at( 0 ).getX() < points_.at( 1 ).getX() &&
      points_.at( 2 ).getX() < points_.at( 3 ).getX() &&
      points_.at( 0 ).getY() < points_.at( 2 ).getY() &&
      points_.at( 1 ).getY() < points_.at( 3 ).getX() ) return true;

  return false;
  
}


///
/// draw a field of splines in the image
///
void DefoRawImage::draw( DefoSplineField const& field ) {

  // draw x splines (SAME-Y, i.e. ALONG-X)
  for( DefoSplineSetXCollection::const_iterator it = field.first.begin(); it < field.first.end(); ++it ) {

    // "it" is one row with "SAME-Y"
    std::vector<DefoSpline> const& splines = it->getSplines();

    // loop all splines (row fragments)
    for( std::vector<DefoSpline>::const_iterator spIt = splines.begin(); spIt < splines.end(); ++spIt ) {
      
      std::pair<double,double> const& range = spIt->getValidityRange();

      // go pixel wise
      for( unsigned int px = round( range.first ); px < round( range.second ); px += 2 ) {
	image_.setPixel( px, round( spIt->eval( px ) ), qRgb( 0, 127, 0 ) );
      }
 
    }

  }

  // same for y splines (SAME-X, i.e. ALONG-Y)
  for( DefoSplineSetYCollection::const_iterator it = field.second.begin(); it < field.second.end(); ++it ) {

    // "it" is one row with "SAME-X"
    std::vector<DefoSpline> const& splines = it->getSplines();

    // loop all splines (row fragments)
    for( std::vector<DefoSpline>::const_iterator spIt = splines.begin(); spIt < splines.end(); ++spIt ) {
      
      std::pair<double,double> const& range = spIt->getValidityRange();

      // go pixel wise
      for( unsigned int py = round( range.first ); py < round( range.second ); py += 2 ) {
	image_.setPixel( round( spIt->eval( py ) ), py, qRgb( 127, 127, 0 ) );
      }
 
    }

  }


}
