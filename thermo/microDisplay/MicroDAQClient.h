#ifndef MICRODAQCLIENT_H
#define MICRODAQCLIENT_H

#include <QObject>
#include <QDateTime>
#include <QTcpSocket>

class MicroDAQClient : public QObject
{
    Q_OBJECT
public:

    explicit MicroDAQClient(int sock, QObject *parent=0);
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

#endif // MICRODAQCLIENT_H
