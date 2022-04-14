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

#ifndef KEITHLEYDAQ6510WIDGET_H
#define KEITHLEYDAQ6510WIDGET_H

#include <vector>

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>

#include "KeithleyDAQ6510Model.h"

class KeithleyDAQ6510UpdateIntervalBox : public QComboBox
{
  Q_OBJECT

public:
    explicit KeithleyDAQ6510UpdateIntervalBox(KeithleyDAQ6510Model* model,
                                              QWidget *parent = 0);

protected slots:
    void currentItemChanged(int);

protected:
    KeithleyDAQ6510Model* model_;

signals:
    void valueChanged(int);
};

class KeithleyDAQ6510Widget : public QWidget
{
  Q_OBJECT

public:
  explicit KeithleyDAQ6510Widget(KeithleyDAQ6510Model* model,
                                 QWidget *parent = 0);

protected:
  KeithleyDAQ6510Model* model_;
  QCheckBox* keithleyCheckBox_;
  QCheckBox* scanCheckBox_;
  KeithleyDAQ6510UpdateIntervalBox* updateIntervalBox_;
  QWidget* sensorControlWidget_;
  unsigned int activeSensorCount_;

public slots:
  void keithleyStateChanged(State newState);
  void activeSensorCountChanged(unsigned int sensors);
  void controlStateChanged(bool);
};

class KeithleyDAQ6510SensorModeWidget : public QComboBox
{
  Q_OBJECT
public:
  explicit KeithleyDAQ6510SensorModeWidget(KeithleyDAQ6510Model* model,
      unsigned int sensor,
      QWidget *parent = 0);

protected:

  void updateWidgets();

  KeithleyDAQ6510Model* model_;
  unsigned int sensor_;

protected slots:

  void keithleyStateChanged(State state);
  void scanStateChanged(bool enabled);
  void sensorStateChanged(uint sensor, State state );
  void sensorModeChanged(uint sensor, VKeithleyDAQ6510::ChannelMode_t mode);
  void indexChanged(int);
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class KeithleyDAQ6510TemperatureWidget : public QWidget
{
  Q_OBJECT

public:
  explicit KeithleyDAQ6510TemperatureWidget(KeithleyDAQ6510Model* model_,
                                            unsigned int sensor,
                                            QWidget *parent = 0);

protected:
  KeithleyDAQ6510Model* model_;
  unsigned int sensor_;

  static const unsigned int LCD_SIZE;
  static const QString LABEL_FORMAT;

  QFormLayout* layout_;
  QCheckBox* enabledCheckBox_;
  KeithleyDAQ6510SensorModeWidget* sensorMode_;
  QLabel* currentTempLabel_;
  QLCDNumber* currentTempDisplay_;
  // QLabel* currentGradientLabel_;
  // QLCDNumber* currentGradientDisplay_;

  void updateWidgets();

public slots:

  void keithleyStateChanged(State state);
  void scanStateChanged(bool enabled);
  void controlStateChanged(bool);
  void sensorStateChanged(uint sensor, State state );
  void sensorTemperatureChanged(uint sensor, double temperature);

  void enabledCheckBoxToggled(bool enabled);
};

#endif // KEITHLEYDAQ6510WIDGET_H
