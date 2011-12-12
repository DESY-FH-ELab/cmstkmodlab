

#include "DefoSquare.h"


///
///
///
bool DefoSquare::isInside( const DefoPoint& aPoint ) const {

  if( ( aPoint.getX() < centerPoint_.getX() + halfWidth_ ) &&
      ( aPoint.getX() > centerPoint_.getX() - halfWidth_ ) &&
      ( aPoint.getY() < centerPoint_.getY() + halfWidth_ ) &&
      ( aPoint.getY() > centerPoint_.getY() - halfWidth_ )    ) return true;

  return false;

}



///
///
///
bool DefoSquare::isOnBorder( const DefoPoint& point ) const {

  // summands of -1 are due to rounding (-> appear only at upper borders)
  
  if( centerPoint_.getPixX() - halfWidth_     == point.getPixX() ||
      centerPoint_.getPixX() + halfWidth_ - 1 == point.getPixX() ||
      centerPoint_.getPixY() - halfWidth_     == point.getPixY() ||
      centerPoint_.getPixY() + halfWidth_ - 1 == point.getPixY()    ) return true;

  return false;

}



///
/// does this square fit in theImage?
///
bool DefoSquare::isFitInImage( const QImage& theImage ) const {

  if( ( centerPoint_.getX() + halfWidth_ > theImage.width() ) ||
      ( centerPoint_.getX() - halfWidth_ < 0) ||
      ( centerPoint_.getY() + halfWidth_ > theImage.height() ) ||
      ( centerPoint_.getY() - halfWidth_ < 0)  ) return false;

  return true;

}



///
/// draw the square in theImage
///
void DefoSquare::draw( QImage& theImage ) const {

  for( iterator it = this->begin(); it <= this->end(); ++it ) {
    if( isOnBorder( it.getPoint() ) ) {
      theImage.setPixel( it.getPoint().getX(), it.getPoint().getY(), qRgb( 0, 0, 255 ) );
    }
  }

}



///
///
///
const DefoSquareIterator DefoSquare::begin( void ) const {

  return DefoSquareIterator( *this, DefoPoint( centerPoint_.getX() - halfWidth_, centerPoint_.getY() - halfWidth_ ) );

}



///
///
///
const DefoPoint DefoSquareIterator::getPoint( void ) const {
  
  return DefoPoint( x_, y_ );

}



///
///
///
const DefoSquareIterator DefoSquare::end( void ) const {

  return DefoSquareIterator( *this, DefoPoint( centerPoint_.getX() + halfWidth_, centerPoint_.getY() + halfWidth_ ) );

}



///
///
///
bool DefoSquareIterator::operator<=( const DefoSquareIterator& another ) {

  if( x_ <= another.getX() && y_ <= another.getY() ) return true;
  return false;

}



///
///
///
void DefoSquareIterator::operator++() {
  
  if( x_ + 1 < centerPoint_.getX() + halfWidth_ ) {
    ++x_;
  }

  else {
    x_ = centerPoint_.getX() - halfWidth_;
    ++y_;
  }
    
}



///
///
///
bool DefoSquareCollection::isInside( const DefoPoint& aPoint ) const {

  for( DefoSquareCollection::const_iterator it = this->begin(); it < this->end(); ++it ) {
    if( it->isInside( aPoint ) ) {
//       std::cout << "REJ: " << aPoint.getX() << " " << aPoint.getY() << " " << it->getCenter().getX() << " " << it->getCenter().getY()
//       		<< " " << it->getHalfWidth() << std::endl;
      return true;
    }
  }

  return false;

}



///
///
///
void DefoSquareCollection::makeEmpty( void ) {
  
  while( size() ) pop_back();

}
