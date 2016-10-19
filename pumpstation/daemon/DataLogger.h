#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QObject.h>

#include <ConradModel.h>

class DataLogger : public QObject
{
  Q_OBJECT

public:

  DataLogger(ConradModel* conradModel,
             QObject *parent = 0);

protected slots:

  void switchStateChanged(int device, State newState);

protected:

  ConradModel* conradModel_;
};

#endif // DATALOGGER_H

