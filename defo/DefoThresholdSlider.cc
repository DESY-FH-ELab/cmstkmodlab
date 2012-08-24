#include "DefoThresholdSlider.h"

DefoThresholdSlider::DefoThresholdSlider(
    DefoPointRecognitionModel* model
  , const DefoPointRecognitionModel::Threshold& threshold
  , QWidget *parent
) :
    QSlider(Qt::Horizontal, parent)
  , model_(model)
  , threshold_(threshold)
{

  // Don't update while changing, calculations are to lengthy for this
  setTracking(false);

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
      , SIGNAL(dynamicMinimumChanged(DefoPointRecognitionModel::Threshold,int))
      , this
      , SLOT(dynamicMinimumChanged(DefoPointRecognitionModel::Threshold,int))
  );
  connect(
        this
      , SIGNAL(valueChanged(int))
      , this
      , SLOT(sliderValueChanged(int))
  );

  setValue(model_->getThresholdValue(threshold_));

}

void DefoThresholdSlider::sliderValueChanged(int value) {
  model_->setThresholdValue(threshold_, value);
}

void DefoThresholdSlider::modelValueChanged(
    DefoPointRecognitionModel::Threshold threshold
  , int value
) {
  if (threshold_ == threshold)
    setValue(value);
}

void DefoThresholdSlider::dynamicMinimumChanged(
    DefoPointRecognitionModel::Threshold threshold
  , int minimum
) {
  if (threshold_ == threshold)
    setValue( std::max(minimum, value()) );
}
