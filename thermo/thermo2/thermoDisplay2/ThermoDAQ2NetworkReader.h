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

#ifndef THERMODAQ2NETWORKREADER_H
#define THERMODAQ2NETWORKREADER_H

#include <string>

#include <QObject>
#include <QStringList>
#include <QXmlStreamReader>
#include <QDateTime>

typedef struct {
  QDateTime      dt;

  bool           u525wState_;
  float          u525wTemperatureSetPoint_;
  bool           u525wTemperatureControlMode_;
  bool           u525wTemperatureControlEnabled_;
  bool           u525wCirculatorEnabled_;
  float          u525wInternalTemperature_;
  float          u525wProcessTemperature_;
  float          u525wReturnTemperature_;
  float          u525wPumpPressure_;
  int            u525wPower_;
  float          u525wCWInletTemperature_;
  float          u525wCWOutletTemperature_;

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
} Measurement_t;

class ThermoDAQ2NetworkReader : public QObject
{
  Q_OBJECT
public:
  explicit ThermoDAQ2NetworkReader(QObject* parent);

  const Measurement_t& getMeasurement() const { return measurement_; }

public slots:
  void run(QString& buffer);

signals:
  void finished();

  protected:
  void process(QString& buffer);
  void processLine(QString& line);

  void processHuberUnistat525w(QXmlStreamReader& xml);
  void processHuberUnistat525wControl(QXmlStreamReader& xml);
  void processHuberUnistat525wInfo(QXmlStreamReader& xml);

  void processMarta(QXmlStreamReader& xml);
  void processMartaR507(QXmlStreamReader& xml);
  void processMartaPTCO2(QXmlStreamReader& xml);
  void processMartaTTCO2(QXmlStreamReader& xml);
  void processMartaSCCO2(QXmlStreamReader& xml);
  void processMartaDPCO2(QXmlStreamReader& xml);
  void processMartaDTCO2(QXmlStreamReader& xml);
  void processMartaSTCO2(QXmlStreamReader& xml);
  void processMartaFlow(QXmlStreamReader& xml);
  void processMartaSettings(QXmlStreamReader& xml);
  void processMartaAlarms(QXmlStreamReader& xml);

  void processAgilentTwisTorr304(QXmlStreamReader& xml);

  void processLeyboldGraphixOne(QXmlStreamReader& xml);

  void processRohdeSchwarzNGE103B(QXmlStreamReader& xml);
  void processRohdeSchwarzNGE103BChannel(QXmlStreamReader& xml);

  void processKeithleyDAQ6510(QXmlStreamReader& xml);
  void processKeithleyDAQ6510Sensor(QXmlStreamReader& xml);

  Measurement_t measurement_;
};

#endif // THERMODAQ2NETWORKREADER_H
