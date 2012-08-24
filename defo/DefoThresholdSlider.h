#ifndef DEFOTHRESHOLDSLIDER_H
#define DEFOTHRESHOLDSLIDER_H

#include <QSlider>
#include "DefoPointRecognitionModel.h"

class DefoThresholdSlider : public QSlider
{
    Q_OBJECT
public:
  explicit DefoThresholdSlider(
      DefoPointRecognitionModel* model
    , const DefoPointRecognitionModel::Threshold& threshold
    , QWidget *parent = 0
  );

protected:
  DefoPointRecognitionModel* model_;
  const DefoPointRecognitionModel::Threshold threshold_;

protected slots:
  void sliderValueChanged(int value);
  void modelValueChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int value
  );
  void dynamicMinimumChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int minimum
  );

};

#endif // DEFOTHRESHOLDSLIDER_H
