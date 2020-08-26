/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
