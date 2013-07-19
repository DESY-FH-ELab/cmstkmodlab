#ifndef THERMODAQCLIENT_H
#define THERMODAQCLIENT_H

#include <QObject>
#include <QDateTime>
#include <QTcpSocket>

class ThermoDAQClient : public QObject
{
    Q_OBJECT
public:

    explicit ThermoDAQClient(int sock, QObject *parent=0);
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

#endif // THERMODAQCLIENT_H
