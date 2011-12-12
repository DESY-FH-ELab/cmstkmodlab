
#include "DefoRecoSurface.h"


///
///
///
DefoRecoSurface::DefoRecoSurface() {

  // read parameters
  DefoConfigReader cfgReader( "defo.cfg" );
  spacingEstimate_ = cfgReader.getValue<int>( "SPACING_ESTIMATE" );
  searchPathHalfWidth_ = cfgReader.getValue<int>( "SEARCH_PATH_HALF_WIDTH" );
  nominalGridDistance_ = cfgReader.getValue<double>( "NOMINAL_GRID_DISTANCE" );
  nominalCameraDistance_ = cfgReader.getValue<double>( "NOMINAL_CAMERA_DISTANCE" );
  nominalViewingAngle_ = cfgReader.getValue<double>( "NOMINAL_VIEWING_ANGLE" );
  pitchX_= cfgReader.getValue<double>( "PIXEL_PITCH_X" );
  pitchY_= cfgReader.getValue<double>( "PIXEL_PITCH_Y" );
  focalLength_= cfgReader.getValue<double>( "LENS_FOCAL_LENGTH" );
  debugLevel_ = cfgReader.getValue<unsigned int>( "DEBUG_LEVEL" );

  calculateHelpers();

}



///
/// calculate some helper variables
///
void DefoRecoSurface::calculateHelpers( void ) {

  heightAboveSensor_ = nominalCameraDistance_ * sin( nominalViewingAngle_ );
  if( tan( nominalViewingAngle_ ) != 0. ) horizontalDistanceToSensor_ = heightAboveSensor_ / tan( nominalViewingAngle_ );
  else {
    std::cerr << " [DefoRecoSurface::DefoRecoSurface] ** ERROR: tan(alpha) is zero, check parameters" << std::endl;
    throw;
  }

}


///
/// perform the reconstruction based on two point collections:
/// a) reconstructed points of current image
/// b) reconstructed points of reference image
///
const DefoSurface DefoRecoSurface::reconstruct( DefoPointCollection& currentPoints, DefoPointCollection& referencePoints ) {

  DefoSurface theSurface;

  // // create XY splines for display
  // const DefoSplineField currentXYSplineField = createXYSplines( currentPoints );

  // index the points for matching ref and reco
  std::pair<unsigned int, unsigned int> recoPointIndexRange = indexPoints( currentPoints );
  if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::createZSplines] =2= " 
				   << "reco point index range is: x: "
				   << recoPointIndexRange.first << " y: "
				   << recoPointIndexRange.second << std::endl;

  std::pair<unsigned int, unsigned int> refPointIndexRange = indexPoints( referencePoints );
  if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::createZSplines] =2= " 
				   << "ref point index range is: x: "
				   << refPointIndexRange.first << " y: "
				   << refPointIndexRange.second << std::endl;


  // create raw z splines for surface reconstruction
  DefoSplineField currentZSplineField = createZSplines( currentPoints, referencePoints );
  // connect x and y splines
  mountZSplines( currentZSplineField );

  // CAN BE DEPRECATED
  // together with groupPointsSorted and SEARCH_PATH_HALF_WIDTH
  // maybe + others
  //   // create raw z splines for surface reconstruction - OLD VERSION
  //   DefoSplineField currentZSplineField = createZSplinesOld( currentPoints, referencePoints );
  //   // connect x and y splines - OLD VERSION
  //   mountZSplinesOld( currentZSplineField );
  
  // correct for global offsets
  removeGlobalOffset( currentZSplineField );

  // attach the spline field
  theSurface.setSplineField( currentZSplineField );
  theSurface.setPoints( referencePoints ); // ref points for the moment because createZSplines uses them
  theSurface.createPointFields(); // create matrix of points (internal)
  //  theSurface.dumpSplineField();

  return theSurface;

}



///
/// create z splines from difference in point positions
/// NEW VERSION based on indexed points
///
const DefoSplineField DefoRecoSurface::createZSplines( DefoPointCollection const& currentPoints, DefoPointCollection const& referencePoints ) {

  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::createZSplines] =3= Starting" << std::endl;

  DefoSplineField theOutput;


  // x,y correction factors: see Diss. S. Koenig, p. 100;
  const std::pair<double,double> correctionFactors = std::make_pair<double,double> (
    pitchX_ / focalLength_ * ( nominalGridDistance_ + nominalCameraDistance_ ) / 2. / nominalGridDistance_,
    pitchY_ / focalLength_ * ( nominalGridDistance_ + nominalCameraDistance_ ) / 2. / nominalGridDistance_
  );

  if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::createZSplines] =2= Will apply nominal correction factors: "
				   << correctionFactors.first << " , " << correctionFactors.second << std::endl;

  // determine index ranges in current points
  // (could also take ref points)
  std::pair<int,int> indexRangeX = std::make_pair<int,int>( 0, 0 );
  std::pair<int,int> indexRangeY = std::make_pair<int,int>( 0, 0 );

  // here we assume that there is at least one point right/left/above/below the blue one, resp., in the image;
  // otherwise the reconstruction will probably crash later
  for( DefoPointCollection::const_iterator it = currentPoints.begin(); it < currentPoints.end(); ++it ) {
    if( it->getIndex().first  < indexRangeX.first )  indexRangeX.first = it->getIndex().first;
    if( it->getIndex().first  > indexRangeX.second ) indexRangeX.second = it->getIndex().first;
    if( it->getIndex().second < indexRangeY.first )  indexRangeY.first  = it->getIndex().second;
    if( it->getIndex().second > indexRangeY.second ) indexRangeY.second = it->getIndex().second;
  }
  
  if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::createZSplines] =2= Found index range: x: " << indexRangeX.first
				   << " .. " << indexRangeX.second << " y: " << indexRangeY.first << " .. " << indexRangeY.second << std::endl;


  // we need the blue point (from *ref*) as geom. reference, it always has index 0,0
  std::pair<bool,DefoPointCollection::const_iterator> bluePointByIndex = findPointByIndex( referencePoints, std::make_pair<int,int>( 0, 0 ) );


  // now attach the points to the spline sets according to their indices
  std::pair<int,int> index = std::make_pair<int,int>( indexRangeX.first, indexRangeY.first );

  // first along-y
  for( ; index.first <= indexRangeX.second; ++index.first ) {

    // create a spline set along-y (a "column") .. 
    DefoSplineSetY aSplineSet;
    index.second = indexRangeY.first; // reset

    for( ; index.second <= indexRangeY.second; ++index.second ) {
      
      std::pair<bool,DefoPointCollection::const_iterator> currentPointByIndex   = findPointByIndex( currentPoints, index );
      std::pair<bool,DefoPointCollection::const_iterator> referencePointByIndex = findPointByIndex( referencePoints, index );
      
      // check if a point with that index exists in both images
      // (it should then have been a reflection from the same source)
      if( currentPointByIndex.first && referencePointByIndex.first ) {

	// this point is abstract and lives where the *ref* point is on the module
	// (make a copy)
	DefoPoint aPoint = DefoPoint( *(referencePointByIndex.second) );

	// the attached slope (= tan(alpha)) is derived from the difference in y position
	aPoint.setSlope( correctionFactors.second * ( *(currentPointByIndex.second) - *(referencePointByIndex.second) ).getY() ); // ##### check

	// convert from pixel units to real units on module

	// find the distance of this point along y wrt the center (blue point)
	//	const double thisPointOffset = ( *(referencePointByIndex.second) - *(bluePointByIndex.second) ).getY() * pitchY_ * nominalCameraDistance_ / focalLength_;

	// includes angular correction
	//	double cameraDistance = sqrt( pow( horizontalDistanceToSensor_ + thisPointOffset, 2. ) + pow( heightAboveSensor_, 2. ) );

	// 	aPoint.setXY( aPoint.getX() * pitchX_ * cameraDistance / focalLength_ , 
	// 		      aPoint.getY() * pitchY_ * cameraDistance / focalLength_ );
	aPoint.setXY( aPoint.getX() * pitchX_ * nominalCameraDistance_ / focalLength_ , // old version
		      aPoint.getY() * pitchY_ * nominalCameraDistance_ / focalLength_ );

	aSplineSet.addPoint( aPoint );

	if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::createZSplines] =3= Found shared point along-y with indices: "
					 << index.first << " , " << index.second << std::endl;

      }

      else {
	if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::createZSplines] =2= Non-shared point along-y in current image with indices: "
					 << index.first << " , " << index.second << std::endl;
      }

    }
    
    // check if there are any points attached to the set
    if( 0 == aSplineSet.getNPoints() ) continue;

    // do the fit
    aSplineSet.doFitZ();
    
    // attach to output field (as *second*!!)
    theOutput.second.push_back( aSplineSet );

  }




  // then along-x
  index = std::make_pair<int,int>( indexRangeX.first, indexRangeY.first );

  for( ; index.second <= indexRangeY.second; ++index.second ) {

    // create a spline set along-x (a "row") .. 
    DefoSplineSetX aSplineSet;
    index.first = indexRangeX.first; // reset

    for( ; index.first <= indexRangeX.second; ++index.first ) {
      
      std::pair<bool,DefoPointCollection::const_iterator> currentPointByIndex   = findPointByIndex( currentPoints, index );
      std::pair<bool,DefoPointCollection::const_iterator> referencePointByIndex = findPointByIndex( referencePoints, index );
      
      // check if a point with that index exists in both images
      // (it should then have been a reflection from the same source)
      if( currentPointByIndex.first && referencePointByIndex.first ) {

	// this point is abstract and lives where the ref point is on the module
	// (make a copy)
	DefoPoint aPoint = DefoPoint( *(referencePointByIndex.second) );

	// the attached slope (= tan(alpha)) is derived from the difference in y position
	aPoint.setSlope( correctionFactors.first * ( *(currentPointByIndex.second) - *(referencePointByIndex.second) ).getX() ); // ##### check
	// 	std::cout << "SLOPE-A-X: " << index.first << " " << index.second << " " << aPoint.getSlope()
	// 		  << " " << correctionFactors.first
	// 		  << " " << ( *(currentPointByIndex.second) - *(referencePointByIndex.second) ).getX()
	// 		  << " " << ( *(currentPointByIndex.second) - *(referencePointByIndex.second) ).getY()
	// 		  << std::endl; /////////////////////////////////
	
	// convert from pixel units to real units on module

	// find the distance along y of this point wrt the center (blue point)
	// const double thisPointOffset = ( *(referencePointByIndex.second) - *(bluePointByIndex.second) ).getY() * pitchY_ * nominalCameraDistance_ / focalLength_;
	//const double thisPointOffset = ( *(bluePointByIndex.second) - *(referencePointByIndex.second) ).getY() * pitchY_ * nominalCameraDistance_ / focalLength_;

	// includes angular correction
	//	double cameraDistance = sqrt( pow( horizontalDistanceToSensor_ + thisPointOffset, 2. ) + pow( heightAboveSensor_, 2. ) );

	// 	aPoint.setXY( aPoint.getX() * pitchX_ * cameraDistance / focalLength_ , 
	// 		      aPoint.getY() * pitchY_ * cameraDistance / focalLength_ );
	aPoint.setXY( aPoint.getX() * pitchX_ * nominalCameraDistance_ / focalLength_ , // old version
		      aPoint.getY() * pitchY_ * nominalCameraDistance_ / focalLength_ );

	aSplineSet.addPoint( aPoint );

	if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::createZSplines] =3= Found shared point along-x with indices: "
					 << index.first << " , " << index.second << std::endl;

      }

      else {
	if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::createZSplines] =2= non-shared point along-x in current image with indices: "
					 << index.first << " , " << index.second << std::endl;
      }

    }

    // check if there are any points attached to the set
    if( 0 == aSplineSet.getNPoints() ) continue;

    // do the fit
    aSplineSet.doFitZ();
    
    // attach to output field (as *second*!!)
    theOutput.first.push_back( aSplineSet );

  }


  // c'est tout
  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::createZSplines] =3= Done" << std::endl;
  return theOutput;

}



///
/// create z splines from difference of point positions
/// OLD symmetric version, to be deprecated
///
const DefoSplineField DefoRecoSurface::createZSplinesOld( DefoPointCollection const& currentPoints, DefoPointCollection const& referencePoints ) {

  DefoSplineField theOutput;

  // group & sort the points.
  // these are supposed to match 1<>1 in structure,
  // so no points must "vanish" from the DUT by deformation..
  const std::pair<std::vector<DefoPointCollection>,std::vector<DefoPointCollection> > currentGroups = groupPointsSorted( currentPoints );
  const std::pair<std::vector<DefoPointCollection>,std::vector<DefoPointCollection> > referenceGroups = groupPointsSorted( referencePoints );

  // check if we have the same number of rows & columns
  // in current image and reference image
  if( currentGroups.first.size() != referenceGroups.first.size()   ||
      currentGroups.second.size() != referenceGroups.second.size()    ) {
    std::cerr << " [DefoRecoSurface::createZSplinesOld] ** ERROR: Size mismatch in sorted point groups." << std::endl;
    throw;
  }

  // x,y correction factors: see Diss. S. Koenig, p. 100
  const std::pair<double,double> correctionFactors (
    pitchX_ / focalLength_ * ( nominalGridDistance_ + nominalCameraDistance_ ) / 2. / nominalGridDistance_,
    pitchY_ / focalLength_ * ( nominalGridDistance_ + nominalCameraDistance_ ) / 2. / nominalGridDistance_
  );

  // first along y = SAME-X = pointGroups.first
  std::vector<DefoPointCollection>::const_iterator currentCollIt, referenceCollIt;
  for( currentCollIt = currentGroups.first.begin(), referenceCollIt = referenceGroups.first.begin();
       currentCollIt < currentGroups.first.end(); ++currentCollIt, ++referenceCollIt ) {
    
    // create a spline set (a "column") .. 
    DefoSplineSetY aSplineSet;

    // .. and attach the points
    DefoPointCollection::const_iterator currentPointIt, referencePointIt;
    for( currentPointIt = currentCollIt->begin(), referencePointIt = referenceCollIt->begin();
	 currentPointIt < currentCollIt->end(); ++currentPointIt, ++referencePointIt ) {

      // this point is abstract and lives where the ref point is on the module
      // (make a copy)
      DefoPoint aPoint = DefoPoint( *referencePointIt );

      // the attached slope (= tan(alpha)) is derived from the difference in x position
      aPoint.setSlope( correctionFactors.first * (*currentPointIt - *referencePointIt).getY() );

      // convert from pixel units to real units on module
      aPoint.setXY( aPoint.getX() * pitchX_ * nominalCameraDistance_ / focalLength_ , 
 		    aPoint.getY() * pitchY_ * nominalCameraDistance_ / focalLength_ );

      aSplineSet.addPoint( aPoint );

    }

    // do the fit
    aSplineSet.doFitZ();

    // attach to output field (as *second*!!)
    theOutput.second.push_back( aSplineSet );

  }


  // then along x = SAME-Y = pointGroups.second
  for( currentCollIt = currentGroups.second.begin(), referenceCollIt = referenceGroups.second.begin();
       currentCollIt < currentGroups.second.end(); ++currentCollIt, ++referenceCollIt ) {
    
    // create a spline set (a "row") .. 
    DefoSplineSetX aSplineSet;
    
    // .. and attach the points
    DefoPointCollection::const_iterator currentPointIt, referencePointIt;
    for( currentPointIt = currentCollIt->begin(), referencePointIt = referenceCollIt->begin();
	 currentPointIt < currentCollIt->end(); ++currentPointIt, ++referencePointIt ) {

      // this point is abstract and lives where the ref point is
      // (make a copy)
      DefoPoint aPoint = DefoPoint( *referencePointIt );

      // the attached slope (= tan(alpha)) is derived from the difference in x position
      aPoint.setSlope( correctionFactors.first * (*currentPointIt - *referencePointIt).getX() );

      // convert from pixel units to real units
      aPoint.setXY( aPoint.getX() * pitchX_ * nominalCameraDistance_ / focalLength_ , 
 		    aPoint.getY() * pitchY_ * nominalCameraDistance_ / focalLength_   );

      aSplineSet.addPoint( aPoint );

    }

    // do the fit
    aSplineSet.doFitZ();

    // attach to output field (as *first*!!)
    theOutput.first.push_back( aSplineSet );
  }

  return theOutput;

}



///
///
///
void DefoRecoSurface::mountZSplines( DefoSplineField& splineField ) const {

  // the numbers of point rows/columns
  std::pair<unsigned int, unsigned int> nSplines
    = std::make_pair<unsigned int, unsigned int>( splineField.first.size(), splineField.second.size() );
  
  // this container stores a double per splineset;
  // used to sum up the relative heights of the splines from different mountings
  // for later averaging
  DefoSplineFieldOffsets theOffsets;
  theOffsets.first.resize( nSplines.first, 0. );
  theOffsets.second.resize( nSplines.second, 0. );

  // this container stores and int per splineset
  // used to properly calculate the average
  DefoSplineFieldCounters theCounters;
  theCounters.first.resize( nSplines.first, 0 );
  theCounters.second.resize( nSplines.second, 0 );


  // make a working copy for temporary values
  DefoSplineField cSplineField( splineField );


  // loop all "along-x" splinesets as height reference for the orthogonal "along-y" splines
  for( DefoSplineSetXCollection::const_iterator itX = cSplineField.first.begin(); itX < cSplineField.first.end(); ++itX ) {

    // loop all the points in this reference
    for( DefoPointCollection::const_iterator itPX = itX->getPoints().begin(); itPX < itX->getPoints().end(); ++itPX ) {
      
      // find the corresponding along-y spline which itX shares this point with
      for( DefoSplineSetYCollection::iterator itY = cSplineField.second.begin(); itY < cSplineField.second.end(); ++itY ) {
	for( DefoPointCollection::const_iterator itPY = itY->getPoints().begin(); itPY < itY->getPoints().end(); ++itPY ) {

	  if( itPX->getIndex() == itPY->getIndex() ) { // sharing?

	    // evaluate the height of the splineset at the corresponding point
	    const double heightOfSpline = itY->eval( itPY->getY() );
	    
	    // evaluate the height of the reference splineset at that point
	    const double heightOfReference = itX->eval( itPX->getX() );
	    
	    // determine the offset and apply
	    itY->offset( heightOfReference - heightOfSpline );

	  }

	} // itPY
      } // itY

    } // itPX
    
    // now fill the offset container with the results
    DefoSplineSetYCollection::const_iterator itY2 = cSplineField.second.begin();
    std::vector<double>::iterator itOY = theOffsets.second.begin();
    for( ; itY2 < cSplineField.second.end(); ++itY2, ++itOY ) {
      *itOY += itY2->eval( itY2->getPoints().front().getY() );
    }
    
  } // itX


  // now "backpropagate" this to the "along-x" splines
  // using the first "along-y" spline as reference

  // fresh working copy
  cSplineField = DefoSplineField( splineField );
  

  // loop all "along-y" splinesets as height reference for the orthogonal "along-x" splines
  for( DefoSplineSetYCollection::const_iterator itY = cSplineField.second.begin(); itY < cSplineField.second.end(); ++itY ) {

    // loop all the points in this reference
    for( DefoPointCollection::const_iterator itPY = itY->getPoints().begin(); itPY < itY->getPoints().end(); ++itPY ) {
      
      // find the corresponding along-y spline which itX shares this point with
      for( DefoSplineSetXCollection::iterator itX = cSplineField.first.begin(); itX < cSplineField.first.end(); ++itX ) {
	for( DefoPointCollection::const_iterator itPX = itX->getPoints().begin(); itPX < itX->getPoints().end(); ++itPX ) {

	  if( itPY->getIndex() == itPX->getIndex() ) { // sharing?

	    // evaluate the height of the splineset at the corresponding point
	    const double heightOfSpline = itX->eval( itPX->getX() );

	    // evaluate the height of the reference splineset at that point
	    const double heightOfReference = itY->eval( itPY->getY() );
	    
	    // determine the offset and apply
	    itX->offset( heightOfReference - heightOfSpline );

	  }

	} // itPX
      } // itX

    } // itPY

    // now fill the offset container with the results
    DefoSplineSetXCollection::const_iterator itX2 = cSplineField.first.begin();
    std::vector<double>::iterator itOX = theOffsets.first.begin();
    for( ; itX2 < cSplineField.first.end(); ++itX2, ++itOX ) {
      *itOX += itX2->eval( itX2->getPoints().front().getX() );
    }

  } // itY
  



  // average the offsets
  for( std::vector<double>::iterator itX = theOffsets.first.begin(); itX < theOffsets.first.end(); ++itX ) {
    *itX /= nSplines.first;
  }
  for( std::vector<double>::iterator itY = theOffsets.second.begin(); itY < theOffsets.second.end(); ++itY ) {
    *itY /= nSplines.second;
  }
  
  
  // apply the average to the original input spline field
  DefoSplineSetXCollection::iterator itX = splineField.first.begin();
  DefoSplineSetYCollection::iterator itY = splineField.second.begin();
  std::vector<double>::const_iterator itOX = theOffsets.first.begin();
  std::vector<double>::const_iterator itOY = theOffsets.second.begin();
  
  for( ; itX < splineField.first.end(); ++itX, ++itOX ) {
    itX->offset( *itOX - itX->eval( itX->getPoints().front().getX() ) );
  }
  
  for( ; itY < splineField.second.end(); ++itY, ++itOY ) {
    itY->offset( *itOY - itY->eval( itY->getPoints().front().getY() ) );
  }

}



///
/// combine z-splines along x and y
/// by mounting them on each other
/// OLD symmetric version, to be deprecated
///
void DefoRecoSurface::mountZSplinesOld( DefoSplineField& splineField ) const {

  // the numbers of point rows/columns
  std::pair<unsigned int, unsigned int> nSplines
    = std::make_pair<unsigned int, unsigned int>( splineField.first.size(), splineField.second.size() );

  // this container stores a double per splineset;
  // used to sum up the relative heights of the splines from different mountings
  // for later averaging
  DefoSplineFieldOffsets theOffsets;
  theOffsets.first.resize( nSplines.first, 0. );
  theOffsets.second.resize( nSplines.second, 0. );


  
  // loop all "along-x" splinesets as reference
  for( DefoSplineSetXCollection::const_iterator itX = splineField.first.begin(); itX < splineField.first.end(); ++itX ) {

    // make a working copy
    DefoSplineField cSplineField( splineField );
    
    // the current reference "along-x" spline
    // on which the along-y splines are mounted
    DefoSplineSetX const& theReferenceSplineSetX = *itX;
    
    // all the "along-y" splines
    // and the corresponding points that are shared with the reference "along-x" spline
    DefoPointCollection::const_iterator pIt = theReferenceSplineSetX.getPoints().begin();
    for( DefoSplineSetYCollection::iterator itY = cSplineField.second.begin();
	 itY < cSplineField.second.end(); ++itY, ++pIt ) {
      
      // evaluate the height of the splineset at the corresponding point
      const double heightOfSpline = itY->eval( pIt->getY() );

      // evaluate the height of the reference splineset at that point
      const double heightOfReference = theReferenceSplineSetX.eval( pIt->getX() );

      // determine the offset and apply
      itY->offset( heightOfReference - heightOfSpline );

    }

    // now fill the offset container with the results
    DefoSplineSetYCollection::const_iterator itY2 = cSplineField.second.begin();
    std::vector<double>::iterator itOY = theOffsets.second.begin();
    for( ; itY2 < cSplineField.second.end(); ++itY2, ++itOY ) {
      *itOY += itY2->eval( itY2->getPoints().front().getY() );
    }


  }

  

  // now "backpropagate" this to the "along-x" splines
  // using the first "along-y" spline as reference


  // loop all "along-y" splinesets as reference
  for( DefoSplineSetYCollection::const_iterator itY = splineField.second.begin(); itY < splineField.second.end(); ++itY ) {

    // make a working copy
    DefoSplineField cSplineField( splineField );

    // the reference "along-y" spline
    // on which the along-x splines are mounted
    DefoSplineSetY const& theReferenceSplineSetY = *itY;

    // all the "along-x" splines
    // and the corresponding points that are shared with the reference "along-y" spline
    DefoPointCollection::const_iterator pIt = theReferenceSplineSetY.getPoints().begin();
    for( DefoSplineSetXCollection::iterator itX = cSplineField.first.begin();
	 itX < cSplineField.first.end(); ++itX, ++pIt ) {
      
      // evaluate the height of the splineset at the first point
      const double heightOfSpline = itX->eval( pIt->getX() );
      
      // evaluate the height of the reference splineset at that point
      const double heightOfReference = theReferenceSplineSetY.eval( pIt->getY() );
      
      // determine the offset and apply
      itX->offset( heightOfReference - heightOfSpline );
      
    }
    
    
    // now fill the offset container with the results
    DefoSplineSetXCollection::const_iterator itX2 = cSplineField.first.begin();
    std::vector<double>::iterator itOX = theOffsets.first.begin();
    
    for( ; itX2 < cSplineField.first.end(); ++itX2, ++itOX ) {
      *itOX += itX2->eval( itX2->getPoints().front().getX() );
    }
    
  }



  // average the offsets
  for( std::vector<double>::iterator itX = theOffsets.first.begin(); itX < theOffsets.first.end(); ++itX ) {
    *itX /= nSplines.first;
  }
  for( std::vector<double>::iterator itY = theOffsets.second.begin(); itY < theOffsets.second.end(); ++itY ) {
    *itY /= nSplines.second;
  }


  // apply the average to the original input spline field
  DefoSplineSetXCollection::iterator itX = splineField.first.begin();
  DefoSplineSetYCollection::iterator itY = splineField.second.begin();
  std::vector<double>::const_iterator itOX = theOffsets.first.begin();
  std::vector<double>::const_iterator itOY = theOffsets.second.begin();

  for( ; itX < splineField.first.end(); ++itX, ++itOX ) {
    itX->offset( *itOX - itX->eval( itX->getPoints().front().getX() ) );
  }

  for( ; itY < splineField.second.end(); ++itY, ++itOY ) {
    itY->offset( *itOY - itY->eval( itY->getPoints().front().getY() ) );
  }
    
}



///
/// create x(y) and y(x) spline field from the points
/// this is for displaying the point grouping results only,
/// and has no effect on surface reco
///
const DefoSplineField DefoRecoSurface::createXYSplines( DefoPointCollection const& points ) {

  DefoSplineField theOutput;

  // group & sort the points
  const std::pair<std::vector<DefoPointCollection>,std::vector<DefoPointCollection> > pointGroups = groupPointsSorted( points );


  // first along y = SAME-X = pointGroups.first
  for( std::vector<DefoPointCollection>::const_iterator collIt = pointGroups.first.begin();
       collIt < pointGroups.first.end(); ++collIt ) {

    // create a spline set (a "row") .. 
    DefoSplineSetY aSplineSet;

    // .. and attach the points
    for( DefoPointCollection::const_iterator pointIt = collIt->begin(); pointIt < collIt->end(); ++pointIt ) {
      aSplineSet.addPoint( *pointIt );
    }

    // fit the splineset
    if( !aSplineSet.doFitXY() ) {
      std::cerr << " [DefoRecoSurface::createRawSplines] ** ERROR: failed to fit splineset along Y." << std::endl;
      throw;
    }

    // attach to output field (as *second*!!)
    theOutput.second.push_back( aSplineSet );

  }


  // then along x = SAME-Y = pointGroups.second
  for( std::vector<DefoPointCollection>::const_iterator collIt = pointGroups.second.begin();
       collIt < pointGroups.second.end(); ++collIt ) {

    // create a spline set (a "row") .. 
    DefoSplineSetX aSplineSet;

    // .. and attach the points
    for( DefoPointCollection::const_iterator pointIt = collIt->begin(); pointIt < collIt->end(); ++pointIt ) {
      aSplineSet.addPoint( *pointIt );
    }

    // fit the splineset
    if( !aSplineSet.doFitXY() ) {
      std::cerr << " [DefoRecoSurface::createRawSplines] ** ERROR: failed to fit splineset along X." << std::endl;
      throw;
    }

    // attach to output field (as *first*!!)
    theOutput.first.push_back( aSplineSet );

  }

  return theOutput;
    
}



///
/// group points in two sets of PointCollections (1 for x and 1 for y),
/// such that points in each collection are within a search path
/// according to x,y coordinates
/// the pair<> is for groups with "~SAME" x and y, resp.:
/// first = "~SAME" X, second = "~SAME" Y
///
const std::pair<std::vector<DefoPointCollection>,std::vector<DefoPointCollection> >
DefoRecoSurface::groupPointsSorted( DefoPointCollection const& thePoints ) {

  // group the points according to their x,y coordinates,
  // assuming they are lined up within a path of
  // +- searchPathHalfWidth_ pixels
  std::pair<std::vector<DefoPointCollection>, std::vector<DefoPointCollection> >  theOutput;

  // loop all points and group simultaneously
  for( DefoPointCollection::const_iterator it = thePoints.begin(); it < thePoints.end(); ++it ) {
    
    // add a point if it complies with the last point in a group
    // according to the searchPathHalfWidth_
    std::pair<bool,bool> isAssigned = std::make_pair<bool,bool>( false, false );

    // first: groups with "~SAME" X
    for( std::vector<DefoPointCollection>::iterator collIt = theOutput.first.begin();
	 collIt < theOutput.first.end(); ++collIt ) {

      if( fabs( it->getX() - collIt->back().getX() ) < searchPathHalfWidth_ ) { 
	collIt->push_back( *it );
	isAssigned.first = true;
      }
      
    }
    
    // if there is no match, create a new point group
    // (applies also for the very first point)
    if( !isAssigned.first ) {
      DefoPointCollection aPointCollection;
      aPointCollection.push_back( *it );
      theOutput.first.push_back( aPointCollection );
    }

    // then groups with "~SAME" Y
    for( std::vector<DefoPointCollection>::iterator collIt = theOutput.second.begin();
	 collIt < theOutput.second.end(); ++collIt ) {
      
      if( fabs( it->getY() - collIt->back().getY() ) < searchPathHalfWidth_ ) { 
	collIt->push_back( *it );
	isAssigned.second = true;
      }
      
    }
    
    // if there is no match, create a new point group
    // (applies also for the very first point)
    if( !isAssigned.second ) {
      DefoPointCollection aPointCollection;
      aPointCollection.push_back( *it );
      theOutput.second.push_back( aPointCollection );
    }
    
  } // point loop


  // sort the output

  // first: points with "~SAME" X are sorted according to Y
  for( std::vector<DefoPointCollection>::iterator collIt = theOutput.first.begin();
       collIt < theOutput.first.end(); ++collIt ) {
    std::sort( collIt->begin(), collIt->end(), DefoPointYPredicate );
  }

  // first: points with "~SAME" Y are sorted according to X
  for( std::vector<DefoPointCollection>::iterator collIt = theOutput.second.begin();
       collIt < theOutput.second.end(); ++collIt ) {
    std::sort( collIt->begin(), collIt->end(), DefoPointXPredicate );
  }

  // then sort the vectors of groups, the "~SAME X" groups according to their average X ..
  std::sort( theOutput.first.begin(), theOutput.first.end(), DefoPointCollectionAverageXPredicate );
  
  // .. and the "~SAME Y" groups according to their average Y
  std::sort( theOutput.second.begin(), theOutput.second.end(), DefoPointCollectionAverageYPredicate );

  return theOutput;

}



///
/// find an estimate for the spacing between the points
/// along x and y, resp.
/// from averaging
///
const std::pair<double,double> DefoRecoSurface::determineAverageSpacing( DefoPointCollection const& points ) const {

  // refill coordinates separately for x,y
  // into vectors container
  std::pair<std::vector<double>,std::vector<double> > coordinates;

  // fill the container
  for( DefoPointCollection::const_iterator it = points.begin(); it < points.end(); ++it ) {
    coordinates.first.push_back(  it->getX() );
    coordinates.second.push_back( it->getY() );
  }

  // sort the vectors
  sort( coordinates.first.begin(),  coordinates.first.end() );
  sort( coordinates.second.begin(), coordinates.second.end() );

    
  // walk along, look for spacings > spacingEstimate_
  // and average them
  // (assuming iterators are parallel)
  std::pair<double,double> sumOfSpacings;
  std::pair<unsigned int,unsigned int> nSpacings = std::make_pair<unsigned int,unsigned int>( 0, 0 );

  for( std::vector<double>::const_iterator it1 = coordinates.first.begin() + 1, it2 = coordinates.second.begin() + 1;
       it1 < coordinates.first.end(); ++it1, ++it2 ) {
    
    const std::pair<double,double> aSpacing = std::make_pair<double,double>( fabs( *it1 - *(it1 - 1) ), fabs( *it2 - *(it2 - 1) ) );
    if( aSpacing.first  > spacingEstimate_ ) { sumOfSpacings.first  += aSpacing.first;  ++nSpacings.first; }
    if( aSpacing.second > spacingEstimate_ ) { sumOfSpacings.second += aSpacing.second; ++nSpacings.second; }

  }

  if( !( nSpacings.first && nSpacings.second ) ) {
    std::cerr << " [DefoRecoSurface::determineAverageSpacing] ** ERROR: unable to determine spacing." << std::endl;
    throw;
  }

  // average
  return std::make_pair<double,double>( sumOfSpacings.first / nSpacings.first, sumOfSpacings.second / nSpacings.second );

}



///
/// determine the indices of the points with respect to the "blue point in the middle",
/// where the blue point has (0,0);
/// returns the abs index range along x,y, i.e. the maximum |index|
/// which is needed for display with QwtPlot3D
///
std::pair<unsigned int, unsigned int> DefoRecoSurface::indexPoints( DefoPointCollection& points ) const {

  // first, look for "blue points" and make sure there's only one
  unsigned int nBluePoints = 0;
  DefoPointCollection::iterator bluePointIt;

  for( DefoPointCollection::iterator pIt = points.begin(); pIt < points.end(); ++pIt ) {
    if( pIt->isBlue() ) {
      nBluePoints++; 
      bluePointIt = pIt;
    }
  }

  if( 1 != nBluePoints ) {
    std::cerr << " [DefoRecoSurface::indexPoints] ** ERROR: illegal number: " << nBluePoints << " of blue points found." << std::endl;
    throw;
  }

  else { // debug
    if( debugLevel_ >= 1 ) std::cout << " [DefoRecoSurface::indexPoints] =1= Found one blue point: x: " 
				    << bluePointIt->getX() << " y: " << bluePointIt->getY() << std::endl;
  }

  // determine the average spacing of the points in x,y in pixels,
  // this is needed as a clue for where approx. to look for a neighbor of a given point
  const std::pair<double,double> spacing = determineAverageSpacing( points );

  if( debugLevel_ >= 1 ) std::cout << " [DefoRecoSurface::indexPoints] =1= Determined average spacing: x: "
				   << spacing.first << " y: " << spacing.second << std::endl;

  // also need the full x,y ranges of the PointCollection wrt. to the blue one in the middle
  // (these pairs are min,max)
  std::pair<double,double> xRange = std::make_pair<double,double>( 0., 0. );
  std::pair<double,double> yRange = std::make_pair<double,double>( 0., 0. );

  // get min/max in a loop
  for( DefoPointCollection::const_iterator pIt = points.begin(); pIt < points.end(); ++pIt ) {
    if( pIt->getX() - bluePointIt->getX() > xRange.second ) xRange.second = pIt->getX() - bluePointIt->getX();
    if( pIt->getX() - bluePointIt->getX() < xRange.first )  xRange.first  = pIt->getX() - bluePointIt->getX();
    if( pIt->getY() - bluePointIt->getY() > yRange.second ) yRange.second = pIt->getY() - bluePointIt->getY();
    if( pIt->getY() - bluePointIt->getY() < yRange.first )  yRange.first  = pIt->getY() - bluePointIt->getY();
  }


  // debug
  if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::indexPoints] =2= Point range wrt. blue point is: x: "
				   << xRange.first << " .. " << xRange.second
				   << " y: " << yRange.first << " .. " << yRange.second << std::endl;



  
  // now start with the blue point;
  // estimate position of next one (with spacing) and look which point is closest to that;
  // if one is found, it gets the appropriate integer indices.
  // If no point is close enough, skip index, because there is obviously no matching point

  std::pair<int,int> index = std::make_pair<int,int>( 0, 0 );
  (*bluePointIt).setIndex( index ); // blue point is indexed <0,0> by definition






  // in the first pass (outer loop), we go along the positive y-direction in the middle column,
  // then from each of the points start branching along a (x-)row

  DefoPoint startPointAlongX = *bluePointIt;

  // go along y, starting from the middle, until we hit the range limits (+ some margin)
  while( startPointAlongX.getY() - bluePointIt->getY() < yRange.second + spacingEstimate_ ) {

    // the new starting point for this (x-)row in the middle (y-)column
    DefoPoint thisPoint = startPointAlongX;

    // reset x index as we're starting from the middle
    index.first = 0;

    // go along +x until we hit the range limits (+ some margin)
    while( thisPoint.getX() - startPointAlongX.getX() < xRange.second + spacingEstimate_ ) {
      
      // estimate the position of the next point along x using the spacing
      const DefoPoint nextEstimate = thisPoint + DefoPoint( spacing.first, 0. );

      // break the loop if we exceed the range
      // to avoid "not found" messages in debug output
      if( nextEstimate.getX() - startPointAlongX.getX() > xRange.second + spacingEstimate_ ) break;
      
      // look for next point around this estimate
      if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3=" 
				      << " Searching point along-x around estimate: x: "
				      << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
      const std::pair<bool,DefoPointCollection::iterator> nextPointC = findClosestPoint( nextEstimate, points );
	
      // index is incremented anyway,
      // even if no point is found at that position
      ++index.first;
      
      // found one?
      if( nextPointC.first ) {

	// need to make sure we don't catch the same point again
	// otherwise we get an infinite loop
	if( ( fabs( thisPoint.getX() - nextPointC.second->getX() ) < 0.1 ) &&
	    ( fabs( thisPoint.getY() - nextPointC.second->getY() ) < 0.1 )    ) {

	  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3=" 
					   << " break indexing (+y+x) because point: x: " << thisPoint.getX()
					   << " y: " << thisPoint.getY() << " already indexed: " 
					   << index.first - 1 << " " << index.second << std::endl;
	  break;
	}

	thisPoint = *(nextPointC.second); // set new current point
	nextPointC.second->setIndex( index ); // set the index of the point
	if( debugLevel_ > 2 ) std::cout << " [DefoRecoSurface::indexPoints] =3= Found next point along-x: x: "
					<< nextPointC.second->getX() << " y: " << nextPointC.second->getY() 
					<< " indices: " << nextPointC.second->getIndex().first << " " << nextPointC.second->getIndex().second
					<< std::endl;
      }
      
      // not found? then use estimate for the hop to the next position
      else {
	if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::indexPoints] =2=" 
					 << " no point along-x found around estimate: x: " 
					 << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
	thisPoint = nextEstimate;
      }
      
    } 

    
    // now the other direction: -x
    
    // reset the starting point for this (x-)row in the middle (y-)column
    thisPoint = startPointAlongX;

    // reset x index as we're starting from the middle
    index.first = 0;

    // go along -x until we hit the range limits (+ some margin)
    while( thisPoint.getX() - startPointAlongX.getX() > xRange.first - spacingEstimate_ ) {
      
      // estimate the position of the next point along x using the spacing
      const DefoPoint nextEstimate = thisPoint - DefoPoint( spacing.first, 0. );

      // break the loop if we exceed the range
      // to avoid "not found" messages in debug output
      if( nextEstimate.getX() - startPointAlongX.getX() < xRange.first - spacingEstimate_ ) break;
      
      // look for next point around this estimate
      if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3=" 
				      << " SEARCH POINT along-x around estimate: x: "
				      << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
      const std::pair<bool,DefoPointCollection::iterator> nextPointC = findClosestPoint( nextEstimate, points );
	
      // index is decremented anyway,
      // even if no point is found at that position
      --index.first;
      
      // found one?
      if( nextPointC.first ) {

	// need to make sure we don't catch the same point again
	// otherwise we get an infinite loop
	if( ( fabs( thisPoint.getX() - nextPointC.second->getX() ) < 0.1 ) &&
	    ( fabs( thisPoint.getY() - nextPointC.second->getY() ) < 0.1 )    ) {
	  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3=" 
					   << " break indexing (+y-x) because point: x: " << thisPoint.getX()
					   << " y: " << thisPoint.getY() << " already indexed: " 
					   << index.first - 1 << " " << index.second << std::endl;
	  break;
	}

	thisPoint = *(nextPointC.second); // set new current point
	nextPointC.second->setIndex( index ); // set the index of the point
	if( debugLevel_ > 2 ) std::cout << " [DefoRecoSurface::indexPoints] =3= FOUND NEXT point along-x: x: "
					<< nextPointC.second->getX() << " y: " << nextPointC.second->getY() 
					<< " indices: " << nextPointC.second->getIndex().first << " " << nextPointC.second->getIndex().second
					<< std::endl;
      }
      
      // not found? then use estimate for the hop to the next position
      else {
	if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::indexPoints] =2=" 
					 << " NO POINT along-x found around estimate: x: " 
					 << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
	thisPoint = nextEstimate;
      }
      
    } 




    // this (x-)row is done, increment y now and repeat;
    // estimate the position of the next point along y using the spacing
    const DefoPoint nextEstimate = startPointAlongX + DefoPoint( 0., spacing.first );

    // break the loop if we exceed the range
    // to avoid "not found" messages in debug output
    if( nextEstimate.getY() - bluePointIt->getY() > yRange.second + spacingEstimate_ ) break;

    // look for next point around this estimate
    if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3= SEARCH POINT along-y around estimate: x: " 
				     << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
    const std::pair<bool,DefoPointCollection::iterator> nextPointC = findClosestPoint( nextEstimate, points );

    // reset index. it is incremented anyway,
    // even if no point is found at that position
    index.second++;
    index.first = 0;

    // found one?
    if( nextPointC.first ) {

      // need to make sure we don't catch the same point again
      // otherwise we get an infinite loop
      if( ( fabs( startPointAlongX.getX() - nextPointC.second->getX() ) < 0.1 ) &&
	  ( fabs( startPointAlongX.getY() - nextPointC.second->getY() ) < 0.1 )    ) {

	if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3=" 
					 << " break indexing (y++) because point: x: " << thisPoint.getX()
					 << " y: " << thisPoint.getY() << " already indexed: " 
					 << index.first - 1 << " " << index.second << std::endl;
	break;
      }
	
      startPointAlongX = *(nextPointC.second); // set new current point
      nextPointC.second->setIndex( index ); // set the index of the point
      if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3= FOUND NEXT point along-y: x: "
				       << nextPointC.second->getX() << " y: " << nextPointC.second->getY() 
				       << " indices: " << nextPointC.second->getIndex().first << " " << nextPointC.second->getIndex().second
				       << std::endl;
    }
    
    // not found? then use estimate for the hop to the next position
    else {
      if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::indexPoints] =2=" 
				       << " NO POINT along-y found around estimate: x: " 
				       << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
      startPointAlongX = nextEstimate;
    }

  } // y loop





  // in the second pass (outer loop), we go along the negative y-direction in the middle column,
  // then from each of the points start branching along a (x-)row

  startPointAlongX = *bluePointIt;
  index = std::make_pair<int,int>( 0, 0 );

  // go along y, starting from the middle, until we hit the range limits (+ some margin)
  while( startPointAlongX.getY() - bluePointIt->getY() > yRange.first - spacingEstimate_ ) {

    // the new starting point for this (x-)row in the middle (y-)column
    DefoPoint thisPoint = startPointAlongX;

    // reset x index as we're starting from the middle
    index.first = 0;

    // go along +x until we hit the range limits (+ some margin)
    while( thisPoint.getX() - startPointAlongX.getX() < xRange.second + spacingEstimate_ ) {
      
      // estimate the position of the next point along x using the spacing
      const DefoPoint nextEstimate = thisPoint + DefoPoint( spacing.first, 0. );

      // break the loop if we exceed the range
      // to avoid "not found" messages in debug output
      if( nextEstimate.getX() - startPointAlongX.getX() > xRange.second + spacingEstimate_ ) break;
      
      // look for next point around this estimate
      if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3=" 
				      << " SEARCH POINT along-x around estimate: x: "
				      << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
      const std::pair<bool,DefoPointCollection::iterator> nextPointC = findClosestPoint( nextEstimate, points );
	
      // index is incremented anyway,
      // even if no point is found at that position
      ++index.first;
      
      // found one?
      if( nextPointC.first ) {

	// need to make sure we don't catch the same point again
	// otherwise we get an infinite loop
	if( ( fabs( thisPoint.getX() - nextPointC.second->getX() ) < 0.1 ) &&
	    ( fabs( thisPoint.getY() - nextPointC.second->getY() ) < 0.1 )    ) {

	  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3=" 
					   << " break indexing (-y+x) because point: x: " << thisPoint.getX()
					   << " y: " << thisPoint.getY() << " already indexed: " 
					   << index.first - 1 << " " << index.second << std::endl;
	  break;
	}

	thisPoint = *(nextPointC.second); // set new current point
	nextPointC.second->setIndex( index ); // set the index of the point
	if( debugLevel_ > 2 ) std::cout << " [DefoRecoSurface::indexPoints] =3= FOUND NEXT point along-x: x: "
					<< nextPointC.second->getX() << " y: " << nextPointC.second->getY() 
					<< " indices: " << nextPointC.second->getIndex().first << " " << nextPointC.second->getIndex().second
					<< std::endl;
      }
      
      // not found? then use estimate for the hop to the next position
      else {
	if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::indexPoints] =2=" 
					 << " NO POINT along-x found around estimate: x: " 
					 << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
	thisPoint = nextEstimate;
      }
      
    } 

    
    // now the other direction: -x
    
    // reset the starting point for this (x-)row in the middle (y-)column
    thisPoint = startPointAlongX;

    // reset x index as we're starting from the middle
    index.first = 0;

    // go along -x until we hit the range limits (+ some margin)
    while( thisPoint.getX() - startPointAlongX.getX() > xRange.first - spacingEstimate_ ) {
      
      // estimate the position of the next point along x using the spacing
      const DefoPoint nextEstimate = thisPoint - DefoPoint( spacing.first, 0. );

      // break the loop if we exceed the range
      // to avoid "not found" messages in debug output
      if( nextEstimate.getX() - startPointAlongX.getX() < xRange.first - spacingEstimate_ ) break;
      
      // look for next point around this estimate
      if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3=" 
				      << " SEARCH POINT along-x around estimate: x: "
				      << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
      const std::pair<bool,DefoPointCollection::iterator> nextPointC = findClosestPoint( nextEstimate, points );
	
      // index is decremented anyway,
      // even if no point is found at that position
      --index.first;
      
      // found one?
      if( nextPointC.first ) {

	// need to make sure we don't catch the same point again
	// otherwise we get an infinite loop
	if( ( fabs( thisPoint.getX() - nextPointC.second->getX() ) < 0.1 ) &&
	    ( fabs( thisPoint.getY() - nextPointC.second->getY() ) < 0.1 )    ) {

	  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3=" 
					   << " break indexing (-y-x) because point: x: " << thisPoint.getX()
					   << " y: " << thisPoint.getY() << " already indexed: " 
					   << index.first - 1 << " " << index.second << std::endl;
	  break;
	}

	thisPoint = *(nextPointC.second); // set new current point
	nextPointC.second->setIndex( index ); // set the index of the point
	if( debugLevel_ > 2 ) std::cout << " [DefoRecoSurface::indexPoints] =3= FOUND NEXT point along-x: x: "
					<< nextPointC.second->getX() << " y: " << nextPointC.second->getY() 
					<< " indices: " << nextPointC.second->getIndex().first << " " << nextPointC.second->getIndex().second
					<< std::endl;
      }
      
      // not found? then use estimate for the hop to the next position
      else {
	if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::indexPoints] =2=" 
					 << " NO POINT along-x found around estimate: x: " 
					 << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
	thisPoint = nextEstimate;
      }
      
    } 




    // this (x-)row is done, increment y now and repeat;
    // estimate the position of the next point along y using the spacing
    const DefoPoint nextEstimate = startPointAlongX - DefoPoint( 0., spacing.first );

    // break the loop if we exceed the range
    // to avoid "not found" messages in debug output
    if( nextEstimate.getY() - bluePointIt->getY() < yRange.first - spacingEstimate_ ) break;

    // look for next point around this estimate
    if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3= search point along-y around estimate: x: " 
				     << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
    const std::pair<bool,DefoPointCollection::iterator> nextPointC = findClosestPoint( nextEstimate, points );

    // reset index. it is incremented anyway,
    // even if no point is found at that position
    index.second--;
    index.first = 0;

    // found one?
    if( nextPointC.first ) {

      // need to make sure we don't catch the same point again
      // otherwise we get an infinite loop
      if( ( fabs( startPointAlongX.getX() - nextPointC.second->getX() ) < 0.1 ) &&
	  ( fabs( startPointAlongX.getY() - nextPointC.second->getY() ) < 0.1 )    ) {

	if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3=" 
					 << " break indexing (x++) because point: x: " << thisPoint.getX()
					 << " y: " << thisPoint.getY() << " already indexed: " 
					 << index.first - 1 << " " << index.second << std::endl;	
	break;
      }

      startPointAlongX = *(nextPointC.second); // set new current point
      nextPointC.second->setIndex( index ); // set the index of the point
      if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3= FOUND NEXT point along-y: x: "
				       << nextPointC.second->getX() << " y: " << nextPointC.second->getY() 
				       << " indices: " << nextPointC.second->getIndex().first << " " << nextPointC.second->getIndex().second
				       << std::endl;
    }
    
    // not found? then use estimate for the hop to the next position
    else {
      if( debugLevel_ >= 2 ) std::cout << " [DefoRecoSurface::indexPoints] =2=" 
				       << " no point along-y found around estimate: x: " 
				       << nextEstimate.getX() << " y: " << nextEstimate.getY() << std::endl;
      startPointAlongX = nextEstimate;
    }

  } // x loop



  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoSurface::indexPoints] =3= indexing complete." << std::endl;


  // finally, check if all points are indexed
  // and fill the output collection

  std::pair<unsigned int, unsigned int> outputRange( 0, 0 );

  for( DefoPointCollection::const_iterator it = points.begin(); it < points.end(); ++it ) {
    if( abs( it->getIndex().first  ) > (int)outputRange.first  ) outputRange.first  = abs( it->getIndex().first   );
    if( abs( it->getIndex().second ) > (int)outputRange.second ) outputRange.second = abs( it->getIndex().second  );

    if( !it->isIndexed() ) {
      std::cerr << " [DefoRecoSurface::indexPoints] ** WARNING: Point not indexed at posistion: x: " 
		<< it->getX() << " y: " << it->getY() << std::endl;
    }
  }

  return outputRange;

}



///
/// in the collection "points", find the one which is closest to "aPoint";
/// this closest point must not be further away from aPoint than (SPACING_ESTIMATE,SPACING_ESTIMATE),
/// otherwise return false.
///
std::pair<bool,DefoPointCollection::iterator> DefoRecoSurface::findClosestPoint( DefoPoint const& aPoint, DefoPointCollection& points ) const {
  
  DefoPoint ref( std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
  DefoPoint max( spacingEstimate_, spacingEstimate_ );
  DefoPointCollection::iterator result;

  // loop points
  for( DefoPointCollection::iterator it = points.begin(); it < points.end(); ++it ) {

    const DefoPoint difference = aPoint - *it;

    if( difference < ref ) {
      ref = difference;
      result = it;
    }
      
  }

  // point not too far away from requested position aPoint?
  bool isFound = false;
  if( *result - aPoint < max ) isFound = true;

  return std::make_pair<bool,DefoPointCollection::iterator>( isFound, result );

}
    


///
///
///  
const std::pair<bool,DefoPointCollection::const_iterator> 
DefoRecoSurface::findPointByIndex( DefoPointCollection const& points, std::pair<int,int> const& index ) const {

  DefoPointCollection::const_iterator it = points.begin();
  bool isFound = false;

  for( ; it < points.end(); ++it ) {
    if( index == it->getIndex() ) { isFound = true; break; }
  }

  return std::make_pair<bool,DefoPointCollection::const_iterator>( isFound, it );

}



///
/// determine and apply a common offset to all spline sets in the field
/// such that the lowermost point has height zero in the end
///
void DefoRecoSurface::removeGlobalOffset( DefoSplineField& splineField ) const {
  
  double minimalHeight = std::numeric_limits<double>::max();

  // first determine the height of the lowermost point
  for( DefoSplineSetXCollection::const_iterator itX = splineField.first.begin(); itX < splineField.first.end(); ++itX ) {
    for( DefoPointCollection::const_iterator itPX = itX->getPoints().begin(); itPX < itX->getPoints().end(); ++itPX ) {
      if( itX->eval( itPX->getX() ) < minimalHeight ) minimalHeight = itX->eval( itPX->getX() );
    }
  }

  // now adjust all the splinesets
  for( DefoSplineSetXCollection::iterator itX = splineField.first.begin(); itX < splineField.first.end(); ++itX ) {
    itX->offset( -1. * minimalHeight );
  }


  minimalHeight = std::numeric_limits<double>::max();
  
  // first determine the height of the lowermost point
  for( DefoSplineSetYCollection::const_iterator itY = splineField.second.begin(); itY < splineField.second.end(); ++itY ) {
    for( DefoPointCollection::const_iterator itPY = itY->getPoints().begin(); itPY < itY->getPoints().end(); ++itPY ) {
      if( itY->eval( itPY->getY() ) < minimalHeight ) minimalHeight = itY->eval( itPY->getY() );
    }
  }


  for( DefoSplineSetYCollection::iterator itY = splineField.second.begin(); itY < splineField.second.end(); ++itY ) {
    itY->offset( -1. * minimalHeight );
  }
  

}



///
/// remove a global tilt (along x or y) of the spline field
///
void DefoRecoSurface::removeTilt( DefoSplineField& ) const {
}
