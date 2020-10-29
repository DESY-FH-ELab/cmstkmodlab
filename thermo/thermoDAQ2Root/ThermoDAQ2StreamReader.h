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

#ifndef THERMODAQ2STREAMREADER_H
#define THERMODAQ2STREAMREADER_H

#include <string>

#include <QObject>
#include <QStringList>
#include <QXmlStreamReader>

#include <TFile.h>
#include <TTree.h>
#include <TDatime.h>

typedef struct {
  unsigned int   uTime;
  TDatime        datime;

  float          nge103BVoltage[3];
  float          nge103BCurrent[3];
} Measurement2_t;

typedef struct {
  unsigned int   uTime;
  TDatime        datime;

  std::string    message;
} Log2_t;

class ThermoDAQ2StreamReader : public QObject
{
  Q_OBJECT
public:
  explicit ThermoDAQ2StreamReader(QStringList arguments, QObject* parent);

public slots:
  void run();

signals:
  void finished();

protected:
  QStringList arguments_;
  void process();
  void processLine(QString& line);

  void processLog(QXmlStreamReader& xml);
  void processDAQStarted(QXmlStreamReader& xml);

  void processRohdeSchwarzNGE103B(QXmlStreamReader& xml);
  void processRohdeSchwarzNGE103BChannel(QXmlStreamReader& xml);

  bool measurementValid_;
  Measurement2_t measurement_;
  Log2_t         log_;

  TFile *ofile_;
  TTree *otree_;
  TTree *ologtree_;
};

#endif // THERMODAQ2STREAMREADER_H
