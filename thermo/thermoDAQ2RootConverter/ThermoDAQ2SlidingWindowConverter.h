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

#ifndef THERMODAQ2SLIDINGWINDOWCONVERTER_H
#define THERMODAQ2SLIDINGWINDOWCONVERTER_H

#include <string>

#include <QObject>
#include <QStringList>
#include <QFile>
#include <QXmlStreamReader>

#include <TFile.h>
#include <TTree.h>
#include <TDatime.h>

typedef struct {
  unsigned int   uTime;
  TDatime        datime;

  bool           u525wState_;
  float          u525wTemperatureSetPoint_;
  bool           u525wTemperatureControlMode_;
  bool           u525wTemperatureControlEnabled_;
  bool           u525wCirculatorEnabled_;
  float          u525wInternalTemperature_;
  float          u525wSigmaInternalTemperature_;
  float          u525wProcessTemperature_;
  float          u525wSigmaProcessTemperature_;
  float          u525wReturnTemperature_;
  float          u525wPumpPressure_;
  int            u525wPower_;
  float          u525wCWInletTemperature_;
  float          u525wCWOutletTemperature_;
  bool           u525wAutoPID_;
  int            u525wKpInternal_;
  float          u525wTnInternal_;
  float          u525wTvInternal_;
  int            u525wKpJacket_;
  float          u525wTnJacket_;
  float          u525wTvJacket_;
  int            u525wKpProcess_;
  float          u525wTnProcess_;
  float          u525wTvProcess_;

  bool           martaState_;
  float          martaPT03_;
  float          martaPT05_;
  float          martaPT01CO2_;
  float          martaPT02CO2_;
  float          martaPT03CO2_;
  float          martaPT04CO2_;
  float          martaPT05CO2_;
  float          martaPT06CO2_;
  float          martaTT02_;
  float          martaTT01CO2_;
  float          martaTT02CO2_;
  float          martaTT03CO2_;
  float          martaTT04CO2_;
  float          martaTT05CO2_;
  float          martaTT06CO2_;
  float          martaTT07CO2_;
  float          martaSH05_;
  float          martaSC01CO2_;
  float          martaSC02CO2_;
  float          martaSC03CO2_;
  float          martaSC05CO2_;
  float          martaSC06CO2_;
  float          martaDP01CO2_;
  float          martaDP02CO2_;
  float          martaDP03CO2_;
  float          martaDP04CO2_;
  float          martaDT02CO2_;
  float          martaDT03CO2_;
  float          martaST01CO2_;
  float          martaST02CO2_;
  float          martaST03CO2_;
  float          martaST04CO2_;
  float          martaFT01CO2_;
  float          martaSpeedSetpoint_;
  float          martaFlowSetpoint_;
  float          martaTemperatureSetpoint_;
  uint16_t       martaStatus_;
  float          martaTemperatureSetpoint2_;
  float          martaSpeedSetpoint2_;
  float          martaFlowSetpoint2_;
  uint16_t       martaAlarms_[4];

  bool           agilentState_;
  bool           agilentPumpState_;
  unsigned int   agilentPumpStatus_;
  unsigned int   agilentErrorCode_;

  bool           leyboldState_;
  float          leyboldPressure_;

  bool           nge103BState[3];
  float          nge103BVoltage[3];
  float          nge103BCurrent[3];

  bool           keithleyState[2][10];
  float          keithleyTemperature[2][10];
  float          keithleySigmaTemperature[2][10];
} Measurement2_t;

typedef struct {
  unsigned int   uTime;
  TDatime        datime;

  std::string    message;
} Log2_t;

class ThermoDAQ2SlidingWindowConverter : public QObject
{
  Q_OBJECT
public:
  explicit ThermoDAQ2SlidingWindowConverter(int windowSize,
      const QString& ifilename,
      const QString& ofilename,
      QObject* parent);

public slots:
  void run();

signals:
  void finished();

protected:

  int windowSize_;
  const QString ifilename_;
  const QString ofilename_;

  void process();

  Measurement2_t imeasurement_;
  Measurement2_t omeasurement_;
  Log2_t log_;

  TFile *ifile_;
  TTree *itree_;
  TTree *ilogtree_;

  TFile *ofile_;
  TTree *otree_;
  TTree *ologtree_;
};

#endif // THERMODAQ2SLIDINGWINDOWCONVERTER_H
