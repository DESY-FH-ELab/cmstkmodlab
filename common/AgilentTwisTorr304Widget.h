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

#ifndef AgilentTwisTorr304WIDGET_H_
#define AgilentTwisTorr304WIDGET_H_

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

#include "AgilentTwisTorr304Model.h"
#include "DeviceState.h"

/** @addtogroup common
 *  @{
 */

/** @addtogroup widgets
 *  @{
 */

class AgilentTwisTorr304Widget : public QWidget
{
  Q_OBJECT
public:
  explicit AgilentTwisTorr304Widget(AgilentTwisTorr304Model* model,
                                    QWidget *parent = 0);

protected:

  AgilentTwisTorr304Model* model_;

  QCheckBox* agilentCheckBox_;

  QWidget* operationPanel_;

  QLabel* statusLabel_;
  QLabel* errorCodeLabel_;

  QPushButton* switchPumpOnButton_;
  QPushButton* switchPumpOffButton_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();

protected slots:

  void switchPumpOnRequest();
  void switchPumpOffRequest();
};

/** @} */

/** @} */

#endif // AgilentTwisTorr304WIDGET_H_
