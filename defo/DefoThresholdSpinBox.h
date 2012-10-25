#ifndef DEFOTHRESHOLDSPINBOX_H
#define DEFOTHRESHOLDSPINBOX_H

#include <QSpinBox>
#include "DefoPointRecognitionModel.h"

class DefoThresholdSpinBox : public QSpinBox
{
    Q_OBJECT
public:
  explicit DefoThresholdSpinBox(
      DefoPointRecognitionModel* model
    , const DefoPointRecognitionModel::Threshold& threshold
    , QWidget *parent = 0
  );

protected:
  DefoPointRecognitionModel* model_;
  const DefoPointRecognitionModel::Threshold threshold_;

protected slots:
  void spinnerValueChanged(int value);
  void modelValueChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int value
  );
  void dynamicMinimumChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int minimum
  );
  void controlStateChanged(bool);

};

#endif // DEFOTHRESHOLDSPINBOX_H
