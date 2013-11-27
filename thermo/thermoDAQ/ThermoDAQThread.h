#ifndef THERMODAQTHREAD_H
#define THERMODAQTHREAD_H

#include <QObject>
#include <QThread>

#include "ThermoDAQModel.h"

class ThermoDAQThread : public QThread
{
public:
  explicit ThermoDAQThread(ThermoDAQModel* daqModel,
                           QObject *parent = 0);
public:
  void run();

protected:
  ThermoDAQModel* daqModel_;
};

#endif // THERMODAQTHREAD_H
