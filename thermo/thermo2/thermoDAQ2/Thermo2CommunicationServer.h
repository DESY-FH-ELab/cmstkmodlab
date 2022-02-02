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

#ifndef THERMO2COMMUNICATIONSERVER_H
#define THERMO2COMMUNICATIONSERVER_H

#include <QTcpServer>

class Thermo2CommunicationServer : public QTcpServer
{
  Q_OBJECT
public:

  Thermo2CommunicationServer(QObject *parent = 0);

protected:

  void incomingConnection(qintptr socketDescriptor);
};

#endif // THERMO2COMMUNICATIONSERVER_H

