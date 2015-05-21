#include <cmath>

#include <nqlogger.h>

#include "DefoPoint.h"

DefoPoint::DefoPoint( const DefoPoint& other ) {

  x_ = other.x_;
  cx_ = other.cx_;
  y_ = other.y_;
  cy_ = other.cy_;
  slope_ = other.slope_;
  height_ = other.height_;
  color_ = other.color_;
  index_ = std::pair<int,int>(other.index_.first, other.index_.second);
  isIndexed_ = other.isIndexed_;
  isValid_ = other.isValid_;
  imageDistanceX_ = other.imageDistanceX_;
  gridDistanceX_ = other.gridDistanceX_;
  imageDistanceY_ = other.imageDistanceY_;
  gridDistanceY_ = other.gridDistanceY_;
  isCalibrated_ = other.isCalibrated_;
}

///
///
///
void DefoPoint::init( void ) {

  x_ = 0.;
  cx_ = 0.;
  y_ = 0.;
  cy_ = 0.;
  slope_ = 0.;
  height_ = 0.;
  isValid_ = false;
  index_ = std::make_pair<int,int>( 0, 0 );
  imageDistanceX_ = 0.;
  gridDistanceX_ = 0.;
  imageDistanceY_ = 0.;
  gridDistanceY_ = 0.;
  isCalibrated_ = false;
}

double DefoPoint::getDistance(const DefoPoint& other) const {
    double dx = x_ - other.x_;
    double dy = y_ - other.y_;
    return std::sqrt(dx*dx+dy*dy);
}

double DefoPoint::getDistanceXY(const DefoPoint& other, double& dx, double& dy) const {
    dx = x_ - other.x_;
    dy = y_ - other.y_;
    return std::sqrt(dx*dx+dy*dy);
}

///
/// Allows for non-hardcoded selection of coordinate
///
const double & DefoPoint::getPosition( const DefoPoint::Axis& coordinate ) const
{
  switch ( coordinate ) {
    case DefoPoint::X:
      return x_;
      break;
    default: // DefoPoint::Y
      return y_;
  }
}

///
/// Allows for non-hardcoded selection of coordinate
///
const double & DefoPoint::getCalibratedPosition( const DefoPoint::Axis& coordinate ) const
{
  switch ( coordinate ) {
    case DefoPoint::X:
      return cx_;
      break;
    default: // DefoPoint::Y
      return cy_;
  }
}

double DefoPoint::getCorrectionFactor( const DefoPoint::Axis& coordinate ) const
{
  double correction;

  switch ( coordinate ) {
    case DefoPoint::X:
      correction = (getGridDistanceX()+getImageDistanceX())/(2.0*getGridDistanceX());
      break;
    default: // DefoPoint::Y
      correction = (getGridDistanceY()+getImageDistanceY())/(2.0*getGridDistanceY());
  }

  //correction = 1.0;

  return correction;
}

/**
  Sets the color of the point.
  */
void DefoPoint::setColor( const QColor& color )
{
  color_ = color;
}

/**
  Returns the currently set color of the point or an invalid QColor (black)
  if none has been set.
  */
const QColor& DefoPoint::getColor( void ) const
{
  return color_;
}

bool DefoPoint::hasReferenceColor(const QColor& seedColor) const
{
  float dhue = color_.hsvHueF() - seedColor.hsvHueF();
  if (std::fabs(dhue) > 0.5)
    dhue = 1 - std::fabs(dhue);
  float dsat = color_.hsvSaturationF() - seedColor.hsvSaturationF();
  float d = std::sqrt(dhue*dhue+dsat*dsat);
  if (d<0.1) {
      return true;
  }
  return false;
}

std::ostream& operator<<(std::ostream& out, const DefoPoint& p)
{
  out << '(' << p.getX() << ',' << p.getY() << ')';
  return out;
}


///
///
///
DefoPoint operator*( DefoPoint const& point, double const& scalar )
{
  return DefoPoint( point.getX() * scalar, point.getY() * scalar );
}



///
///
///
DefoPoint operator-( DefoPoint const& point, DefoPoint const& another )
{
  return DefoPoint( point.getX() - another.getX(), point.getY() - another.getY() );
}



///
///
///
DefoPoint operator+( DefoPoint const& point, DefoPoint const& another )
{
  return DefoPoint( point.getX() + another.getX(), point.getY() + another.getY() );
}



///
/// for sorting DefoPoints according to increasing X
///
bool DefoPointXPredicate( DefoPoint const& one, DefoPoint const& another )
{
  return( one.getX() < another.getX() );
}



///
///for sorting DefoPoints according to increasing Y
///
bool DefoPointYPredicate( DefoPoint const& one, DefoPoint const& another )
{
  return( one.getY() < another.getY() );
}



///
/// for sorting DefoPointCollections according to the average X of their points
///
bool DefoPointCollectionAverageXPredicate( DefoPointCollection const& one,
                                           DefoPointCollection const& another )
{
  std::pair<double,double> average = std::make_pair<double,double>( 0., 0. );
  std::pair<unsigned int,unsigned int> nPoints = std::make_pair<unsigned int,unsigned int>( 0, 0 );
  for( DefoPointCollection::const_iterator it = one.begin(); it < one.end(); ++it ) average.first += it->getX(), ++nPoints.first;
  for( DefoPointCollection::const_iterator it = another.begin(); it < another.end(); ++it ) average.second += it->getX(), ++nPoints.second;
  
  if( !( nPoints.first && nPoints.second ) ) {
    NQLogCritical("DefoPointCollectionAverageXPredicate")
        << "Empty input collection, sorting will fail.";
    return false;
  }

  return ( average.first / nPoints.first ) < ( average.second / nPoints.second );
}



///
/// for sorting DefoPointCollections according to the average Y of their points
///
bool DefoPointCollectionAverageYPredicate( DefoPointCollection const& one,
                                           DefoPointCollection const& another )
{
  std::pair<double,double> average = std::make_pair<double,double>( 0., 0. );
  std::pair<unsigned int,unsigned int> nPoints = std::make_pair<unsigned int,unsigned int>( 0, 0 );
  for( DefoPointCollection::const_iterator it = one.begin(); it < one.end(); ++it ) average.first += it->getY(), ++nPoints.first;
  for( DefoPointCollection::const_iterator it = another.begin(); it < another.end(); ++it ) average.second += it->getY(), ++nPoints.second;
  
  // TO BE FIXED LATER IF IT OCCURS!!
  if( !( nPoints.first && nPoints.second ) ) {
    NQLogCritical("DefoPointCollectionAverageYPredicate")
        << "Empty input collection, sorting will fail.";
    return false;
  }

  return ( average.first / nPoints.first ) < ( average.second / nPoints.second );
}

///
/// for sorting std::pair<DefoPoint,DefoPoint> according to second.abs();
/// (for use e.g. when the second is a difference)
/// 
bool DefoPointPairSecondAbsPredicate( std::pair<DefoPointCollection::iterator,DefoPoint> const& one, 
                                      std::pair<DefoPointCollection::iterator,DefoPoint> const& another )
{
  return( one.second.abs() < another.second.abs() );
}
