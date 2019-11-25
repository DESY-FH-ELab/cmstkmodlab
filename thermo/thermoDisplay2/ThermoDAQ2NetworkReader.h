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

  float          nge103BVoltage[3];
  float          nge103BCurrent[3];
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

  void processRohdeSchwarzNGE103B(QXmlStreamReader& xml);
  void processRohdeSchwarzNGE103BChannel(QXmlStreamReader& xml);

  /*
    void processHuberTemperature(QXmlStreamReader& xml);
    void processKeithleyState(QXmlStreamReader& xml);
    void processKeithleyTemperature(QXmlStreamReader& xml);
    void processPfeifferPressure(QXmlStreamReader& xml);
    void processHamegSetup(QXmlStreamReader& xml);
    void processHamegSetvalues(QXmlStreamReader& xml);
    void processHamegValues(QXmlStreamReader& xml);
    void processIotaSetup(QXmlStreamReader& xml);
    void processIotaValues(QXmlStreamReader& xml);
    void processArduinoPressure(QXmlStreamReader& xml);
    */

  Measurement_t measurement_;
};

#endif // THERMODAQ2NETWORKREADER_H
