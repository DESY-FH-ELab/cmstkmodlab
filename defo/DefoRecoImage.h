
#ifndef _DEFORECOIMAGE_H
#define _DEFORECOIMAGE_H

#include <iostream>
#include <utility>
#include <cmath>

#include <QImage>

#include "DefoRawImage.h"
#include "DefoPoint.h"
#include "DefoSquare.h"
#include "DefoConfigReader.h"
#include "DefoArea.h"


class DefoRecoImage {

 public:
  DefoRecoImage();
  std::pair<DefoPointCollection,DefoRawImage> reconstruct( DefoRawImage&, DefoArea& );
  void setSeedingThresholds( int a, int b, int c ) { step1Threshold_ = a; step2Threshold_ = b; step3Threshold_ = c; }
  void setHalfSquareWidth( unsigned int h ) { halfSquareWidth_ = h; }
  void setBlueishnessThreshold( double b ) { blueishnessThreshold_ = b; }
  DefoSquareCollection const& getForbiddenAreas( void ) const { return forbiddenAreas_; }

 private:
  DefoPoint averageSquare( QImage const&, DefoPoint const& ) const;
  double calculateBlueishness( QImage const&, DefoPoint const& ) const;

  DefoSquareCollection forbiddenAreas_;
  int step1Threshold_;
  int step2Threshold_;
  int step3Threshold_;
  unsigned int spacingAssumed_;
  unsigned int halfSquareWidth_;
  //  int squareSeedOffsetX_;
  //  int squareSeedOffsetY_;
  double blueishnessThreshold_;
  unsigned int debugLevel_;

};


#endif
