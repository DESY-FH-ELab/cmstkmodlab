#include <nqlogger.h>

#include "DefoSpline.h"

///
///
///
DefoSplineSetBase::DefoSplineSetBase( DefoPoint::Axis axis ) {

  axis_ = axis;
}

///
/// add offset to all splines in a set
/// by increasing/decreasing their C values;
/// for z-(height-)splines this alters their height
///
void DefoSplineSetBase::offset( double anOffset ) {

  for( std::vector<DefoSpline>::iterator it = splines_.begin(); it < splines_.end(); ++it ) {
    it->setC( it->c() + anOffset );
  }

}



///
/// evaluate a spline set at a given position;
/// assume that the spline set is continuous
///
double DefoSplineSetBase::eval( double pos ) const {

  for( std::vector<DefoSpline>::const_iterator it = splines_.begin(); it < splines_.end(); ++it ) {
    if( it->isInRange( pos ) ) return it->eval( pos );
  }

  // no spline with matching range?
  NQLogCritical("DefoSplineSetBase::eval()")
      << "failed to evaluate spline at pos: " << pos
      << " (range: [ " << validityRange().first << " , "
      << validityRange().second << " ]. Returning 0.";
  
  return 0.;
}



///
///
///
std::pair<double,double> const DefoSplineSetBase::validityRange( void ) const {

  std::pair<double,double> result = std::make_pair<double,double>( 0., 0. );
  result.first = splines_.front().getValidityRange().first;
  result.second = splines_.back().getValidityRange().second;

  return result;

}

bool DefoSplineSetBase::doFitXY( void ) {
  // Adapted from original (duplicate) implementation
  // XXX Why is this method needed??

  // z values for each spline;
  // first value is arbitrary
  // (note: this z is not the height!!)
  std::vector<double> z;
  z.push_back( 0. );

  std::pair<double,double> current = std::make_pair<double,double>( 0., 0. );
  std::pair<double,double> next = std::make_pair<double,double>( 0., 0. );

  // create splines & calculate their parameters
  // we have nPoints - 1 splines in the row
  for( DefoPointCollection::const_iterator it = points_.begin(); it < points_.end() - 1; ++it ) {

    DefoSpline aSpline;

    // select values according to coordinate
    if ( axis_ == DefoPoint::X ) { // SAME-Y = ALONG-X

      current.first = it->getX();
      current.second = it->getY();
      next.first = (it+1)->getX();
      next.second = (it+1)->getY();

    } else { // SAME-X = ALONG-Y

      current.first = it->getY();
      current.second = it->getX();
      next.first = (it+1)->getY();
      next.second = (it+1)->getX();

    }


    // next z comes from recurrence relation
    const double nextZ = -1. * z.back() + 2. * ( next.second - current.second ) / ( next.first - current.first );

    // two shortcuts
    const double thisZ = z.back();
    const double k = ( nextZ - thisZ ) / 2. / ( next.first - current.first );

    // then these are the parameters for the spline (y = ax^2 + bx + c):
    aSpline.setA( k );
    aSpline.setB( thisZ - 2. * k * current.first );
    aSpline.setC( current.second - z.back() * current.first + k * pow( current.first, 2. ) );

    // pushback z for next round
    z.push_back( nextZ );

    // pushback spline
    aSpline.setValidityRange( current.first, next.first );
    splines_.push_back( aSpline );

  }

  // MISSING CHECK

  return true;
}


///
/// fit Z splines
/// for surface reconstruction
///
bool DefoSplineSetBase::doFitZ( void ) {

  // height (h) values for each spline;
  // first value is arbitrary
//  std::vector<double> h;
//  h.push_back( 0. );

  // Only the last value is of interest
  double h = 0.;

  // avoid crash when np = 0, seems to be related to  "points_.end() - 1"
  if( 0 == points_.size() ) {
    NQLogWarning("DefoSplineSetBase::doFitZ")
        << "number of Points is zero.";
    return false;
  }

  // create splines & calculate their parameters
  // we have nPoints - 1 splines in the row
  for( DefoPointCollection::const_iterator it = points_.begin(); it < points_.end() - 1; ++it ) {

    DefoSpline aSpline;
    // use known coordinate to request the correct value
    // FIXME Slower than seperate, but duplicate implementation.
    const double thisValue = it->getPosition(axis_);
    const double nextValue = (it+1)->getPosition(axis_);

    const double thisSlope = it->getSlope();
    const double nextSlope = (it+1)->getSlope();

    const double diff = thisValue - nextValue;

    // set coefficients
    // S_1 = 2Ax_1 + B
    // S_2 = 2Ax_2 + B
    const double a = ( thisSlope - nextSlope ) / 2. / diff;
    const double b = ( nextSlope * thisValue - thisSlope * nextValue ) / diff;

    aSpline.setA( a );
    aSpline.setB( b );

    // h = Ax**2 + Bx + C
//    aSpline.setC( h.back() - ( a * thisValue + b ) * thisValue );
    aSpline.setC( h - ( a * thisValue + b ) * thisValue );

    // store it
    aSpline.setValidityRange( thisValue, nextValue );
    splines_.push_back( aSpline );

    // z value at end point for next spline
    h =  aSpline.eval( nextValue );

  }

  // MISSING CHECK
  return true;

}
