/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QStringList>
#include <QObject>
#include <QTcpSocket>

class Controller : public QObject
{
  Q_OBJECT

public:

  Controller(const QString& command, const QStringList& parameters);

public slots:

  void connectToServer();
  void sendCommand();
  void readResponse();

private:

  const QString command_;
  const QStringList parameters_;

  QTcpSocket *socket_;
  QString ipAddress_;
  quint16 port_;
};

#endif
