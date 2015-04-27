#ifndef CORIFLOWWIDGET_H
#define CORIDLOWWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "CoriFlowModel.h"
#include "DeviceState.h"

class CoriFlowWidget : public QWidget
{
  Q_OBJECT
public:
  explicit CoriFlowWidget(CoriFlowModel* model,
                             QWidget *parent = 0);

protected:
  const static int LCD_SIZE = 5;

  CoriFlowModel* model_;

  QCheckBox* CoriFlowCheckBox_;

  QWidget* operationPanel_;

  QLCDNumber* TempLCD_;
  QLCDNumber* MeasureLCD_;

  public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // CORIFLOWWIDGET_H
