/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
