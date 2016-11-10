#ifndef COMMUNICATIONTHREAD_H
#define COMMUNICATIONTHREAD_H

#include <QThread>
#include <QTcpSocket>

#include <PumpStationModel.h>

#include "CommunicationServer.h"

class CommunicationThread : public QThread
{
  Q_OBJECT

public:

  CommunicationThread(PumpStationModel* model,
                      QObject *parent = 0);

  void run();

protected:

  PumpStationModel* model_;

  CommunicationServer* server_;
};

#endif // COMMUNICATIONTHREAD_H
