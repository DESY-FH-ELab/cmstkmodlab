#include <cmath>
#include <iostream>

#include <TString.h>

#include <nqlogger.h>

#include "DefoPointFinder.h"

double twoDgauss(double *x, double *par)
{
  double r1 = (x[0]-par[1])/par[2];
  double r2 = (x[1]-par[3])/par[4];

  return par[0]*std::exp(-0.5*(r1*r1+r2*r2));
}

DefoPointFinder::DefoPointFinder(int block,
                                 QMutex* mutex,
                                 DefoMeasurementListModel *listModel,
                                 DefoPointRecognitionModel *pointModel,
                                 DefoMeasurement *measurement,
                                 const QRect &searchRectangle,
                                 bool do2Dfit,
                                 DefoROIModel * roiModel)
: block_(block),
  mutex_(mutex),
  listModel_(listModel),
  pointModel_(pointModel),
  measurement_(measurement),
  searchArea_(searchRectangle),
  do2Dfit_(do2Dfit),
  roiModel_(roiModel)
{
  image_ = measurement_->getImage();

  connect(this, SIGNAL(pointsFound(DefoMeasurement*, const DefoPointCollection*)),
	  listModel_, SLOT(appendMeasurementPoints(DefoMeasurement*, const DefoPointCollection*)));
  
  NQLogMessage("DefoPointFinder") << "DefoPointerFinder for block "
				  << block_ << " constructed";
}

DefoPointFinder::~DefoPointFinder()
{
  NQLogMessage("DefoPointFinder") << "DefoPointerFinder for block "
				  << block_ << " destructed";  
}

void DefoPointFinder::run()
{
  QPolygonF roi;

  if (roiModel_) {
    for (int j=0;j<roiModel_->size();++j) {
      const QPointF& p = roiModel_->at(j);
      QPointF np(p.x()*image_.width(), p.y()*image_.height());
      roi.push_back(np);
    }
  }

  const DefoPointCollection* points = findPoints(&searchArea_,
						 &roi,
						 pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_1),
						 pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_2),
						 pointModel_->getThresholdValue(DefoPointRecognitionModel::THRESHOLD_3),
						 pointModel_->getHalfSquareWidth());

  emit pointsFound(measurement_, points);
}

/**
  Image recognition algorithm that searches for the reflection of grid dots
  in the measurement picture. An QRect area that defines a subregion of the
  image may be provided. In the case this points to NULL, the whole image is
  searched for suitable points.
 */
const DefoPointCollection* DefoPointFinder::findPoints(const QRect* searchArea,
                                                       const QPolygonF* roi,
                                                       int step1Threshold,
                                                       int step2Threshold,
                                                       int step3Threshold,
                                                       int halfSquareWidth)
{
  DefoPointCollection* points = new DefoPointCollection();

  DefoSquareCollection forbiddenAreas;
  const int step2TotalThreshold = 4 * step2Threshold;

  const int width = image_.width();
  const int height = image_.height();

  // Determine intersection of area and image
  QRect imageArea = image_.rect();
  QRect area;
  if (searchArea == NULL)
    area = imageArea;
  else
    area = imageArea & *searchArea;

  /*
  mutex_->lock();
  NQLogSpam("DefoMeasurement::findPoints()") << "[block=" << block_ << "] Scanning area ("
					     <<  area.x() << ',' <<  area.y() << " -> "
					     <<  area.width() << ',' <<  area.height() << ')';
  mutex_->unlock();
   */

  // Scan x-line per x-line
  int y = area.y();
  while (  y < (area.y() + area.height()) ) {

    int x = area.x();
    while ( x < (area.x() + area.width())) {

      // Check if the point is in the allowed area AND bright enough
      // FIXME leap over forbidden areas instead of having to skip every pixel

      if (!forbiddenAreas.isInside(DefoPoint(x, y)) &&
          qGray( image_.pixel(x,y) ) > step1Threshold &&
          imageArea.contains(x+3, y+3)) {

        // We now have an initial seed. Check average amplitude
        // of some more pixels ahead
        double theProbe = 0.;
        theProbe += qGray( image_.pixel( x + 2, y + 2 ) );
        theProbe += qGray( image_.pixel( x + 2, y + 3 ) );
        theProbe += qGray( image_.pixel( x + 3, y + 2 ) );
        theProbe += qGray( image_.pixel( x + 3 ,y + 3 ) );

        // Check if the grayscale value is high enough for the (premultiplied)
        // second threshold.
        if ( theProbe > step2TotalThreshold ) {

          // have a "cornfirmed" seed here..
          // now reconstruct its position;
          // do this recursively 3 times for improving the coordinates

          DefoPoint intermediate( x, y );
          QRect searchRect(intermediate.getPixX() - halfSquareWidth,
                           intermediate.getPixY() - halfSquareWidth,
                           2 * halfSquareWidth,
                           2 * halfSquareWidth);

          int i = 0;

          // check if this point is still in the area, otherwise drop it
          // FIXME DefoSquare intersections
          while ( i < 4 && imageArea.contains( searchRect ) ) {
            intermediate = getCenterOfGravity(searchRect,
                                              step3Threshold);
            searchRect.setCoords(intermediate.getPixX() - halfSquareWidth,
                                 intermediate.getPixY() - halfSquareWidth,
                                 intermediate.getPixX() + halfSquareWidth,
                                 intermediate.getPixY() + halfSquareWidth);

            ++i;
          }

          if ( i == 4 ) { // Iterated without drifting

            if (do2Dfit_) {
              intermediate = getFitPosition(intermediate,
                                            searchRect,
                                            halfSquareWidth);
            }

            // check again since the point can be reconstructed at a distance
            // from the seed
            if (area.contains(intermediate.getPixX(), intermediate.getPixY()) &&
                !forbiddenAreas.isInside(intermediate)) {
              // FIXME implement point/brightness isolation

              // Create DefoSquare around last found COG
              DefoSquare searchSquare( intermediate, halfSquareWidth );

              /*
	      mutex_->lock();
              NQLogDebug("DefoMeasurement::findPoints()") << "block=" << block_
                  << " Reconstructed point at: x: " << intermediate.getX()
                  << " y: " << intermediate.getY();
	      mutex_->unlock();
               */

              // save square around this point as already tagged
              forbiddenAreas.push_back( searchSquare );

              if (roi->size()==0) {
                intermediate.setValid(true);
                points->push_back(intermediate);
              } else {
                if (roi->containsPoint(QPointF(intermediate.getPixX(), intermediate.getPixY()),
                                       Qt::OddEvenFill)) {
                  intermediate.setValid(true);
                  points->push_back(intermediate);
                }
              }
            }
          }

          /*
	  mutex_->lock();
          NQLogDebug("DefoMeasurement::findPoints()") << "block=" << block_ << " Point: x: "
						      << x << " y: " << y << " jumped to: x: "
						      << intermediate.getX() << " y: " << intermediate.getY()
						      << " -> Dropping it.";
	  mutex_->unlock();
           */
        }

        ++x;

      }
      else {
        // inside forbidden areas
        // TODO find area, skip it
        // x += 2*halfSquareWidth;
        ++x;
      }
    }

    ++y;
  }

  /*
  mutex_->lock();
  NQLogSpam("DefoMeasurement::findPoints()") << "[block=" << block_ << "] "
					     << points->size() << " points found.";
  mutex_->unlock();
   */

  // Now that all points have been found, determine their color
  determinePointColors(points, halfSquareWidth, step3Threshold);

  return points;
}

/**
 * Loops over all the points retrieved by calling findPoints and calls setBlue
 * of the DefoPoint. Please remark that this function does not check if there
 * is only one blue point, but tags all points exceeding the threshold!
 */
void DefoPointFinder::determinePointColors(DefoPointCollection* points,
                                           int halfSquareWidth,
                                           int threshold) const
{
  QRect area;
  QColor color;
  // double yellow;
  // double blue;

  for (DefoPointCollection::iterator it = points->begin();
      it != points->end();
      ++it) {

    area.setCoords(it->getPixX() - halfSquareWidth,
                   it->getPixY() - halfSquareWidth,
                   it->getPixX() + halfSquareWidth,
                   it->getPixY() + halfSquareWidth);

    color = getAverageColor(area, threshold); //.toHsv();
    it->setColor( color );
  }
}

/**
 * Returns the grayscale weighted center of gravity of a certain image region
 * defined by area. If area is (partially) outside image, only pixels inside
 * image are taken into account. Otherwise a point at (0,0) is returned.
 */
// FIXME DefoSquare <> QRect
DefoPoint DefoPointFinder::getCenterOfGravity(const QRect &area,
                                              int threshold) const
{
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
        if ( gray > threshold ) {
          totalGray += gray;
          weightedX += x*gray;
          weightedY += y*gray;
        }

      }
    }

    if ( totalGray > 0 )
      weightedSum.setPosition( weightedX/totalGray, weightedY/totalGray );
  }

  return weightedSum;
}

DefoPoint DefoPointFinder::getFitPosition(const DefoPoint& intermediate,
                                          const QRect &area,
                                          int threshold) const
{
  const int right = area.x() + area.width();
  const int bottom = area.y() + area.height();

  DefoPoint newPoint = intermediate;

  QRgb rgb;
  double value;
  int nPoints = 0;

  mutex_->lock();

  std::cout << intermediate.getX() << std::endl;
  std::cout << intermediate.getY() << std::endl;

  for (int x = area.x(); x < right; ++x) {
    for (int y = area.y(); y < bottom; ++y) {

      rgb = image_.pixel(x, y);
      value = QColor::fromRgb(rgb).toHsv().valueF();

      gr2D_->SetPoint(nPoints, x, y, value);

      nPoints++;
    }
  }

  fitFunc_->SetParameter(0, 1);
  fitFunc_->SetParameter(1, intermediate.getX());
  fitFunc_->SetParameter(2, 1);
  fitFunc_->SetParameter(3, intermediate.getY());
  fitFunc_->SetParameter(4, 1);

  gr2D_->Fit(fitFunc_, "QNC");

  mutex_->unlock();

  newPoint.setX(fitFunc_->GetParameter(1));
  newPoint.setY(fitFunc_->GetParameter(3));

  return newPoint;
}

/**
 * Returns the average color of an area the image. If area is (partially)
 * outside image, only the pixels inside image are taken into account.
 * If the overlap of area and image is empty, rgb(0,0,0) is returned.
 */
const QColor DefoPointFinder::getAverageColor(const QRect &area,
                                              int threshold) const
{
  // Ensure complete overlap of the area, everything outside is undefined
  QRect realArea = area & image_.rect();
  QColor average(0, 0, 0);

  if ( !realArea.isNull() ) {
    QRgb pixel;
    int brightness = 0;
    double red = 0;
    double green = 0;
    double blue = 0;

    int totalBrightness = 0;

    const int right = realArea.x() + realArea.width();
    const int bottom = realArea.y() + realArea.height();
    // const int pixelCount = realArea.width() * realArea.height();

    for (int x = realArea.x(); x < right; ++x) {
      for (int y = realArea.y(); y < bottom; ++y) {
        pixel = image_.pixel(x, y);
        brightness = qGray(pixel);
        // Only take into account sufficiently bright pixels, reduces background
        if ( brightness > threshold ) {
          red += qRed(pixel) * brightness;
          green += qGreen(pixel) * brightness;
          blue += qBlue(pixel) * brightness;
          totalBrightness += brightness;
        }
      }
    }

    average.setRgb(red/totalBrightness,
                   green/totalBrightness,
                   blue/totalBrightness);
  }

  return average;
}
