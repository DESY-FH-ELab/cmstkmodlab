
#ifndef _DEFOSQUARE_H
#define _DEFOSQUARE_H


#include <QImage>
#include "DefoPoint.h"

class DefoSquareIterator;


///
///
///
class DefoSquare {

  friend class DefoSquareIterator;

 public:
  DefoSquare( const DefoPoint& theCenter, const unsigned int& theHalfWidth ) { centerPoint_ = theCenter; halfWidth_ = theHalfWidth; }
  DefoPoint& getCenter( void ) { return centerPoint_; }
  unsigned int getHalfWidth( void ) { return halfWidth_; }
  void setCenter( const DefoPoint& theCenter ) { centerPoint_ = theCenter; }
  void setWidth( const unsigned& theHalfWidth ) { halfWidth_ = theHalfWidth; }
  bool isInside( const DefoPoint& ) const;
  bool isOnBorder( const DefoPoint& ) const;
  bool isFitInImage( const QImage& ) const;
  const DefoSquareIterator begin( void ) const;
  const DefoSquareIterator end( void ) const;
  void draw( QImage& ) const;
  typedef DefoSquareIterator iterator;

 private:
  DefoPoint centerPoint_;
  unsigned int halfWidth_;

};



///
/// iterator for pixels in a square,
/// operator++ hierarchy: first x, then y
///
class DefoSquareIterator : public DefoSquare, public DefoPoint {
  
 public:
  DefoSquareIterator( const DefoSquare& aSquare, const DefoPoint& aPoint ) : DefoSquare( aSquare ), DefoPoint( aPoint ) {}
  const DefoPoint getPoint( void ) const;
  void operator++( void );
  bool operator<=( const DefoSquareIterator& ); 

};



//
//
//
class DefoSquareCollection : public std::vector<DefoSquare> {

 public:
  bool isInside( const DefoPoint& ) const;
  void makeEmpty( void );

};

#endif
