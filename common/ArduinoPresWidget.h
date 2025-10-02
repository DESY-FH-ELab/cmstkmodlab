/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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

/** @addtogroup common
 *  @{
 */

/** @addtogroup widgets
 *  @{
 */

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

/** @} */

/** @} */

#endif // ARDUINOPRESWIDGET_H
