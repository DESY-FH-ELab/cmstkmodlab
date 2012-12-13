
#ifndef _DEFOPOINTCOLLECTION_H
#define _DEFOPOINTCOLLECTION_H

#include <vector>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <cmath>

#include <QColor>


///
/// DefoPoint is the "swiss army-knife" coordinate container.
/// Normally a point on the CCD chip with indices,
/// height & slope values attached (if needed)
///
class DefoPoint {

 public:
  enum Axis {
      X,
      Y
  };

  DefoPoint() { init(); }
  DefoPoint( double x, double y ) { init(); x_ = x; y_ = y; }
  DefoPoint( double x, double y, double slope ) { init(); x_ = x; y_ = y; slope_ = slope; }
  DefoPoint( const DefoPoint& other );
  void setX( double x ) { x_ = x; }
  void setY( double y ) { y_ = y; }
  void setPosition( double x, double y ) { x_ = x; y_ = y; }
  void setSlope( double slope ) { slope_ = slope; }
  void setHeight( double height ) { height_ = height; }
  // TODO setIndex(Axis, int)
  void setIndex( std::pair<int,int>& index ) { index_ = index; isIndexed_ = true; }
  void setIndex( int indexX, int indexY ) { index_.first = indexX; index_.second = indexY; isIndexed_ = true; }
  void setIndexX( int index ) { index_.first  = index; isIndexed_ = true; }
  void setIndexY( int index ) { index_.second = index; isIndexed_ = true; }
  void unindex() { isIndexed_ = false; }
  bool isIndexed( void ) const { return isIndexed_; }
  double const& getX( void ) const { return x_; }
  double const& getY( void ) const { return y_; }
  double getDistanceXY(const DefoPoint& other, double& dx, double& dy) const;
  double const& getPosition( const DefoPoint::Axis& ) const;
  double const& getSlope( void ) const { return slope_; }
  double const& getHeight( void ) const { return height_; }
  double abs( void ) const { return sqrt( pow( x_, 2. ) + pow( y_, 2. ) ); }
  const std::pair<int,int> getIndex( void ) const { return index_; }
  unsigned int getPixX( void ) const { return static_cast<int>( round( x_ ) ); }
  unsigned int getPixY( void ) const { return static_cast<int>( round( y_ ) ); }
  void setColor( const QColor& color );
  const QColor& getColor( void ) const;
  bool hasReferenceColor(const QColor& seedColor) const;
  bool isBlue( void ) const;
  void setValid( bool isValid ) { isValid_ = isValid; }
  bool isValid( void ) const { return isValid_; }
  DefoPoint operator/=( const double& aFactor ) { x_ /= aFactor; y_ /= aFactor; return *this; }
  DefoPoint operator+=( const DefoPoint& another ) { x_ += another.getX(); y_ += another.getY(); return *this; }
  DefoPoint operator-=( const DefoPoint& another ) { x_ -= another.getX(); y_ -= another.getY(); return *this; }
  bool operator<( const DefoPoint& another ) const { return( sqrt( pow( x_, 2. ) + pow( y_, 2. ) ) < 
							    sqrt( pow( another.getX(), 2. ) + pow( another.getY(), 2. ) ) ); }
  bool operator<=( const DefoPoint& another ) const { return( sqrt( pow( x_, 2. ) + pow( y_, 2. ) ) <=
							    sqrt( pow( another.getX(), 2. ) + pow( another.getY(), 2. ) ) ); }
  bool operator>( const DefoPoint& another ) const { return( sqrt( pow( x_, 2. ) + pow( y_, 2. ) ) > 
							    sqrt( pow( another.getX(), 2. ) + pow( another.getY(), 2. ) ) ); }
  bool operator>=( const DefoPoint& another ) const { return( sqrt( pow( x_, 2. ) + pow( y_, 2. ) ) >= 
							    sqrt( pow( another.getX(), 2. ) + pow( another.getY(), 2. ) ) ); }

 protected:
  void init( void );
  double x_;
  double y_;
  double slope_;
  double height_;
  QColor color_;
  std::pair<int,int> index_;
  bool isIndexed_;
  bool isValid_; // contains valid height data? for use with DefoSurfacePlot..

};


std::ostream& operator<< (std::ostream& out, const DefoPoint& p);

typedef std::vector<DefoPoint> DefoPointCollection;
typedef std::vector<std::vector<DefoPoint> > DefoPointField;
typedef std::pair<DefoPointField,DefoPointField> DefoPointFields;
DefoPoint operator*( DefoPoint const&, double const& );
DefoPoint operator-( DefoPoint const&, DefoPoint const& );
DefoPoint operator+( DefoPoint const&, DefoPoint const& );

bool DefoPointXPredicate( DefoPoint const&, DefoPoint const& );
bool DefoPointYPredicate( DefoPoint const&, DefoPoint const& );
bool DefoPointCollectionAverageXPredicate( DefoPointCollection const&, DefoPointCollection const& );
bool DefoPointCollectionAverageYPredicate( DefoPointCollection const&, DefoPointCollection const& );
bool DefoPointPairSecondAbsPredicate( std::pair<DefoPointCollection::iterator,DefoPoint> const&, 
				      std::pair<DefoPointCollection::iterator,DefoPoint> const& );
#endif
