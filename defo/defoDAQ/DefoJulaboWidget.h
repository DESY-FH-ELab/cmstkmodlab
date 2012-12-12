#ifndef DEFOJULABOWIDGET_H
#define DEFOJULABOWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "DefoJulaboModel.h"
#include "DefoState.h"

class DefoJulaboWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoJulaboWidget(DefoJulaboModel* model, QWidget *parent = 0);

protected:
  const static int LCD_SIZE = 5;

  DefoJulaboModel* model_;

  QCheckBox* chillerCheckBox_;

  QWidget* operationPanel_;

  QDoubleSpinBox* proportionalSpinner_;
  QSpinBox* integralSpinner_;
  QSpinBox* differentialSpinner_;

  QCheckBox* circulatorCheckBox_;
  QSpinBox* pumpSpinner_;

  QLCDNumber* bathTempLCD_;
  QDoubleSpinBox* workingTempSpinner_;
  QLCDNumber* powerLCD_;

public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateChillerInfo();

};

#endif // DEFOJULABOWIDGET_H
