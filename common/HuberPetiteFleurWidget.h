#ifndef HUBERHuberPetiteFLEURWIDGET_H
#define HUBERHuberPetiteFLEURWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "HuberPetiteFleurModel.h"
#include "DeviceState.h"

class HuberPetiteFleurWidget : public QWidget
{
    Q_OBJECT
public:
  explicit HuberPetiteFleurWidget(HuberPetiteFleurModel* model,
                                  QWidget *parent = 0);

protected:
  const static int LCD_SIZE = 5;

  HuberPetiteFleurModel* model_;

  QCheckBox* huberPetiteFleurCheckBox_;

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

#endif // HUBERHuberPetiteFLEURWIDGET_H
