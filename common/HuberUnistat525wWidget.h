/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef HUBERUNISTAT525WWIDGET_H
#define HUBERUNISTAT525WWIDGET_H

#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>

#include "HuberUnistat525wModel.h"
#include "DeviceState.h"

class HuberUnistat525wTemperatureControlModeBox : public QComboBox
{
  Q_OBJECT

public:
  explicit HuberUnistat525wTemperatureControlModeBox(HuberUnistat525wModel* model,
                                                     QWidget *parent = 0);

  void setControlMode(bool external);

protected slots:
  void currentItemChanged(int);
  void updateInformation();

protected:
  HuberUnistat525wModel* model_;

signals:
  void valueChanged(bool);
};

class HuberUnistat525wWidget : public QWidget
{
  Q_OBJECT
public:

  explicit HuberUnistat525wWidget(HuberUnistat525wModel* model,
                                  QWidget *parent = 0);

protected:

  const static int LCD_SIZE = 8;

  HuberUnistat525wModel* model_;

  QCheckBox* huberUnistat525wCheckBox_;

  QWidget* operationPanel_;

  QDoubleSpinBox* temperatureSetPointSpinner_;
  
  QCheckBox* autoPIDCheckBox_;
  HuberUnistat525wTemperatureControlModeBox* temperatureControlModeBox_;

  QSpinBox* kpSpinBox_;
  QDoubleSpinBox* tnSpinBox_;
  QDoubleSpinBox* tvSpinBox_;

  QCheckBox* temperatureControlCheckBox_;
  QCheckBox* circulatorCheckBox_;

  QLCDNumber* internalTempLCD_;
  QLCDNumber* processTempLCD_;
  QLCDNumber* returnTempLCD_;

  QLCDNumber* pumpPressureLCD_;
  QLCDNumber* powerLCD_;

  QLCDNumber* cwInletTempLCD_;
  QLCDNumber* cwOutletTempLCD_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // HUBERUNISTAT525WWIDGET_H
