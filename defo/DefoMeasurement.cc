#include "DefoMeasurement.h"

DefoMeasurement::DefoMeasurement(const QImage& image) {

  // Configuration
  // FIXME central (singleton) config reader
  DefoConfigReader cfgReader( "defo.cfg" );
  step1Threshold_ = cfgReader.getValue<int>( "STEP1_THRESHOLD" );
  step2Threshold_ = cfgReader.getValue<int>( "STEP2_THRESHOLD" );
  step3Threshold_ = cfgReader.getValue<int>( "STEP3_THRESHOLD" );
  halfSquareWidth_ = cfgReader.getValue<int>( "HALF_SQUARE_WIDTH" );
  blueishnessThreshold_ = cfgReader.getValue<double>( "BLUEISHNESS_THRESHOLD" );
  debugLevel_ = cfgReader.getValue<unsigned int>( "DEBUG_LEVEL" );

  image_ = image;
  timestamp_ = QDateTime::currentDateTime();
  points_ = DefoPointCollection();

//  findPoints();

  for (int x=0; x<image_.width(); ++x) {
    for (int y=0; y<image_.height(); ++y) {
      int gray = qGray(image_.pixel(x, y));
      if (gray > step3Threshold_)
        image_.setPixel(x, y, 0x0000FF);
      else if (gray > step2Threshold_)
        image_.setPixel(x, y, 0x00FF00);
      else if (gray > step1Threshold_)
        image_.setPixel(x, y, 0xFF0000);
      else
        image_.setPixel(x, y, 0);
    }
  }

}

const QDateTime & DefoMeasurement::getTimeStamp() const {
  return timestamp_;
}

const DefoPointCollection & DefoMeasurement::getPoints() const {
  return points_;
}

const QImage & DefoMeasurement::getImage() const {
  return image_;
}

/**
  * Image recognition algorithm that searches for the reflection of grid dots
  * in the measurement picture. An QRect area that defines a subregion of the
  * image may be provided. In the case this points to NULL, the whole image is
  * searched for suitable points. To be called before calling
  * DefoMeasurement::getPoints().
  */
void DefoMeasurement::findPoints(const QRect* searchArea) {

  // Adapted from original implementation in DefoRecoImage::findPoints
  points_.clear();

  DefoSquareCollection forbiddenAreas;

  const int width = image_.width();
  const int height = image_.height();

  if( debugLevel_ >= 2 )
    std::cout << " [DefoMeasurement::findPoints] =2= Image has: "
              << width << " x " << height << " pixels" << std::endl;

  // Determine intersection of area and image
  QRect area;
  if (searchArea == NULL)
    area = image_.rect();
  else
    area = image_.rect() & *searchArea;

  if( debugLevel_ >= 2 )
    std::cout <<  " [DefoMeasurement::findPoints] =2= Scanning area ("
              <<  area.x() << ',' <<  area.y() << ';'
              <<  area.width() << ',' <<  area.height() << ')' << std::endl;

  // Scan x-line per x-line
  int y = area.y();
  while (  y < (area.y() + area.height()) ) {

    int x = area.x();
    while ( x < (area.x() + area.width())) {

      // Check if the point is in the allowed area AND bright enough
      // FIXME leap over forbidden areas instead of having to skip every pixel

      if (    !forbiddenAreas.isInside( DefoPoint( x, y ) )
           && qGray( image_.pixel(x,y) ) > step1Threshold_
           && area.contains( x+3, y+3 )
      ) {

        // We now have an initial seed. Check average amplitude
        // of some more pixels ahead
        double theProbe = 0.;
        theProbe += qGray( image_.pixel( x + 2, y + 2 ) );
        theProbe += qGray( image_.pixel( x + 2, y + 3 ) );
        theProbe += qGray( image_.pixel( x + 3, y + 2 ) );
        theProbe += qGray( image_.pixel( x + 3 ,y + 3 ) );

        // Check if the grayscale value is high enough for the (premultiplied)
        // second threshold.
        if ( theProbe > step2Threshold_ ) {

          // have a "cornfirmed" seed here..
          // now reconstruct its position;
          // do this recursively 3 times for improving the coordinates

          DefoPoint intermediate( x, y );
          QRect searchRect(
                intermediate.getPixX() - halfSquareWidth_
              , intermediate.getPixY() - halfSquareWidth_
              , 2 * halfSquareWidth_
              , 2 * halfSquareWidth_
          );

          int i = 0;

          // check if this point is still in the area, otherwise drop it
          // FIXME DefoSquare intersections
          while ( i < 4 && area.contains( searchRect ) ) {
            intermediate = getCenterOfGravity( searchRect );
            searchRect.setCoords(
                  intermediate.getPixX() - halfSquareWidth_
                , intermediate.getPixY() - halfSquareWidth_
                , intermediate.getPixX() + halfSquareWidth_
                , intermediate.getPixY() + halfSquareWidth_
            );

//            std::cout << "i=" << i << ": " << intermediate << std::endl;

            ++i;
          }

          if ( i == 4 ) { // Iterated without drifting

            // check again since the point can be reconstructed at a distance
            // from the seed
            if( !forbiddenAreas.isInside( intermediate ) ) {

              // Create DefoSquare around last found COG
              DefoSquare searchSquare( intermediate, halfSquareWidth_ );

              if( debugLevel_ >= 3 )
                std::cout << " [DefoMeasurement::findPoints] =3= Reconstructed "
                          << "point at: x: " << intermediate.getX()
                          << " y: " << intermediate.getY() << std::endl;

              // save square around this point as already tagged
              forbiddenAreas.push_back( searchSquare );
              points_.push_back(intermediate);

            }
          }
          else if ( debugLevel_ >= 3 ) { // Log if point drifted away

            std::cout << " [DefoMeasurement::findPoints] =3= (Pos1) Point: x: "
                      << x << " y: " << y << " jumped to: x: "
                      << intermediate.getX() << " y: " << intermediate.getY()
                      << " . Dropping it. " << std::endl;

          }

        }

      }

      ++x;

    }

    ++y;

  }

  if( debugLevel_ >= 2 )
    std::cout << " [DefoMeasurement::findPoints] =2= "
              << points_.size() << " points found." << std::endl;

  // Now that all points have been found, determine their color
  determinePointColors();

}

/**
  * Loops over all the points retrieved by calling findPoints and calls setBlue
  * of the DefoPoint. Please remark that this function does not check if there
  * is only one blue point, but tags all points exceeding the threshold!
  */
void DefoMeasurement::determinePointColors() {

  QRect area;
  QColor color;
//  double yellow;
//  double blue;

  for ( DefoPointCollection::iterator it = points_.begin();
        it < points_.end();
        ++it
  ) {

    area.setCoords(
        it->getPixX() - halfSquareWidth_
      , it->getPixY() - halfSquareWidth_
      , it->getPixX() + halfSquareWidth_
      , it->getPixY() + halfSquareWidth_
    );

    /*
     * FIXME "Blueishness" is ill-defined.
     * Pixels that are perfectly white (r=g=b=1) will validate as blue.
     *    isBlue = ( 2*b / (r+g) ) > threshold (< 1)
     * A HSV-scheme with restrictions on the value and saturation might be
     * better
     */
    // Determine blue - yellow ratio
//    yellow = ( color.red() + color.green() ) / 2.0 ;
//    blue = color.blue();

    // TODO determine whether these thresholds are hardcoded
    color = getAverageColor(area); //.toHsv();
    it->setColor( color );
  }

}

/**
  * Returns the grayscale weighted center of gravity of a certain image region
  * defined by area. If area is (partially) outside image, only pixels inside
  * image are taken into account. Otherwise a point at (0,0) is returned.
  */
// FIXME DefoSquare <> QRect
const DefoPoint DefoMeasurement::getCenterOfGravity(const QRect &area) const {

  // Ensure complete overlap of the area, everything outside is undefined
//  QRect area = area & image_.rect();
  DefoPoint weightedSum(0, 0);

  if ( !area.isNull() ) {
    const int right = area.x() + area.width();
    const int bottom = area.y() + area.height();

    int gray;
    double totalGray = 0.;
    double weightedX = 0.;
    double weightedY = 0.;

    for (int x = area.x(); x < right; ++x) {
      for (int y = area.y(); y < bottom; ++y) {

        gray = qGray( image_.pixel(x, y) );

        // Threshold 3 reduces false positives
        if ( gray > step3Threshold_ ) {
          totalGray += gray;
          weightedX += x*gray;
          weightedY += y*gray;
        }

      }
    }

//    std::cout << weightedX << ','
//              << weightedY << ','
//              << totalGray << ','
//              << (totalGray > 0) << std::endl;
    if ( totalGray > 0 )
      weightedSum.setPosition( weightedX/totalGray, weightedY/totalGray );

//    std::cout << weightedSum << std::endl;
    // otherwise position is (0,0) anyway
  }

  return weightedSum;

}


/**
  * Returns the average color of an area the image. If area is (partially)
  * outside image, only the pixels inside image are taken into account.
  * If the overlap of area and image is empty, rgb(0,0,0) is returned.
  */
const QColor DefoMeasurement::getAverageColor(const QRect &area) const {

  // Ensure complete overlap of the area, everything outside is undefined
  QRect realArea = area & image_.rect();
  QColor average(0, 0, 0);

  if ( !realArea.isNull() ) {
    QRgb pixel;
    int red = 0;
    int green = 0;
    int blue = 0;

    const int right = realArea.x() + realArea.width();
    const int bottom = realArea.y() + realArea.height();
    const int pixelCount = realArea.width() * realArea.height();

    for (int x = realArea.x(); x < right; ++x) {
      for (int y = realArea.y(); y < bottom; ++y) {
        pixel = image_.pixel(x, y);
        red += qRed(pixel);
        green += qGreen(pixel);
        blue += qBlue(pixel);
      }
    }

    average.setRgb( red/pixelCount, green/pixelCount, blue/pixelCount );
  }

  return average;

}
