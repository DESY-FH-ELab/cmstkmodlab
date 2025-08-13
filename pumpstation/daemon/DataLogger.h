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

#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QMutex>
#include <QTimer>
#include <QDir>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <QSocketNotifier>

#include <PumpStationModel.h>
#include <CommunicationThread.h>

class DataLogger : public QObject
{
  Q_OBJECT

public:

  DataLogger(PumpStationModel* model, CommunicationThread* thread,
             QObject *parent = 0);

  // Unix signal handlers.
  static void hupSignalHandler(int unused);
  static void intSignalHandler(int unused);
  static void termSignalHandler(int unused);

public slots:

  void initialize();
  void start();
  void stop();

  // Qt signal handlers.
  void handleSigHup();
  void handleSigInt();
  void handleSigTerm();

protected slots:

  void switchStateChanged(int device, State newState);
  void pressureChanged(int sensor, double p);

  void checkRestart();
  void writeStatus();

protected:

  PumpStationModel* model_;
  CommunicationThread* thread_;

  QMutex mutex_;
  bool isStreaming_;
  QString ofilename_;
  QFile* ofile_;
  QTextStream* stream_;
  QXmlStreamWriter* xml_;
  QDir currentDir_;
  QDateTime fileDateTime_;
  QTimer* restartTimer_;
  QTimer* statusTimer_;

  void writeToStream(QString& buffer);

private:

  static int sighupFd[2];
  static int sigintFd[2];
  static int sigtermFd[2];

  QSocketNotifier *snHup;
  QSocketNotifier *snInt;
  QSocketNotifier *snTerm;
};

#endif // DATALOGGER_H

