#ifndef MICRODAQSERVER_H
#define MICRODAQSERVER_H

#include <QTcpServer>
#include <QThread>
#include <QTcpSocket>

#include <MicroDAQModel.h>

class MicroDAQServerThread : public QThread
{
    Q_OBJECT

public:
    MicroDAQServerThread(int socketDescriptor, MicroDAQModel* model, QObject *parent);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor_;
    MicroDAQModel* model_;
};

class MicroDAQServer : public QTcpServer
{
    Q_OBJECT

public:
    MicroDAQServer(MicroDAQModel* model, QObject *parent = 0);

protected:
    void incomingConnection(int socketDescriptor);

private:
    MicroDAQModel* model_;
};

#endif // MICRODAQSERVER_H
