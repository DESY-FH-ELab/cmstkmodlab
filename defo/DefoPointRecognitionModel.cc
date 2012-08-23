#include "DefoPointRecognitionModel.h"

DefoPointRecognitionModel::DefoPointRecognitionModel(QObject *parent) :
    QObject(parent)
  , thresholds_(3)
{

  DefoConfigReader cfgReader( "defo.cfg" );
  thresholds_[0] = cfgReader.getValue<int>( "STEP1_THRESHOLD" );
  thresholds_[1] = cfgReader.getValue<int>( "STEP2_THRESHOLD" );
  thresholds_[2] = cfgReader.getValue<int>( "STEP3_THRESHOLD" );

//  halfSquareWidth_ = cfgReader.getValue<int>( "HALF_SQUARE_WIDTH" );
//  blueishnessThreshold_ = cfgReader.getValue<double>( "BLUEISHNESS_THRESHOLD" );
  //  debugLevel_ = cfgReader.getValue<unsigned int>( "DEBUG_LEVEL" );

}

int DefoPointRecognitionModel::getThresholdValue(int threshold) const {
  return thresholds_.at(threshold-1);
}

void DefoPointRecognitionModel::setThresholdValue(int threshold, int value) {

  int i = threshold-1;

  if ( thresholds_.at(i) != value) {
    thresholds_[i] = value;
    emit thresholdValueChanged(threshold, value);
  }

}
