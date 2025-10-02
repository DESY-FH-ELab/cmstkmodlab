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

#ifndef COMMUNICATIONTHREAD_H
#define COMMUNICATIONTHREAD_H

#include <QThread>
#include <QTcpSocket>

#include <PumpStationModel.h>

#include "CommunicationServer.h"

class CommunicationThread : public QThread
{
  Q_OBJECT

public:

  CommunicationThread(PumpStationModel* model,
                      QObject *parent = 0);

  void run();

protected:

  PumpStationModel* model_;

  CommunicationServer* server_;
};

#endif // COMMUNICATIONTHREAD_H
