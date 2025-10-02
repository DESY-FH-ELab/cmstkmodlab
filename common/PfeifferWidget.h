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
#include <QGroupBox>
#include <QLabel>

#include "PfeifferModel.h"
#include "DeviceState.h"

/** @addtogroup common
 *  @{
 */

/** @addtogroup widgets
 *  @{
 */

class PfeifferWidget : public QWidget
{
    Q_OBJECT
public:
  explicit PfeifferWidget(PfeifferModel* model, QWidget *parent = 0);

protected:
  const static int LCD_SIZE = 5;
  static const QString LABEL_FORMAT;

  PfeifferModel* model_;

  QCheckBox* pfeifferCheckBox_;

  QWidget* operationPanel_;

public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class PfeifferGaugeWidget : public QGroupBox
{
  Q_OBJECT

public:
  explicit PfeifferGaugeWidget(PfeifferModel* model_,
                                     unsigned int sensor,
                                     QWidget *parent = 0);

protected:

  PfeifferModel* model_;
  unsigned int sensor_;

  static const unsigned int LCD_SIZE;
  static const QString LABEL_FORMAT;

  QLabel* statusDisplay_;
  QLCDNumber* pressureDisplay_;

public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

/** @} */

/** @} */

#endif // PFEIFFERWIDGET_H
