/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "DefoThresholdSpinBox.h"

DefoThresholdSpinBox::DefoThresholdSpinBox(
    DefoPointRecognitionModel* model
  , const DefoPointRecognitionModel::Threshold& threshold
  , QWidget *parent
) :
    QSpinBox(parent)
  , model_(model)
  , threshold_(threshold)
{

  // Don't update while changing, calculations are to lengthy for this
  setKeyboardTracking(false);

  int min = DefoPointRecognitionModel::THRESHOLD_MINIMUM;
  int max = DefoPointRecognitionModel::THRESHOLD_MAXIMUM;

  setMinimum(min);
  setMaximum(max);

  // TODO connect to model
  connect(
        model_
      , SIGNAL(thresholdValueChanged(DefoPointRecognitionModel::Threshold,int))
      , this
      , SLOT(modelValueChanged(DefoPointRecognitionModel::Threshold,int))
  );

  connect(
        model_
      , SIGNAL(controlStateChanged(bool))
      , this
      , SLOT(controlStateChanged(bool))
  );

  connect(
        model_
      , SIGNAL(dynamicMinimumChanged(DefoPointRecognitionModel::Threshold,int))
      , this
      , SLOT(dynamicMinimumChanged(DefoPointRecognitionModel::Threshold,int))
  );
  connect(
        this
      , SIGNAL(valueChanged(int))
      , this
      , SLOT(spinnerValueChanged(int))
  );

  setValue(model_->getThresholdValue(threshold_));

}

void DefoThresholdSpinBox::spinnerValueChanged(int value) {
  model_->setThresholdValue(threshold_, value);
}

void DefoThresholdSpinBox::modelValueChanged(
    DefoPointRecognitionModel::Threshold threshold
  , int value
) {
  if (threshold_ == threshold)
    setValue(value);
}

void DefoThresholdSpinBox::dynamicMinimumChanged(
    DefoPointRecognitionModel::Threshold threshold
  , int minimum
) {
  if (threshold_ == threshold)
    setValue( std::max(minimum, value()) );
}

void DefoThresholdSpinBox::controlStateChanged(bool enabled) {
  setEnabled(enabled);
}
