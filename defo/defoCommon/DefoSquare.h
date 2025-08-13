/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////


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
  DefoSquare() {}
  DefoSquare( const DefoPoint& theCenter, const unsigned int& theHalfWidth ) { centerPoint_ = theCenter; halfWidth_ = theHalfWidth; }
  DefoPoint const& getCenter( void ) const { return centerPoint_; }
  unsigned int getHalfWidth( void ) const { return halfWidth_; }
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
  DefoSquareCollection() : std::vector<DefoSquare>() {};
  bool isInside( const DefoPoint& ) const;
//  const DefoSquare* getSquareAtPoint( const DefoPoint& ) const;
  void makeEmpty( void );

};

#endif
