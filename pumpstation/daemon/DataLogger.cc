#include <iostream>

#include <QMutexLocker>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "DataLogger.h"

DataLogger::DataLogger(ConradModel* conradModel,
                       QObject *parent)
 : QObject(parent),
   conradModel_(conradModel)
{
  connect(conradModel_, SIGNAL(switchStateChanged(int, State)),
          this, SLOT(switchStateChanged(int, State)));
}

void DataLogger::switchStateChanged(int device, State newState)
{
  if (newState!=OFF && newState!=READY) return;

  NQLogDebug("logger") << "void DataLogger::switchStateChanged(" << device << ", " << (int)newState << ")";
}
