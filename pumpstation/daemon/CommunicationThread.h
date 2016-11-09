#ifndef COMMUNICATIONTHREAD_H
#define COMMUNICATIONTHREAD_H

#include <QThread>
#include <QTcpSocket>

#include <ConradModel.h>
#include <LeyboldGraphixThreeModel.h>

#include "CommunicationServer.h"

class CommunicationThread : public QThread
{
  Q_OBJECT

public:

  CommunicationThread(ConradModel* conradModel,
                      LeyboldGraphixThreeModel* leyboldModel,
                      QObject *parent = 0);

  void run();

protected:

  ConradModel* conradModel_;
  LeyboldGraphixThreeModel* leyboldModel_;

  CommunicationServer* server_;
};

#endif // COMMUNICATIONTHREAD_H
