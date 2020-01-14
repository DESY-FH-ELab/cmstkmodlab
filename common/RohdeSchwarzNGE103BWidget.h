/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef ROHDESCHWARZNGE103BWIDGET_H
#define ROHDESCHWARZNGE103BWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>

#include "RohdeSchwarzNGE103BModel.h"
#include "DeviceState.h"

class RohdeSchwarzNGE103BChannelWidget : public QGroupBox
{
  Q_OBJECT
public:
  explicit RohdeSchwarzNGE103BChannelWidget(RohdeSchwarzNGE103BModel* model,
                                            int channel, QWidget *parent = 0);

protected:

  const static int LCD_SIZE = 5;

  RohdeSchwarzNGE103BModel* model_;
  int channel_;

  QLCDNumber* voltageDisplay_;
  QLCDNumber* currentDisplay_;

  QDoubleSpinBox* voltageSpinner_;
  QDoubleSpinBox* currentSpinner_;
  QDoubleSpinBox* easyRampDurationSpinner_;
  QCheckBox* easyRampStateBox_;
  QCheckBox* outputStateBox_;

  QPalette lcdOFFPalette_;
  QPalette lcdCVPalette_;
  QPalette lcdCCPalette_;

  protected slots:

  void voltageSpinnerChanged(double);
  void currentSpinnerChanged(double);
  void easyRampDurationSpinnerChanged(double);
  void easyRampStateChanged(bool);
  void setOutputStateChanged(bool);

  void updateDeviceState(State);
  void controlStateChanged(bool);
  void updateInfo();
};

class RohdeSchwarzNGE103BWidget : public QWidget
{
  Q_OBJECT
public:
  explicit RohdeSchwarzNGE103BWidget(RohdeSchwarzNGE103BModel* model, QWidget *parent = 0);

protected:
  RohdeSchwarzNGE103BModel* model_;

  QCheckBox* deviceCheckBox_;

  QWidget* operationPanel_;

  RohdeSchwarzNGE103BChannelWidget* channel1_;
  RohdeSchwarzNGE103BChannelWidget* channel2_;
  RohdeSchwarzNGE103BChannelWidget* channel3_;

  public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // ROHDESCHWARZNGE103BWIDGET_H
