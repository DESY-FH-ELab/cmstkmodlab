#include "DefoHalfSquareWidthSpinBox.h"

DefoHalfSquareWidthSpinBox::DefoHalfSquareWidthSpinBox(
    DefoPointRecognitionModel* model
  , QWidget *parent
) :
    QSpinBox(parent)
  , model_(model)
{

  // Don't update while changing, calculations are to lengthy for this
  setKeyboardTracking(false);

  int min = DefoPointRecognitionModel::HALFSQUAREWIDTH_MINIMUM;
  int max = DefoPointRecognitionModel::HALFSQUAREWIDTH_MAXIMUM;

  setMinimum(min);
  setMaximum(max);

  // TODO connect to model
  connect(
        model_
      , SIGNAL(halfSquareWidthChanged(int))
      , this
      , SLOT(modelValueChanged(int))
  );

  connect(
        model_
      , SIGNAL(controlStateChanged(bool))
      , this
      , SLOT(controlStateChanged(bool))
  );

  connect(
        this
      , SIGNAL(valueChanged(int))
      , this
      , SLOT(spinnerValueChanged(int))
  );

  setValue(model_->getHalfSquareWidth());
}

void DefoHalfSquareWidthSpinBox::spinnerValueChanged(int value) {
  model_->setHalfSquareWidth(value);
}

void DefoHalfSquareWidthSpinBox::modelValueChanged(
    int value
) {
  setValue(value);
}

void DefoHalfSquareWidthSpinBox::controlStateChanged(bool enabled) {
  setEnabled(enabled);
}
