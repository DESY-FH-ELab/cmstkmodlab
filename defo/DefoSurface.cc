
#include <iostream>
#include <iomanip>

#include "DefoSurface.h"


///
///
///
DefoSurface::DefoSurface() {

  isSplineField_ = false;

}



///
///
///
void DefoSurface::dumpSplineField( void ) const {

  // first "along-x" splines
  DefoSplineSetXCollection const& splinesX = splineField_.first;
  
  // loop set of spline "rows", itC is a DefoSplineSetX
  for( DefoSplineSetXCollection::const_iterator itC = splinesX.begin(); itC < splinesX.end(); ++itC ) {
    
    // loop the points
    for( DefoPointCollection::const_iterator itP = itC->getPoints().begin(); itP < itC->getPoints().end(); ++itP ) {
      
      std::cout << "HX: "
		<< std::setw( 14 ) << itP->getX() 
		<< std::setw( 14 ) << itP->getY() 
		<< std::setw( 14 ) << itC->eval( itP->getX() )
		<< std::endl;

    }

  }

  // then "along-y" splines
  DefoSplineSetYCollection const& splinesY = splineField_.second;

  // loop set of spline "rows", itC is a DefoSplineSetX
  for( DefoSplineSetYCollection::const_iterator itC = splinesY.begin(); itC < splinesY.end(); ++itC ) {
    
    // loop the points
    for( DefoPointCollection::const_iterator itP = itC->getPoints().begin(); itP < itC->getPoints().end(); ++itP ) {
      
      std::cout << "HY: "
		<< std::setw( 14 ) << itP->getX() 
		<< std::setw( 14 ) << itP->getY() 
		<< std::setw( 14 ) << itC->eval( itP->getY() )
		<< std::endl;

    }

  }


}




///
/// create pair of DefoPointField,
/// one for x-splines, one for y-splines;
///
void DefoSurface::createPointFields( void ) {

  if( !isSplineField_ ) {
    std::cerr << " [DefoSurface::createPointFields] ** ERROR: spline field not set" << std::endl;
    throw;
  }

  // determine index range
  std::pair<unsigned int, unsigned int> indexRange( 0, 0 );
  for( DefoPointCollection::const_iterator it = points_.begin(); it != points_.end(); ++it ) {
    if( abs( it->getIndex().first  ) > (int)indexRange.first  ) indexRange.first  = abs( it->getIndex().first   );
    if( abs( it->getIndex().second ) > (int)indexRange.second ) indexRange.second = abs( it->getIndex().second  );

    if( !it->isIndexed() ) {
      std::cerr << " [DefoRecoSurface::indexPoints] ** WARNING: Point not indexed at posistion: x: " 
		<< it->getX() << " y: " << it->getY() << std::endl;
    }

  }


  // create matrix accordingly
  pointFields_.first.resize( indexRange.first * 2 + 1 );
  for( DefoPointField::iterator it = pointFields_.first.begin(); it != pointFields_.first.end(); ++it ) {
    it->resize( indexRange.second * 2 + 1 );
  }

  std::cout << " [DefoSurface::createPointFields] =2= matrix dimensions: x: " << indexRange.first << " y: " << indexRange.second << std::endl;

  

  // first "along-x" splines
  DefoSplineSetXCollection const& splinesX = splineField_.first;
  
  // loop set of spline "rows", itC is a DefoSplineSetX
  for( DefoSplineSetXCollection::const_iterator itC = splinesX.begin(); itC < splinesX.end(); ++itC ) {
  
    // loop the points, determine matrix index
    std::vector<DefoPoint> aColumn;
    for( DefoPointCollection::const_iterator itP = itC->getPoints().begin(); itP < itC->getPoints().end(); ++itP ) {
      DefoPoint aPoint( *itP ); // make copy to preserve constness
      aPoint.setHeight( itC->eval( itP->getX() ) );
      aPoint.setValid( true ); // sign for the DefoSurfacePlot to take it
      std::pair<int,int> absIndex = aPoint.getIndex();
      absIndex.first += indexRange.first;
      absIndex.second += indexRange.second;

//       std::cout << "ABSIND: " 
// 		<< std::setw( 5 ) << aPoint.getIndex().first 
// 		<< std::setw( 5 ) << aPoint.getIndex().second 
// 		<< std::setw( 5 ) << absIndex.first 
// 		<< std::setw( 5 ) << absIndex.second
// 		<< std::endl;

      pointFields_.first.at( absIndex.first ).at( absIndex.second ) = aPoint; // uuaargh..

    }

  }


  /////////////////////////////////
  /////////////////////////////////
  /////////////////////////////////
  // CLONE FOR THE MOMENT

  pointFields_.second = pointFields_.first; ////////////////////////////////////////////


}

