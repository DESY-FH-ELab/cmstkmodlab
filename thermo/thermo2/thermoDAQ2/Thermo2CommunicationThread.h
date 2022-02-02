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

#ifndef THERMO2COMMUNICATIONTHREAD_H
#define THERMO2COMMUNICATIONTHREAD_H

#include <QThread>
#include <QMutex>
#include <QTcpSocket>

class Thermo2CommunicationThread : public QThread
{
  Q_OBJECT
public:
  
  Thermo2CommunicationThread(qintptr socketDescriptor, QObject *parent = 0);
  ~Thermo2CommunicationThread();
  
  void run() override;
  
protected slots:
  
  void readData();

protected:
  
  qintptr socketDescriptor_;
  QTcpSocket* socket_;
};

#endif // THERMO2COMMUNICATIONTHREAD_H

