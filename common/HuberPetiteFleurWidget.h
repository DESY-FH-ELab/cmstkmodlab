/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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

    QCheckBox* circulatorCheckBox_;
    QLCDNumber* bathTempLCD_;
    QDoubleSpinBox* workingTempSpinner_;

public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // HUBERHuberPetiteFLEURWIDGET_H
