#ifndef DEFOHALFSQUAREWIDTHSPINBOX_H
#define DEFOHALFSQUAREWIDTHSPINBOX_H

#include <QSpinBox>
#include "DefoPointRecognitionModel.h"

class DefoHalfSquareWidthSpinBox : public QSpinBox
{
    Q_OBJECT
public:
  explicit DefoHalfSquareWidthSpinBox(
      DefoPointRecognitionModel* model
    , QWidget *parent = 0
  );

protected:
  DefoPointRecognitionModel* model_;

protected slots:
  void spinnerValueChanged(int value);
  void modelValueChanged(int value);
  void controlStateChanged(bool);

};

#endif // DEFOHALFSQUAREWIDTHSPINBOX_H
