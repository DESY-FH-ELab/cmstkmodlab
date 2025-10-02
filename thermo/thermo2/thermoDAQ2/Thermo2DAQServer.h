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
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  Thermo2DAQServerThread(int socketDescriptor,
                         Thermo2DAQModel* model,
                         QObject *parent);
#else
  Thermo2DAQServerThread(qintptr socketDescriptor,
                         Thermo2DAQModel* model,
                         QObject *parent);
#endif

  void run();

signals:

  void error(QTcpSocket::SocketError socketError);

private:

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  int socketDescriptor_;
#else
  qintptr socketDescriptor_;
#endif

  Thermo2DAQModel* model_;
};

class Thermo2DAQServer : public QTcpServer
{
  Q_OBJECT

public:
  Thermo2DAQServer(Thermo2DAQModel* model, QObject *parent = 0);

protected:

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  void incomingConnection(int socketDescriptor);
#else
  void incomingConnection(qintptr socketDescriptor);
#endif

private:

  Thermo2DAQModel* model_;
};

#endif // THERMO2DAQSERVER_H
