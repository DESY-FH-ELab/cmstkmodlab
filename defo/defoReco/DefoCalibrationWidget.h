#ifndef DEFOCALIBRATIONWIDGET_H
#define DEFOCALIBRATIONWIDGET_H

#include <QWidget>
#include <QDoubleSpinBox>

#include "DefoCalibrationModel.h"

class DefoCalibrationWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoCalibrationWidget(DefoCalibrationModel* calibrationModel,
                                 QWidget *parent = 0);

protected:

  DefoCalibrationModel* calibrationModel_;

  QDoubleSpinBox * calibXSpinBox_;
  QDoubleSpinBox * calibYSpinBox_;

protected slots:

  void calibXChanged(double);
  void calibYChanged(double);
  void calibrationChanged();
};

#endif // DEFOCALIBRATIONWIDGET_H
