/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "ApplicationConfig.h"

#include "DefoPointRecognitionModel.h"

const int DefoPointRecognitionModel::THRESHOLD_MINIMUM = 0;
const int DefoPointRecognitionModel::THRESHOLD_MAXIMUM = 255;

const int DefoPointRecognitionModel::HALFSQUAREWIDTH_MINIMUM = 0;
const int DefoPointRecognitionModel::HALFSQUAREWIDTH_MAXIMUM = 250;

DefoPointRecognitionModel::DefoPointRecognitionModel(QObject *parent) :
    QObject(parent)
  , thresholds_(3)
  , halfSquareWidth_(0)
{

//  blueishnessThreshold_ = cfgReader.getValue<double>( "BLUEISHNESS_THRESHOLD" );
  //  debugLevel_ = cfgReader.getValue<unsigned int>( "DEBUG_LEVEL" );

}

int DefoPointRecognitionModel::getThresholdValue(
    const Threshold& threshold
) const {
  return thresholds_.at(threshold);
}

void DefoPointRecognitionModel::setThresholdValue(
    const Threshold& threshold
  , int value
) {

  if ( thresholds_.at(threshold) != value) {

    /*
      Test if the value makes sense, i.e. if its not smaller than any lower
      thresholds
      */
    thresholds_[threshold] = std::max(
            value
          , getThresholdDynamicMininum(threshold)
    );

    // Fist emit lower threshold so that the image is not refreshed.
    if ( threshold < THRESHOLD_3 ) {
      emit dynamicMinimumChanged(
              static_cast<Threshold>(threshold+1)
            , thresholds_[threshold]
      );
    }

    emit thresholdValueChanged(threshold, thresholds_[threshold]);

  }

}

/**
  Returns the lowest logical value given threshold may take.
  For thresholds 2 and 3 it does not make sense to have a lower value than the
  preceding threshold.
  \return The current value of the preceding threshold or 0 if threshold 1.
  */
int DefoPointRecognitionModel::getThresholdDynamicMininum(
    const Threshold& threshold
) const {

  if ( threshold > 0 )
    return getThresholdValue(static_cast<Threshold>(threshold-1));
  else
    return THRESHOLD_MINIMUM;

}

int DefoPointRecognitionModel::getHalfSquareWidth() const {
  return halfSquareWidth_;
}

void DefoPointRecognitionModel::setHalfSquareWidth(int value) {

  if (halfSquareWidth_ != value) {
    if (value > 0) halfSquareWidth_ = value;
    emit halfSquareWidthChanged(halfSquareWidth_);
  }

}

void DefoPointRecognitionModel::setControlsEnabled(bool enabled) {
  emit controlStateChanged(enabled);
}
