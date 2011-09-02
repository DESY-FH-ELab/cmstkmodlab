
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

class DefoRecoImage {

 public:
  DefoRecoImage();
  std::pair<DefoPointCollection,DefoRawImage> reconstruct( DefoRawImage& );

 private:
  DefoPoint averageSquare( QImage const&, DefoPoint const&, QImage* = 0 ) const;
  double calculateReddishness( QImage const&, DefoPoint const& ) const;

  DefoSquareCollection forbiddenAreas_;
  int step1Threshold_;
  int step2Threshold_;
  int step3Threshold_;
  unsigned int spacingAssumed_;
  unsigned int halfSquareWidth_;
  int squareSeedOffsetX_;
  int squareSeedOffsetY_;
  double reddishnessThreshold_;
  unsigned int debugLevel_;

};


#endif
