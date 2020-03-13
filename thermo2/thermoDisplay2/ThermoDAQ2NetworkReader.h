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
  float          u525wBathTemperature_;
  float          u525wReturnTemperature_;
  float          u525wPumpPressure_;
  int            u525wPower_;
  float          u525wCWInletTemperature_;
  float          u525wCWOutletTemperature_;

  bool           leyboldState_;
  double         leyboldPressure_;

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

  void processLeyboldGraphixOne(QXmlStreamReader& xml);

  void processRohdeSchwarzNGE103B(QXmlStreamReader& xml);
  void processRohdeSchwarzNGE103BChannel(QXmlStreamReader& xml);

  void processKeithleyDAQ6510(QXmlStreamReader& xml);
  void processKeithleyDAQ6510Sensor(QXmlStreamReader& xml);

  Measurement_t measurement_;
};

#endif // THERMODAQ2NETWORKREADER_H
