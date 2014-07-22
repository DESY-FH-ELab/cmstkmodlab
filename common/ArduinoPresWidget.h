#ifndef ARDUINOPRESWIDGET_H
#define ARDUINOPRESWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "ArduinoPresModel.h"
#include "DeviceState.h"

class ArduinoPresWidget : public QWidget
{
    Q_OBJECT
public:
  explicit ArduinoPresWidget(ArduinoPresModel* model,
                                  QWidget *parent = 0);

protected:
  const static int LCD_SIZE = 5;

  ArduinoPresModel* model_;

  QCheckBox* arduinoCheckBox_;

  QWidget* operationPanel_;

    QLCDNumber* PressureALCD_;
    QLCDNumber* PressureBLCD_;

public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // ARDUINOPRESWIDGET_H
