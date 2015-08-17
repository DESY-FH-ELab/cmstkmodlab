#include "ApplicationConfig.h"

#include "nqlogger.h"

#include "nvector3D.h"
#include "npoint3D.h"
#include "ndirection3D.h"
#include "nline3D.h"
#include "nplane3D.h"
#include "nvector2D.h"

#include "DefoRecoSurface.h"

///
///
///
DefoRecoSurface::DefoRecoSurface(QObject *parent)
  :QObject(parent)
{
  // read parameters
  spacingEstimate_ = ApplicationConfig::instance()->getValue<int>( "SPACING_ESTIMATE" );
  searchPathHalfWidth_ = ApplicationConfig::instance()->getValue<int>( "SEARCH_PATH_HALF_WIDTH" );
  nominalGridDistance_ = ApplicationConfig::instance()->getValue<double>( "NOMINAL_GRID_DISTANCE" );
  nominalCameraDistance_ = ApplicationConfig::instance()->getValue<double>( "NOMINAL_CAMERA_DISTANCE" );
  nominalViewingAngle_ = ApplicationConfig::instance()->getValue<double>( "NOMINAL_VIEWING_ANGLE" );
  pitchX_= ApplicationConfig::instance()->getValue<double>( "PIXEL_PITCH_X" );
  pitchY_= ApplicationConfig::instance()->getValue<double>( "PIXEL_PITCH_Y" );
  focalLength_= ApplicationConfig::instance()->getValue<double>( "LENS_FOCAL_LENGTH" );
  debugLevel_ = ApplicationConfig::instance()->getValue<unsigned int>( "DEBUG_LEVEL" );

  calibX_ = ApplicationConfig::instance()->getValue<double>("CALIBX", 1.0);
  calibY_ = ApplicationConfig::instance()->getValue<double>("CALIBY", 1.0);
  calibZx_ = ApplicationConfig::instance()->getValue<double>("CALIBZX", 1.0);
  calibZy_ = ApplicationConfig::instance()->getValue<double>("CALIBZY", 1.0);

  angle1_ = ApplicationConfig::instance()->getValue<double>( "ANGLE1" );
  angle2_ = ApplicationConfig::instance()->getValue<double>( "ANGLE2" );
  angle3_ = ApplicationConfig::instance()->getValue<double>( "ANGLE3" );
  distance_ = ApplicationConfig::instance()->getValue<double>( "DISTANCE" );
  height1_ = ApplicationConfig::instance()->getValue<double>( "HEIGHT1" );
  height2_ = ApplicationConfig::instance()->getValue<double>( "HEIGHT2" );

  // to be called after cfg reading
  calculateHelpers();
}

///
/// calculate some helper variables
///
void DefoRecoSurface::calculateHelpers( void )
{
  NQLog("DefoRecoSurface::calculateHelpers()", NQLog::Message) << "start";

  heightAboveSensor_ = nominalCameraDistance_ * std::sin(nominalViewingAngle_);
  NQLog("DefoRecoSurface", NQLog::Message)
      << "nominalCameraDistance [mm] = " << nominalCameraDistance_;
  
  if (tan( nominalViewingAngle_) != 0.) {
    horizontalDistanceToSensor_ = heightAboveSensor_ / tan( nominalViewingAngle_ );
    NQLog("DefoRecoSurface", NQLog::Message)
        << "horizontalDistanceToSensor [mm] = " << horizontalDistanceToSensor_;
  } else {
    NQLogFatal("DefoRecoSurface")
        << "tan(delta) is zero, no chance for proper reconstruction. "
        << "Check parameters in configuration file. Abort.";
    throw;
  }

  NQLog("DefoRecoSurface::calculateHelpers()", NQLog::Message) << "end";
}

///
/// perform the reconstruction based on two point collections:
/// a) reconstructed points of current image
/// b) reconstructed points of reference image
///
const DefoSurface DefoRecoSurface::reconstruct(DefoPointCollection& currentPoints,
                                               DefoPointCollection& referencePoints)
{
  DefoSurface theSurface;

  // calibrate XY coordinates of currentPoints
  calibrateXYPoints(currentPoints);
  emit incrementRecoProgress();

  // calibrate XY coordinates of referencePoints
  calibrateXYPoints(referencePoints);
  emit incrementRecoProgress();

  // create raw z splines for surface reconstruction
  DefoSplineField currentZSplineField = createZSplines( currentPoints, referencePoints );
  emit incrementRecoProgress();

  // connect x and y splines
  mountZSplines( currentZSplineField );
  emit incrementRecoProgress();

  // correct for global offsets
  removeGlobalOffset( currentZSplineField );
  emit incrementRecoProgress();

  // attach the spline field
  theSurface.setSplineField( currentZSplineField );
  emit incrementRecoProgress();

  theSurface.setPoints( referencePoints ); // ref points for the moment because createZSplines uses them
  emit incrementRecoProgress();

  theSurface.createPointFields(); // create matrix of points (internal)
  emit incrementRecoProgress();

  theSurface.makeSummary();

  theSurface.calibrateZ(calibZx_, calibZy_);

  return theSurface;
}

void DefoRecoSurface::calibrateXYPoints(DefoPointCollection & points)
{
  double f = focalLength_;
  double gamma = imageScale(f);
  double imageDistance = f * (gamma + 1.0);
  double objectDistance = imageDistance / gamma;

  NVector3D height1(0., 0., height1_);
  NVector3D height2(0., 0., height2_);
  NVector3D distance(0., -1.0*distance_, 0.);

  double a1 = angle1_ * M_PI / 180.;
  double a2 = angle2_ * M_PI / 180.;
  double a3 = angle3_ * M_PI / 180.;

  distance.rotateX(a2);

  NPoint3D cameraPoint(0., 0., 0.);
  cameraPoint.move(height1);
  cameraPoint.move(distance);

  NPoint3D objectPoint(0., 0., 0.);
  objectPoint.move(height2);
  NDirection3D objectNormal(0., 0., 1.);
  NPlane3D objectPlane(objectPoint, objectNormal);

  NDirection3D centerRayDirection(0., 0., -1.);
  centerRayDirection.rotateX(a2 + a3);

  NLine3D centerRay(cameraPoint, centerRayDirection);
  centerRay.intersection(objectPlane, objectPoint);

  NVector3D imageDistanceVector(objectPoint, cameraPoint);
  imageDistanceVector *= objectDistance / imageDistanceVector.length();

  NPoint3D imagePoint(objectPoint);
  imagePoint.move(imageDistanceVector);

  NPoint3D gridPoint(0., 0., 0.);
  gridPoint.move(height1);

  NDirection3D gridNormal(0., 0., -1.);
  gridNormal.rotateX(-a1);

  NPlane3D gridPlane(gridPoint, gridNormal);

  NPoint3D objectIntersection;
  NPoint3D gridIntersection;

  /*
  std::cout << imageSize_.first << std::endl;
  std::cout << imageSize_.second << std::endl;
  */

  for( DefoPointCollection::iterator it = points.begin();
      it != points.end();
      ++it ) {

    DefoPoint& aPoint = *it;

    NDirection3D imageRayDirection((aPoint.getX() - 0.5 * imageSize_.first) * pitchX_,
                                   (aPoint.getY() - 0.5 * imageSize_.second) * pitchY_,
                                   imageDistance);
    imageRayDirection.rotateX(a2 + a3);
    NLine3D imageRay(imagePoint, imageRayDirection);
    imageRay.intersection(objectPlane, objectIntersection);

    NVector3D imageDistance(objectIntersection, imagePoint);

    NDirection3D gridRayDirection(imageRayDirection);
    gridRayDirection.rotateZ(M_PI);
    NLine3D gridRay(objectIntersection, gridRayDirection);
    gridRay.intersection(gridPlane, gridIntersection);

    NVector3D gridDistance(objectIntersection, gridIntersection);

    double x = -1.0 * objectIntersection.x() * calibX_;
    double y =  1.0 * objectIntersection.y() * calibY_;

    aPoint.setCalibratedPosition(x, y);

    NVector2D imageDistanceX(imageDistance.x(), imageDistance.z());
    NVector2D gridDistanceX(gridDistance.x(), gridDistance.z());

    aPoint.setImageDistanceX(imageDistanceX.length());
    aPoint.setGridDistanceX(gridDistanceX.length());

    NVector2D imageDistanceY(imageDistance.y(), imageDistance.z());
    NVector2D gridDistanceY(gridDistance.y(), gridDistance.z());

    aPoint.setImageDistanceY(imageDistanceY.length());
    aPoint.setGridDistanceY(gridDistanceY.length());

    /*
    std::cout << "("
	      << aPoint.getIndex().first << ", "
	      << aPoint.getIndex().second << ") -> ("
	      << aPoint.getCalibratedX() << ", "
	      << aPoint.getCalibratedY() << ") -> ("
	      << aPoint.getImageDistance() << ", "
	      << aPoint.getGridDistance() << ") -> "
              << (aPoint.getGridDistance()+aPoint.getImageDistance())/(2.0*aPoint.getGridDistance())
	      << std::endl;
	*/
  }
}

///
/// create z splines from difference in point positions
/// NEW VERSION based on indexed points
///
const DefoSplineField DefoRecoSurface::createZSplines(DefoPointCollection const& currentPoints,
                                                      DefoPointCollection const& referencePoints)
{
  NQLog("DefoRecoSurface", NQLog::Message) << "createZSplines starting";

  DefoSplineField theOutput;

  /*
  // x,y correction factors: see Diss. S. Koenig, p. 100;
  const std::pair<double,double> correctionFactors = std::pair<double,double> (
    pitchX_ / focalLength_ * ( nominalGridDistance_ + nominalCameraDistance_ ) / 2. / nominalGridDistance_,
    pitchY_ / focalLength_ * ( nominalGridDistance_ + nominalCameraDistance_ ) / 2. / nominalGridDistance_
  );

  NQLog("DefoRecoSurface", NQLog::Message) << "nominal correction factors: "
      << correctionFactors.first << " , " << correctionFactors.second;
  */

  NQLog("DefoRecoSurface", NQLog::Message) << "image size: ("
      << imageSize_.first << ", " << imageSize_.second << ")";

  // determine index ranges in current points
  // (could also take ref points)
  std::pair<int,int> indexRangeX = std::pair<int,int>( 0, 0 );
  std::pair<int,int> indexRangeY = std::pair<int,int>( 0, 0 );

  // here we assume that there is at least one point right/left/above/below the blue one, resp., in the image;
  // otherwise the reconstruction will probably crash later
  for( DefoPointCollection::const_iterator it = currentPoints.begin(); it < currentPoints.end(); ++it ) {
    if( it->getIndex().first  < indexRangeX.first )  indexRangeX.first = it->getIndex().first;
    if( it->getIndex().first  > indexRangeX.second ) indexRangeX.second = it->getIndex().first;
    if( it->getIndex().second < indexRangeY.first )  indexRangeY.first  = it->getIndex().second;
    if( it->getIndex().second > indexRangeY.second ) indexRangeY.second = it->getIndex().second;
    /*
    indexRangeX.first = std::min(it->getIndex().first, indexRangeX.first);
    indexRangeX.second = std::max(it->getIndex().first, indexRangeX.second);
    indexRangeY.first = std::min(it->getIndex().first, indexRangeY.first);
    indexRangeY.second = std::max(it->getIndex().first, indexRangeY.second);
     */
  }
  
  /*
  std::pair<int,int> indexRangeXref = std::pair<int,int>( 0, 0 );
  std::pair<int,int> indexRangeYref = std::pair<int,int>( 0, 0 );
  for( DefoPointCollection::const_iterator it = referencePoints.begin(); it < referencePoints.end(); ++it ) {
    indexRangeXref.first = std::min(it->getIndex().first, indexRangeXref.first);
    indexRangeXref.second = std::max(it->getIndex().first, indexRangeXref.second);
    indexRangeYref.first = std::min(it->getIndex().first, indexRangeYref.first);
    indexRangeYref.second = std::max(it->getIndex().first, indexRangeYref.second);
  }
  indexRangeX.first = std::max(indexRangeXref.first, indexRangeX.first);
  indexRangeX.second = std::min(indexRangeXref.second, indexRangeX.second);
  indexRangeY.first = std::max(indexRangeYref.first, indexRangeY.first);
  indexRangeY.second = std::min(indexRangeYref.second, indexRangeY.second);
   */

  // we need the blue point (from *ref*) as geom. reference, it always has index 0,0
  std::pair<bool,DefoPointCollection::const_iterator> bluePointByIndex =
      findPointByIndex(referencePoints, std::pair<int,int>( 0, 0 ));

  // now attach the points to the spline sets according to their indices
  std::pair<int,int> index = std::pair<int,int>( indexRangeX.first, indexRangeY.first );

  // first along-y
  for( ; index.first <= indexRangeX.second; ++index.first ) {

    // create a spline set along-y (a "column") .. 
    DefoSplineSetY aSplineSet;
    index.second = indexRangeY.first; // reset

    for( ; index.second <= indexRangeY.second; ++index.second ) {
      
      std::pair<bool,DefoPointCollection::const_iterator> currentPointByIndex =
          findPointByIndex(currentPoints, index);

      std::pair<bool,DefoPointCollection::const_iterator> referencePointByIndex =
          findPointByIndex(referencePoints, index);
      
      // check if a point with that index exists in both images
      // (it should then have been a reflection from the same source)
      if (currentPointByIndex.first && referencePointByIndex.first) {

        // this point is abstract and lives where the *ref* point is on the module
        // (make a copy)
        DefoPoint aPoint = DefoPoint(*(referencePointByIndex.second));

        // the attached slope (= tan(alpha)) is derived from the difference in y position
        double currentY = (*(currentPointByIndex.second)).getCalibratedY();
        double referenceY = (*(referencePointByIndex.second)).getCalibratedY();
        double dY = 1.0*(currentY - referenceY);

        aPoint.setSlope( aPoint.getCorrectionFactor(DefoPoint::Y) * dY);

        aSplineSet.addPoint( aPoint );

        //NQLog("DefoRecoSurface", NQLog::Spam) << "found shared point along y with indices: "
        //    << index.first << " , " << index.second;

      } else {
        NQLog("DefoRecoSurface", NQLog::Warning) << "Non-shared point along y in current image with indices: "
            << index.first << " , " << index.second;
      }
    }
    
    // check if there are enough points attached to the set (min 2)
    if( 2 > aSplineSet.getNPoints() ) continue;

    // do the fit
    aSplineSet.doFitZ();
    
    // attach to output field (as *second*!!)
    theOutput.second.push_back(aSplineSet);
  }

  // then along-x
  index = std::pair<int,int>( indexRangeX.first, indexRangeY.first );

  for( ; index.second <= indexRangeY.second; ++index.second ) {

    // create a spline set along-x (a "row") .. 
    DefoSplineSetX aSplineSet;
    index.first = indexRangeX.first; // reset

    for( ; index.first <= indexRangeX.second; ++index.first ) {
      
      std::pair<bool,DefoPointCollection::const_iterator> currentPointByIndex =
          findPointByIndex( currentPoints, index );

      std::pair<bool,DefoPointCollection::const_iterator> referencePointByIndex =
          findPointByIndex( referencePoints, index );
      
      // check if a point with that index exists in both images
      // (it should then have been a reflection from the same source)
      if (currentPointByIndex.first && referencePointByIndex.first) {

        // this point is abstract and lives where the ref point is on the module
        // (make a copy)
        DefoPoint aPoint = DefoPoint( *(referencePointByIndex.second) );
        // convert from pixel units to real units on module

        // the attached slope (= tan(alpha)) is derived from the difference in x position
        double currentX = (*(currentPointByIndex.second)).getCalibratedX();
        double referenceX = (*(referencePointByIndex.second)).getCalibratedX();
        double dX = 1.0*(currentX - referenceX);

        aPoint.setSlope( aPoint.getCorrectionFactor(DefoPoint::Y) * dX);

        aSplineSet.addPoint( aPoint );

        //NQLog("DefoRecoSurface", NQLog::Spam) << "found shared point along x with indices: "
        //    << index.first << " , " << index.second;

      } else {
        NQLog("DefoRecoSurface", NQLog::Warning) << "Non-shared point along x in current image with indices: "
            << index.first << " , " << index.second;
      }
    }

    // check if there are enought points attached to the set (min 2)
    if( 2 > aSplineSet.getNPoints() ) continue;

    // do the fit
    aSplineSet.doFitZ();
    
    // attach to output field (as *second*!!)
    theOutput.first.push_back(aSplineSet);
  }

  // c'est tout
  NQLog("DefoRecoSurface", NQLog::Message) << "createZSplines done";

  return theOutput;
}

///
///
///
void DefoRecoSurface::mountZSplines( DefoSplineField& splineField ) const {

  // the numbers of point rows/columns
  std::pair<unsigned int, unsigned int> nSplines
    = std::pair<unsigned int, unsigned int>( splineField.first.size(), splineField.second.size() );
  
  // this container stores a double per splineset;
  // used to sum up the relative heights of the splines from different mountings
  // for later averaging
  DefoSplineFieldOffsets theOffsets;
  theOffsets.first.resize( nSplines.first, 0. );
  theOffsets.second.resize( nSplines.second, 0. );

  // this container stores and int per splineset, counting the mounting processes,
  // used to properly calculate the average
  DefoSplineFieldCounters theCounters;
  theCounters.first.resize( nSplines.first, 0 );
  theCounters.second.resize( nSplines.second, 0 );


  // make a working copy for temporary values
  DefoSplineField cSplineField( splineField );


  // loop all "along-x" splinesets as height reference for the orthogonal "along-y" splines
  for( DefoSplineSetXCollection::const_iterator itX = cSplineField.first.begin();
       itX < cSplineField.first.end();
       ++itX ) {

    // loop all the points in this reference
    for( DefoPointCollection::const_iterator itPX = itX->getPoints().begin();
         itPX < itX->getPoints().end();
         ++itPX ) {
      
      // find the corresponding along-y spline which itX shares this point with
      for( DefoSplineSetYCollection::iterator itY = cSplineField.second.begin();
           itY < cSplineField.second.end();
           ++itY ) {

        for( DefoPointCollection::const_iterator itPY = itY->getPoints().begin();
             itPY < itY->getPoints().end();
             ++itPY ) {

          if( itPX->getIndex() == itPY->getIndex() ) { // sharing?

            // evaluate the height of the splineset at the corresponding point
            const double heightOfSpline = itY->eval( itPY->getCalibratedY() );

            // evaluate the height of the reference splineset at that point
            const double heightOfReference = itX->eval( itPX->getCalibratedX() );

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
      *itOY += itY2->eval( itY2->getPoints().front().getCalibratedY() );
    }
    
  } // itX

  // now "backpropagate" this to the "along-x" splines
  // using the first "along-y" spline as reference

  // fresh working copy
  cSplineField = DefoSplineField( splineField );
  
  // loop all "along-y" splinesets as height reference for the orthogonal "along-x" splines
  for( DefoSplineSetYCollection::const_iterator itY = cSplineField.second.begin();
      itY < cSplineField.second.end();
      ++itY ) {

    // loop all the points in this reference
    for( DefoPointCollection::const_iterator itPY = itY->getPoints().begin();
        itPY < itY->getPoints().end();
        ++itPY ) {
      
      // find the corresponding along-y spline which itX shares this point with
      for( DefoSplineSetXCollection::iterator itX = cSplineField.first.begin();
          itX < cSplineField.first.end();
          ++itX ) {

        for( DefoPointCollection::const_iterator itPX = itX->getPoints().begin();
             itPX < itX->getPoints().end();
             ++itPX ) {

          if( itPY->getIndex() == itPX->getIndex() ) { // sharing?

            // evaluate the height of the splineset at the corresponding point
            const double heightOfSpline = itX->eval( itPX->getCalibratedX() );

            // evaluate the height of the reference splineset at that point
            const double heightOfReference = itY->eval( itPY->getCalibratedY() );

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
      *itOX += itX2->eval( itX2->getPoints().front().getCalibratedX() );
    }
  } // itY
  
  // average the offsets
  for( std::vector<double>::iterator itX = theOffsets.first.begin();
       itX < theOffsets.first.end();
       ++itX ) {
    *itX /= nSplines.first;
  }
  for( std::vector<double>::iterator itY = theOffsets.second.begin();
       itY < theOffsets.second.end();
       ++itY ) {
    *itY /= nSplines.second;
  }
  
  // apply the average to the original input spline field
  DefoSplineSetXCollection::iterator itX = splineField.first.begin();
  DefoSplineSetYCollection::iterator itY = splineField.second.begin();
  std::vector<double>::const_iterator itOX = theOffsets.first.begin();
  std::vector<double>::const_iterator itOY = theOffsets.second.begin();
  
  for( ; itX < splineField.first.end(); ++itX, ++itOX ) {
    itX->offset( *itOX - itX->eval( itX->getPoints().front().getCalibratedX() ) );
  }
  
  for( ; itY < splineField.second.end(); ++itY, ++itOY ) {
    itY->offset( *itOY - itY->eval( itY->getPoints().front().getCalibratedY() ) );
  }
}

const std::pair<bool,DefoPointCollection::const_iterator> 
DefoRecoSurface::findPointByIndex(DefoPointCollection const& points,
                                  std::pair<int,int> const& index ) const
{
  DefoPointCollection::const_iterator it = points.begin();
  bool isFound = false;

  for( ; it < points.end(); ++it) {
    if (index == it->getIndex()) {
      isFound = true;
      break;
    }
  }

  return std::pair<bool,DefoPointCollection::const_iterator>(isFound, it);
}

///
/// determine and apply a common offset to all spline sets in the field
/// such that the lowermost point has height zero in the end
///
void DefoRecoSurface::removeGlobalOffset( DefoSplineField& splineField ) const
{
  double minimalHeight = std::numeric_limits<double>::max();

  // first determine the height of the lowermost point
  for (DefoSplineSetXCollection::const_iterator itX = splineField.first.begin();
       itX < splineField.first.end();
       ++itX ) {
    for (DefoPointCollection::const_iterator itPX = itX->getPoints().begin();
         itPX < itX->getPoints().end();
         ++itPX ) {
      if (itX->eval(itPX->getCalibratedX()) < minimalHeight) minimalHeight = itX->eval(itPX->getCalibratedX());
    }
  }

  // now adjust all the splinesets
  for (DefoSplineSetXCollection::iterator itX = splineField.first.begin();
       itX < splineField.first.end();
       ++itX) {
    itX->offset(-1. * minimalHeight);
  }

  minimalHeight = std::numeric_limits<double>::max();
  
  // first determine the height of the lowermost point
  for (DefoSplineSetYCollection::const_iterator itY = splineField.second.begin();
       itY < splineField.second.end();
       ++itY) {
    for (DefoPointCollection::const_iterator itPY = itY->getPoints().begin();
         itPY < itY->getPoints().end();
         ++itPY) {
      if (itY->eval(itPY->getCalibratedY()) < minimalHeight) minimalHeight = itY->eval(itPY->getCalibratedY());
    }
  }

  for (DefoSplineSetYCollection::iterator itY = splineField.second.begin();
       itY < splineField.second.end();
       ++itY) {
    itY->offset(-1. * minimalHeight);
  }
}

///
/// remove a global tilt (along x or y) of the spline field
///
void DefoRecoSurface::removeTilt( DefoSplineField& ) const
{

}

void DefoRecoSurface::setLensParameters(double p0, double p1, double p2, double p3)
{
  lensP0_ = p0;
  lensP1_ = p1;
  lensP2_ = p2;
  lensP3_ = p3;
}

double DefoRecoSurface::imageScale(double focalLength) const
{
  /*
  // zoom lens
  double p0 = -0.0240888;
  double p1 = 0.00148648;
  double p2 = -6.91494e-06;
  */

  // 50 mm lens
  double p0 = 0.0366916;
  double p1 = 0.0;
  double p2 = 0.0;
  
  return p0 + p1 * focalLength + p2 * focalLength * focalLength;
}
