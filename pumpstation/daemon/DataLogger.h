#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QMutex>
#include <QTimer>
#include <QDateTime>

#include <ConradModel.h>

class DataLogger : public QObject
{
  Q_OBJECT

public:

  DataLogger(ConradModel* conradModel,
             QObject *parent = 0);

  void start();
  void stop();

protected slots:

  void switchStateChanged(int device, State newState);
  void checkRestart();

protected:

  ConradModel* conradModel_;

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

