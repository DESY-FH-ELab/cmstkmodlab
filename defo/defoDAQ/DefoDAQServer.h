#ifndef DEFODAQSERVER_H
#define DEFODAQSERVER_H

#include <QTcpServer>
#include <QThread>
#include <QTcpSocket>

#include <DefoDAQModel.h>

class DefoDAQServerThread : public QThread
{
    Q_OBJECT

public:
    DefoDAQServerThread(int socketDescriptor, DefoDAQModel* model, QObject *parent);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor_;
    DefoDAQModel* model_;
};

class DefoDAQServer : public QTcpServer
{
    Q_OBJECT

public:
    DefoDAQServer(DefoDAQModel* model, QObject *parent = 0);

protected:
    void incomingConnection(int socketDescriptor);

private:
    DefoDAQModel* model_;
};

#endif // DEFODAQSERVER_H
