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
#include "MartaModel.h"
#include "AgilentTwisTorr304Model.h"
#include "LeyboldGraphixOneModel.h"
#include "RohdeSchwarzNGE103BModel.h"
#include "KeithleyDAQ6510Model.h"

class Thermo2DAQModel : public QObject
{
  Q_OBJECT
public:

  explicit Thermo2DAQModel(HuberUnistat525wModel* huberModel,
		  MartaModel* martaModel,
		  AgilentTwisTorr304Model* agilentModel,
		  LeyboldGraphixOneModel* leyboldModel,
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
  void martaInfoChanged();
  void agilentInfoChanged();
  void leyboldInfoChanged();
  void nge103BInfoChanged();
  void keithleyInfoChanged();

  void clearHistory();

protected:

  bool daqState_;

  HuberUnistat525wModel* huberModel_;
  MartaModel* martaModel_;
  AgilentTwisTorr304Model* agilentModel_;
  LeyboldGraphixOneModel* leyboldModel_;
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
  float u525wInternalTemperature_;
  float u525wProcessTemperature_;
  float u525wReturnTemperature_;
  float u525wPumpPressure_;
  int u525wPower_;
  float u525wCWInletTemperature_;
  float u525wCWOutletTemperature_;
  bool u525AutoPID_;
  int u525KpInternal_;
  float u525TnInternal_;
  float u525TvInternal_;
  int u525KpJacket_;
  float u525TnJacket_;
  float u525TvJacket_;
  int u525KpProcess_;
  float u525TnProcess_;
  float u525TvProcess_;

  // Marta CO2 Chiller Data
  bool martaState_;
  float martaPT03_;
  float martaPT05_;
  float martaPT01CO2_;
  float martaPT02CO2_;
  float martaPT03CO2_;
  float martaPT04CO2_;
  float martaPT05CO2_;
  float martaPT06CO2_;
  float martaTT02_;
  float martaTT01CO2_;
  float martaTT02CO2_;
  float martaTT03CO2_;
  float martaTT04CO2_;
  float martaTT05CO2_;
  float martaTT06CO2_;
  float martaTT07CO2_;
  float martaSH05_;
  float martaSC01CO2_;
  float martaSC02CO2_;
  float martaSC03CO2_;
  float martaSC05CO2_;
  float martaSC06CO2_;
  float martaDP01CO2_;
  float martaDP02CO2_;
  float martaDP03CO2_;
  float martaDP04CO2_;
  float martaDT02CO2_;
  float martaDT03CO2_;
  float martaST01CO2_;
  float martaST02CO2_;
  float martaST03CO2_;
  float martaST04CO2_;
  float martaFT01CO2_;
  float martaSpeedSetpoint_;
  float martaFlowSetpoint_;
  float martaTemperatureSetpoint_;
  std::array<uint16_t,4> martaAlarms_;
  uint16_t martaAlarmStatus_;
  uint16_t martaStatus_;
  float martaTemperatureSetpoint2_;
  float martaSpeedSetpoint2_;
  float martaFlowSetpoint2_;

  // Agilent TwisTorr 304 Data
  bool agilentState_;
  bool agilentPumpState_;
  unsigned int agilentPumpStatus_;
  unsigned int agilentErrorCode_;

  // Leybold GraphixOne Data;
  bool leyboldState_;
  double leyboldPressure_;

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
