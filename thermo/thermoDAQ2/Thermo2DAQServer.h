/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMO2DAQSERVER_H
#define THERMO2DAQSERVER_H

#include <QTcpServer>
#include <QThread>
#include <QTcpSocket>

#include <Thermo2DAQModel.h>

class Thermo2DAQServerThread : public QThread
{
  Q_OBJECT

public:
  Thermo2DAQServerThread(int socketDescriptor,
                         Thermo2DAQModel* model,
                         QObject *parent);

  void run();

signals:

  void error(QTcpSocket::SocketError socketError);

private:

  int socketDescriptor_;
  Thermo2DAQModel* model_;
};

class Thermo2DAQServer : public QTcpServer
{
  Q_OBJECT

public:
  Thermo2DAQServer(Thermo2DAQModel* model, QObject *parent = 0);

protected:

  void incomingConnection(int socketDescriptor);

private:

  Thermo2DAQModel* model_;
};

#endif // THERMO2DAQSERVER_H
