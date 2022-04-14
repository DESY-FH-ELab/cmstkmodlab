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

#include <Thermo2DAQModel.h>
#include <HuberUnistat525wModel.h>
#include <MartaModel.h>
#include <AgilentTwisTorr304Model.h>
#include <LeyboldGraphixOneModel.h>
#include <RohdeSchwarzNGE103BModel.h>
#include <KeithleyDAQ6510Model.h>
#include <Thermo2ThroughPlaneModel.h>

class Thermo2CommunicationServer : public QTcpServer
{
  Q_OBJECT
public:

  Thermo2CommunicationServer(Thermo2DAQModel* daqModel,
      HuberUnistat525wModel* huberModel,
      MartaModel* martaModel,
      AgilentTwisTorr304Model* agilentModel,
      LeyboldGraphixOneModel* leyboldModel,
      RohdeSchwarzNGE103BModel* nge103BModel,
      KeithleyDAQ6510Model* keithleyModel,
      QObject *parent = 0);

protected:

  void incomingConnection(qintptr socketDescriptor);

  Thermo2DAQModel* daqModel_;
  HuberUnistat525wModel* huberModel_;
  MartaModel* martaModel_;
  AgilentTwisTorr304Model* agilentModel_;
  LeyboldGraphixOneModel* leyboldModel_;
  RohdeSchwarzNGE103BModel* nge103BModel_;
  KeithleyDAQ6510Model* keithleyModel_;
};

#endif // THERMO2COMMUNICATIONSERVER_H

