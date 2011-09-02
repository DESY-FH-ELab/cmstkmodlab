
#include "DefoSpline.h"



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
  std::cerr << " [DefoSplineSetBase::eval] ** ERROR: failed to evaluate spline at pos: " << pos << std::endl;
  throw;

}



///
/// fit XY splines
/// (SAME-Y = ALONG-X)
///
bool DefoSplineSetX::doFitXY( void ) {
  
  // z values for each spline;
  // first value is arbitrary
  // (note: this z is not the height!!)
  std::vector<double> z;
  z.push_back( 0. );

  // create splines & calculate their parameters
  // we have nPoints - 1 splines in the row
  for( DefoPointCollection::const_iterator it = points_.begin(); it < points_.end() - 1; ++it ) {
    
    DefoSpline aSpline;

    // next z comes from recurrence relation
    const double nextZ = -1. * z.back() + 2. * ( (it+1)->getY() - it->getY() ) / ( (it+1)->getX() - it->getX() );

    // two shortcuts
    const double thisZ = z.back();
    const double k = ( nextZ - thisZ ) / 2. / ( (it+1)->getX() - it->getX() );

    // then these are the parameters for the spline (y = ax^2 + bx + c):
    aSpline.setA( k );
    aSpline.setB( thisZ - 2. * k * it->getX() );
    aSpline.setC( it->getY() - z.back() * it->getX() + k * pow( it->getX(), 2. ) );

    // pushback z for next round
    z.push_back( nextZ );
    
    // pushback spline
    aSpline.setValidityRange( it->getX(), (it+1)->getX() );
    splines_.push_back( aSpline );

  }

  // MISSING CHECK

  return true;

}



///
/// fit Z splines
/// for surface reconstruction
/// (SAME-Y = ALONG-X)
///
bool DefoSplineSetX::doFitZ( void ) {

  // height (h) values for each spline;
  // first value is arbitrary
  std::vector<double> h;
  h.push_back( 0. );

  // create splines & calculate their parameters
  // we have nPoints - 1 splines in the row
  for( DefoPointCollection::const_iterator it = points_.begin(); it < points_.end() - 1; ++it ) {
    
    DefoSpline aSpline;

    // set coefficiencts
    aSpline.setA( ( it->getSlope() - (it+1)->getSlope() ) / 2. / ( it->getX() - (it+1)->getX() ) );
    aSpline.setB( ( (it+1)->getSlope() * it->getX() - it->getSlope() * (it+1)->getX() ) / ( it->getX() - (it+1)->getX() ) );
    
    // shortcuts for C
    const double c1 = (it+1)->getSlope() * pow( it->getX(), 2. );
    const double c2 = it->getSlope() * it->getX() * ( it->getX() - 2. * (it+1)->getX() );
    const double c3 = 2. * ( (it+1)->getX() - it->getX() ) * h.back();
    const double c4 = 2. * ( it->getX() - (it+1)->getX() );
    aSpline.setC( -1. * ( c1 + c2 + c3 ) / c4 );

    // store it
    aSpline.setValidityRange( it->getX(), (it+1)->getX() );
    splines_.push_back( aSpline );

    // z value at end point for next spline
    h.push_back( aSpline.eval( (it+1)->getX() ) );

    
  }

  // MISSING CHECK
  return true;

}



///
/// like DefoSplineSetX::doFitXY but
/// X <> Y
/// (SAME-X = ALONG-Y)
///
bool DefoSplineSetY::doFitXY( void ) {

  // z values for each spline;
  // first value is arbitrary
  std::vector<double> z;
  z.push_back( 0. );

  // create splines & calculate their parameters
  // we have nPoints - 1 splines in the row
  for( DefoPointCollection::const_iterator it = points_.begin(); it < points_.end() - 1; ++it ) {
    
    DefoSpline aSpline;

    // next z comes from recurrence relation
    const double nextZ = -1. * z.back() + 2. * ( (it+1)->getX() - it->getX() ) / ( (it+1)->getY() - it->getY() );

    // two shortcuts
    const double thisZ = z.back();
    const double k = ( nextZ - thisZ ) / 2. / ( (it+1)->getY() - it->getY() );

    // then these are the parameters for the spline (y = ax^2 + bx + c):
    aSpline.setA( k );
    aSpline.setB( thisZ - 2. * k * it->getY() );
    aSpline.setC( it->getX() - z.back() * it->getY() + k * pow( it->getY(), 2. ) );

    // pushback z for next round
    z.push_back( nextZ );
    
    // pushback spline
    aSpline.setValidityRange( it->getY(), (it+1)->getY() );
    splines_.push_back( aSpline );

  }

  // MISSING CHECK
  return true;

}



///
/// fit Z splines
/// for surface reconstruction
/// (SAME-X = ALONG-Y)
///
bool DefoSplineSetY::doFitZ( void ) {

  // height (h) values for each spline;
  // first value is arbitrary
  std::vector<double> h;
  h.push_back( 0. );

  // create splines & calculate their parameters
  // we have nPoints - 1 splines in the row
  for( DefoPointCollection::const_iterator it = points_.begin(); it < points_.end() - 1; ++it ) {
    
    DefoSpline aSpline;

    // set coefficiencts
    aSpline.setA( ( it->getSlope() - (it+1)->getSlope() ) / 2. / ( it->getY() - (it+1)->getY() ) );
    aSpline.setB( ( (it+1)->getSlope() * it->getY() - it->getSlope() * (it+1)->getY() ) / ( it->getY() - (it+1)->getY() ) );
    
    // shortcuts for C
    const double c1 = (it+1)->getSlope() * pow( it->getY(), 2. );
    const double c2 = it->getSlope() * it->getY() * ( it->getY() - 2. * (it+1)->getY() );
    const double c3 = 2. * ( (it+1)->getY() - it->getY() ) * h.back();
    const double c4 = 2. * ( it->getY() - (it+1)->getY() );
    aSpline.setC( -1. * ( c1 + c2 + c3 ) / c4 );

    // store it
    aSpline.setValidityRange( it->getY(), (it+1)->getY() );
    splines_.push_back( aSpline );

    // z value at end point for next spline
//    std::cout << "HY: " << it->getX() << " " << it->getY() << " " <<  h.back() << std::endl; /////////////////////////////////
    h.push_back( aSpline.eval( (it+1)->getY() ) );
    
  }

  // MISSING CHECK
  return true;

}



// ///
// /// add the normalization ("c") constants from "from" field to those of "to" field;
// /// this is used for averaging them...
// ///
// void DefoAddSplineFieldNormalization( DefoSplineField& to, DefoSplineField const& from ) {
  
//   // check for compatibility in size
//   if( ( to.first.size() != from.first.size() ) || 
//       ( to.second.size() != from.second.size() )  ) {

//     std::cerr << " [DefoAddSplineFieldNormalization] ** ERROR: incompatible size" << std::endl;
//     throw;

//   }

//   DefoSplineSetXCollection::iterator toX;
//   DefoSplineSetXCollection::const_iterator fromX;
//   DefoSplineSetYCollection::iterator toY;
//   DefoSplineSetYCollection::const_iterator fromY;

//   // first
//   for( toX = to.first.begin(), fromX = from.first.begin();
//        toX < to.first.end();
//        ++toX, ++fromX ) {
    
//     // lift "to" splineset by height of first point of "from"
//     toX->offset( fromX->eval( fromX->getPoints().front().getX() ) );

//   }
  

//   // second
//   for( toY = to.second.begin(), fromY = from.second.begin();
//        toY < to.second.end();
//        ++toY, ++fromY ) {

//     // lift "to" splineset by height of first point of "from"
//     toY->offset( fromY->eval( fromY->getPoints().front().getY() ) );

//   }

  
// }
