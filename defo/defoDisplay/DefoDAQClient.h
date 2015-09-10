#ifndef DEFODAQCLIENT_H
#define DEFODAQCLIENT_H

#include <QObject>
#include <QDateTime>
#include <QTcpSocket>

class DefoDAQClient : public QObject
{
    Q_OBJECT
public:

    explicit DefoDAQClient(int sock, QObject *parent=0);
    void readDAQStatus();

protected slots:

    void read();

signals:

    void handleMessage(QString&);

protected:

    int socket_;
    QTcpSocket *tcpSocket_;
    quint16 blockSize_;
};

#endif // DEFODAQCLIENT_H
