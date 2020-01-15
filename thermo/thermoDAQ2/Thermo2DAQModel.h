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

#ifndef THERMO2DAQMODEL_H
#define THERMO2DAQMODEL_H

#include <array>

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "HuberUnistat525wModel.h"
#include "RohdeSchwarzNGE103BModel.h"
#include "KeithleyDAQ6510Model.h"

class Thermo2DAQModel : public QObject
{
  Q_OBJECT
public:

  explicit Thermo2DAQModel(HuberUnistat525wModel* huberModel,
                           RohdeSchwarzNGE103BModel* nge103BModel,
                           KeithleyDAQ6510Model* keithleyModel,
                           QObject *parent = 0);

  QDateTime& currentTime();

  void customDAQMessage(const QString & message);
  void createDAQStatusMessage(QString & buffer, bool start=false);

  void myMoveToThread(QThread *thread);

  bool daqState() const { return daqState_; }

public slots:

  void startMeasurement();
  void stopMeasurement();

protected slots:

  void huberInfoChanged();
  void nge103BInfoChanged();
  void keithleyInfoChanged();

  void clearHistory();

protected:

  bool daqState_;

  HuberUnistat525wModel* huberModel_;
  RohdeSchwarzNGE103BModel* nge103BModel_;
  KeithleyDAQ6510Model* keithleyModel_;

  QMutex mutex_;

  QDateTime currentTime_;

  template <typename T> bool updateIfChanged(T &variable, T newValue) {
    if (variable==newValue) return false;
    variable = newValue;
    return true;
  }

  // Huber Unistat 525w Data
  bool u525wState_;
  float u525wTemperatureSetPoint_;
  bool u525wTemperatureControlMode_;
  bool u525wTemperatureControlEnabled_;
  bool u525wCirculatorEnabled_;
  float u525wBathTemperature_;
  float u525wReturnTemperature_;
  float u525wPumpPressure_;
  int u525wPower_;
  float u525wCWInletTemperature_;
  float u525wCWOutletTemperature_;

  // Rohde & Schwarz NGE130B Data
  bool nge103BOutputState_[3];
  unsigned int nge103BOutputMode_[3];
  float nge103BVoltage_[3];
  float nge103BCurrent_[3];
  float nge103BMeasuredVoltage_[3];
  float nge103BMeasuredCurrent_[3];

  // Keithley DAQ6510 Data
  std::array<std::array<bool,10>,2> keithleyState_;
  std::array<std::array<float,10>,2> keithleyTemperature_;

signals:

  void daqMessage(const QString & message);
  void daqStateChanged(bool running);
  void newDataAvailable();
};

#endif // THERMO2DAQMODEL_H
