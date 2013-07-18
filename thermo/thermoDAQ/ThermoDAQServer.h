#ifndef THERMODAQSERVER_H
#define THERMODAQSERVER_H

#include <QTcpServer>
#include <QThread>
#include <QTcpSocket>

#include <ThermoDAQModel.h>

class ThermoDAQServerThread : public QThread
{
    Q_OBJECT

public:
    ThermoDAQServerThread(int socketDescriptor, ThermoDAQModel* model, QObject *parent);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor_;
    ThermoDAQModel* model_;
};

class ThermoDAQServer : public QTcpServer
{
    Q_OBJECT

public:
    ThermoDAQServer(ThermoDAQModel* model, QObject *parent = 0);

protected:
    void incomingConnection(int socketDescriptor);

private:
    ThermoDAQModel* model_;
};

#endif // THERMODAQSERVER_H
