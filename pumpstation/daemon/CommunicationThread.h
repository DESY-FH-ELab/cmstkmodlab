#ifndef COMMUNICATIONTHREAD_H
#define COMMUNICATIONTHREAD_H

#include <QThread>
#include <QTcpSocket>

#include <ConradModel.h>

#include "CommunicationServer.h"

class CommunicationThread : public QThread
{
  Q_OBJECT

public:

  CommunicationThread(ConradModel* conradModel,
                      QObject *parent = 0);

  void run();

protected:

  ConradModel* conradModel_;
  CommunicationServer* server_;
};

#endif // COMMUNICATIONTHREAD_H
