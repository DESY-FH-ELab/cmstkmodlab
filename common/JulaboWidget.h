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

#ifndef JULABOWIDGET_H
#define JULABOWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "JulaboModel.h"
#include "DeviceState.h"

/** @addtogroup common
 *  @{
 */

/** @addtogroup widgets
 *  @{
 */

class JulaboWidget : public QWidget
{
    Q_OBJECT
public:
  explicit JulaboWidget(JulaboModel* model, QWidget *parent = 0);

protected:
  const static int LCD_SIZE = 5;

  JulaboModel* model_;

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

/** @} */

/** @} */

#endif // JULABOWIDGET_H
