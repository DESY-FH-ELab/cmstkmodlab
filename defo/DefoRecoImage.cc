
#include "DefoRecoImage.h"

///
/// image reconstructor class:
/// identify dots, return reconstructed center points
///
DefoRecoImage::DefoRecoImage() {

  DefoConfigReader cfgReader( "defo.cfg" );

  step1Threshold_ = cfgReader.getValue<int>( "STEP1_THRESHOLD" );
  step2Threshold_ = cfgReader.getValue<int>( "STEP2_THRESHOLD" );
  step3Threshold_ = cfgReader.getValue<int>( "STEP3_THRESHOLD" );
  halfSquareWidth_ = cfgReader.getValue<int>( "HALF_SQUARE_WIDTH" );
  blueishnessThreshold_ = cfgReader.getValue<double>( "BLUEISHNESS_THRESHOLD" );
  debugLevel_ = cfgReader.getValue<unsigned int>( "DEBUG_LEVEL" );

}


///
/// point finding & reconstruction in a raw image;
/// requires:
///  1. the image (DefoRawImage)
///  2. a DefoArea, i.e. a selection frame in pixels inside the image;
///     will only search for points within this area
/// returns: 
///  1. vector of DefoPoints, one for each dot reconstructed in theImage
///  2. a copy of the original image with square & cross showing reco results
///
std::pair<DefoPointCollection,DefoRawImage> DefoRecoImage::reconstruct( DefoRawImage& theImage, DefoArea& theArea ) {

  DefoRawImage copyOfTheRawImage( theImage );
  DefoPointCollection outputCollection;

  std::pair<DefoPointCollection,DefoRawImage> theOutput
    = std::make_pair<DefoPointCollection,DefoRawImage>( outputCollection, copyOfTheRawImage );

  std::pair<double,double> imageWidth( theImage.getImage().width(), theImage.getImage().height() );

  if( debugLevel_ >= 2 ) std::cout << " [DefoRecoImage::reconstruct] =2= Image has: "
				   << imageWidth.first << " x " << imageWidth.second << " pixels" << std::endl;
  
  // clear the areas
  forbiddenAreas_.makeEmpty();


  for( int xIt = theArea.getRectangle().x(); xIt < theArea.getRectangle().x() + theArea.getRectangle().width(); ++xIt ) {
    for( int yIt = theArea.getRectangle().y(); yIt < theArea.getRectangle().y() + theArea.getRectangle().height(); ++yIt ) {

      // do not consider areas around points already picked up;
      // at this stage we need to check in order to avoid too many L1 seeds
      if( forbiddenAreas_.isInside( DefoPoint( xIt, yIt ) ) ) continue;
      
      // check if this pixel has a grayscale adc value above step1 threshold
      if( qGray( theImage.getImage().pixel( xIt, yIt ) ) > step1Threshold_ ) {
      
	// have an initial seed, check average amplitude
	// of some more pixels ahead
	double theProbe = 0.;
	theProbe += qGray( theImage.getImage().pixel( xIt + 2, yIt + 2 ) );
	theProbe += qGray( theImage.getImage().pixel( xIt + 2, yIt + 3 ) );
	theProbe += qGray( theImage.getImage().pixel( xIt + 3, yIt + 2 ) );
	theProbe += qGray( theImage.getImage().pixel( xIt + 3 ,yIt + 3 ) );
 	theProbe /= 4.;

	// check if the summed value is above step2 threshold
	if( theProbe < step2Threshold_ ) continue;

	// have a "cornfirmed" seed here..
	// now reconstruct its position;
	// do this recursively 3 times for improving the coordinates

	// iteration 1
	DefoPoint intermediatePoint = averageSquare( theImage.getImage(), DefoPoint( xIt, yIt ) );

	// check if this point is still in range, otherwise drop it
	if( intermediatePoint.getX() < halfSquareWidth_ + 1 ||
	    intermediatePoint.getY() < halfSquareWidth_ + 1 ||
	    intermediatePoint.getX() > imageWidth.first  - halfSquareWidth_ - 1 ||
	    intermediatePoint.getY() > imageWidth.second - halfSquareWidth_ - 1    ) {
	  
	  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoImage::reconstruct] =3= (Pos1) Point: x: "
					   << xIt << " y: " << yIt << " jumped to: x: "
					   << intermediatePoint.getX() << " y: " << intermediatePoint.getY()
					   << " . Dropping it." << std::endl;
	  continue;
	}


	// determine if it's a blue LED before the next iterations
	// so we can later (iteration 3) draw the cross in the appropriate color
	const double blueishness = calculateBlueishness( theImage.getImage(), DefoPoint( xIt, yIt ) );

	if( blueishness > blueishnessThreshold_ ) {
	  intermediatePoint.setBlue( true );
	  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoImage::reconstruct] =3= Identified blue point: x: "
					   << intermediatePoint.getX() << " y: " << intermediatePoint.getY() 
					   << " blueishness: " << blueishness << std::endl;
	}

	// iteration 2
	intermediatePoint = averageSquare( theImage.getImage(), intermediatePoint );

	// check if this point is still in range, otherwise drop it
	if( intermediatePoint.getX() < halfSquareWidth_ + 1 ||
	    intermediatePoint.getY() < halfSquareWidth_ + 1 ||
	    intermediatePoint.getX() > imageWidth.first  - halfSquareWidth_ - 1 ||
	    intermediatePoint.getY() > imageWidth.second - halfSquareWidth_ - 1    ) {
	  
	  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoImage::reconstruct] =3= (Pos2) Point: x: "
					   << xIt << " y: " << yIt << " jumped to: x: "
					   << intermediatePoint.getX() << " y: " << intermediatePoint.getY()
					   << " . Dropping it." << std::endl;
	  continue;
	}

	// iteration 3;
	// we draw the point now, even if it could be dropped later
        intermediatePoint = averageSquare( theImage.getImage(), intermediatePoint );


	// check if this point is still in range, otherwise drop it
	if( intermediatePoint.getX() < halfSquareWidth_ + 1 ||
	    intermediatePoint.getY() < halfSquareWidth_ + 1 ||
	    intermediatePoint.getX() > imageWidth.first  - halfSquareWidth_ - 1 ||
	    intermediatePoint.getY() > imageWidth.second - halfSquareWidth_ - 1    ) {
	  
	  if( debugLevel_ >= 3 ) std::cout << " [DefoRecoImage::reconstruct] =3= (Pos3) Point: x: "
					   << xIt << " y: " << yIt << " jumped to: x: "
					   << intermediatePoint.getX() << " y: " << intermediatePoint.getY()
					   << " . Dropping it." << std::endl;
	  continue;
	}

	// check again since the point can be reconstructed at a distance from the seed
	if( forbiddenAreas_.isInside( intermediatePoint ) ) continue;

	theOutput.first.push_back( intermediatePoint );

	if( debugLevel_ >= 3 ) std::cout << " [DefoRecoImage::reconstruct] =3= Reconstructed point at: x: "
					 << intermediatePoint.getX() << " y: " << intermediatePoint.getY()
					 << " blue: " << blueishness << std::endl;

	// save square around this point as already tagged
	forbiddenAreas_.push_back( DefoSquare( intermediatePoint, halfSquareWidth_ ) );
	
      }

    }
  }

  return theOutput;

}



///
/// around a given seed point (theSeed), create a square (width=halfSquareWidth),
/// find center-of-gravity and return this as the reconstructed point position;
/// if 3rd parameter is nonzero, draw rectangle & cross in that image
///
DefoPoint DefoRecoImage::averageSquare( QImage const& theImage, DefoPoint const& theSeed ) const {
  
  DefoPoint outputPoint( 0., 0. );
  outputPoint.setBlue( theSeed.isBlue() ); // copy "red" flag
  int squareSummedAmplitude = 0;

  // create a square of pixels round the seed
  DefoSquare theSquare( theSeed, halfSquareWidth_ );

  // loop all pixels in the square
  for( DefoSquare::iterator it = theSquare.begin(); it <= theSquare.end(); ++it ) {

    // retrieve grayscale value of that pixel
    const int grayscaleValue = qGray( theImage.pixel( it.getPoint().getPixX(), it.getPoint().getPixY() ) );

    // sum amplitude of all pixels above step3 threshold
    // for center-of-gravity
    if( grayscaleValue > step3Threshold_ ) {
      outputPoint += ( DefoPoint( it.getPoint().getX(), it.getPoint() .getY() ) * grayscaleValue );
      squareSummedAmplitude += grayscaleValue;
    }

  }

  // divide by total sum for center-of-gravity
  if( squareSummedAmplitude > 0 ) outputPoint /= squareSummedAmplitude;

  return outputPoint;

}



///
/// determines the "blueishness" of a point,
/// i.e. the average ratio blue/yellow of the color of its pixels
/// to determine if this is the reflection of a blue LED
///
double DefoRecoImage::calculateBlueishness( QImage const& theImage, DefoPoint const& theSeed ) const {

  double summedBlueishness = 0.;
  unsigned int nPixel = 0;

  // create a square of pixels round the seed
  DefoSquare theSquare( theSeed, halfSquareWidth_ );

  // loop all pixels in the square
  for( DefoSquare::iterator it = theSquare.begin(); it <= theSquare.end(); ++it ) {

    // retrieve grayscale value of that pixel
    const int grayscaleValue = qGray( theImage.pixel( it.getPoint().getPixX(), it.getPoint().getPixY() ) );

    // determine "blueishness" of this pixel
    if( grayscaleValue ) {

      const double yellow = (
	  qRed( theImage.pixel( it.getPoint().getPixX(), it.getPoint().getPixY() ) )
	+ qGreen( theImage.pixel( it.getPoint().getPixX(), it.getPoint().getPixY() ) )
      ) / 2.;

      if( yellow ) {
	summedBlueishness += qBlue( theImage.pixel( it.getPoint().getPixX(), it.getPoint().getPixY() ) ) / yellow;
	nPixel++;
      }
    }


  }

  if( nPixel ) return( summedBlueishness / nPixel );
  else {
    std::cerr << " [DefoRecoImage::calculateBlueishness] ** ERROR: No pixels > 0 in point window, expect program failure." << std::endl;
    return 0.;
  }

}
