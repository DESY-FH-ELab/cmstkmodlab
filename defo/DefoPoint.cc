
#include "DefoPoint.h"


///
///
///
void DefoPoint::init( void ) {
  
  isBlue_ = false;
  x_ = 0.;;
  y_ = 0.;
  slope_ = 0.;
  height_ = 0.;
  isValid_ = false;
  index_ = std::make_pair<int,int>( 0, 0 );

}



///
///
///
DefoPoint operator*( DefoPoint const& point, double const& scalar ) {
  return DefoPoint( point.getX() * scalar, point.getY() * scalar );
}



///
///
///
DefoPoint operator-( DefoPoint const& point, DefoPoint const& another ) {
  return DefoPoint( point.getX() - another.getX(), point.getY() - another.getY() );
}



///
///
///
DefoPoint operator+( DefoPoint const& point, DefoPoint const& another ) {
  return DefoPoint( point.getX() + another.getX(), point.getY() + another.getY() );
}



///
/// for sorting DefoPoints according to increasing X
///
bool DefoPointXPredicate( DefoPoint const& one, DefoPoint const& another ) {
  return( one.getX() < another.getX() );
}



///
///for sorting DefoPoints according to increasing Y
///
bool DefoPointYPredicate( DefoPoint const& one, DefoPoint const& another ) {
  return( one.getY() < another.getY() );
}



///
/// for sorting DefoPointCollections according to the average X of their points
///
bool DefoPointCollectionAverageXPredicate( DefoPointCollection const& one, DefoPointCollection const& another ) {

  std::pair<double,double> average = std::make_pair<double,double>( 0., 0. );
  std::pair<unsigned int,unsigned int> nPoints = std::make_pair<unsigned int,unsigned int>( 0, 0 );
  for( DefoPointCollection::const_iterator it = one.begin(); it < one.end(); ++it ) average.first += it->getX(), ++nPoints.first;
  for( DefoPointCollection::const_iterator it = another.begin(); it < another.end(); ++it ) average.second += it->getX(), ++nPoints.second;
  
  // TO BE FIXED LATER IF IT OCCURS!!
  if( !( nPoints.first && nPoints.second ) ) {
    std::cerr << " [DefoPointCollectionAverageXPredicate] ** ERROR: empty collection." << std::endl;
    throw;
  }

  return ( average.first / nPoints.first ) < ( average.second / nPoints.second );
  
}



///
/// for sorting DefoPointCollections according to the average Y of their points
///
bool DefoPointCollectionAverageYPredicate( DefoPointCollection const& one, DefoPointCollection const& another ) {

  std::pair<double,double> average = std::make_pair<double,double>( 0., 0. );
  std::pair<unsigned int,unsigned int> nPoints = std::make_pair<unsigned int,unsigned int>( 0, 0 );
  for( DefoPointCollection::const_iterator it = one.begin(); it < one.end(); ++it ) average.first += it->getY(), ++nPoints.first;
  for( DefoPointCollection::const_iterator it = another.begin(); it < another.end(); ++it ) average.second += it->getY(), ++nPoints.second;
  
  // TO BE FIXED LATER IF IT OCCURS!!
  if( !( nPoints.first && nPoints.second ) ) {
    std::cerr << " [DefoPointCollectionAverageXPredicate] ** ERROR: empty collection." << std::endl;
    throw;
  }

  return ( average.first / nPoints.first ) < ( average.second / nPoints.second );

}



///
/// for sorting std::pair<DefoPoint,DefoPoint> according to second.abs();
/// (for use e.g. when the second is a difference)
/// 
bool DefoPointPairSecondAbsPredicate( std::pair<DefoPointCollection::iterator,DefoPoint> const& one, 
				      std::pair<DefoPointCollection::iterator,DefoPoint> const& another ) {

  return( one.second.abs() < another.second.abs() );

}
