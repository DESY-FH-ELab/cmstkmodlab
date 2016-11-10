#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QMutex>
#include <QTimer>
#include <QDateTime>

#include <PumpStationModel.h>

class DataLogger : public QObject
{
  Q_OBJECT

public:

  DataLogger(PumpStationModel* model,
             QObject *parent = 0);

  void start();
  void stop();

protected slots:

  void switchStateChanged(int device, State newState);
  void pressureChanged(int channel, double p);

  void checkRestart();

protected:

  PumpStationModel* model_;

  QMutex mutex_;
  bool isStreaming_;
  QString ofilename_;
  QFile* ofile_;
  QTextStream* stream_;
  QDir currentDir_;
  QDateTime fileDateTime_;
  QTimer* restartTimer_;

  void writeToStream(QString& buffer);
  void writeStatus();
};

#endif // DATALOGGER_H

