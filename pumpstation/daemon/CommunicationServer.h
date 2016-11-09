#ifndef COMMUNICATIONSERVER_H
#define COMMUNICATIONSERVER_H

#include <QMutex>
#include <QTcpServer>
#include <QTcpSocket>

#include <ConradModel.h>
#include <LeyboldGraphixThreeModel.h>

class CommunicationServer : public QTcpServer
{
  Q_OBJECT

public:

  CommunicationServer(ConradModel* conradModel,
                      LeyboldGraphixThreeModel* leyboldModel,
                      QObject *parent = 0);

protected slots:

  void handleCommand();

signals:

  void setSwitchEnabled(int channel, bool enabled);

protected:

  void incomingConnection(int socketDescriptor);

  ConradModel* conradModel_;
  LeyboldGraphixThreeModel* leyboldModel_;

  QTcpSocket* socket_;
  QMutex mutex_;
};

#endif // COMMUNICATIONSERVER_H

