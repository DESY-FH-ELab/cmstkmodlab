#ifndef PFEIFFERWIDGET_H
#define PFEIFFERWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "PfeifferModel.h"
#include "DeviceState.h"

class PfeifferWidget : public QWidget
{
    Q_OBJECT
public:
  explicit PfeifferWidget(PfeifferModel* model, QWidget *parent = 0);

protected:
  const static int LCD_SIZE = 5;

  PfeifferModel* model_;

  QCheckBox* pfeifferCheckBox_;

  QWidget* operationPanel_;

//  QDoubleSpinBox* proportionalSpinner_;
//  QSpinBox* integralSpinner_;
//  QSpinBox* differentialSpinner_;

//  QCheckBox* circulatorCheckBox_;
//  QSpinBox* pumpSpinner_;

//  QLCDNumber* bathTempLCD_;
//  QDoubleSpinBox* workingTempSpinner_;
//  QLCDNumber* powerLCD_;

public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // PFEIFFERWIDGET_H
