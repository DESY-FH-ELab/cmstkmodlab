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

#ifndef CONRADMODEL_H
#define CONRADMODEL_H

#include <string>

#include "VRelayCardModel.h"
#include "DeviceState.h"

#ifdef USE_FAKEIO
#include "devices/Conrad/ConradControllerFake.h"
typedef ConradControllerFake ConradController_t;
#else
#include "devices/Conrad/ConradController.h"
typedef ConradController ConradController_t;
#endif

/** @addtogroup common
 *  @{
 */

/** @addtogroup models
 *  @{
 */

/*
  Original code by Jan Olzem and Andreas Mussgiller.
  Invasive refactor by Sander Vanheule.
  */

/**
  \brief Model for controlling the Conrad switch.
  This model can be used to set and query the state of the switch. The
  response of the device will not be instantaneous and as such device control
  should be handled in a separate thread. Device states INITIALIZING and
  CLOSING signify that no operations on that device should be attempted!

  Currently any state changing command that does not comply with the ongoing
  operation will be ignored (e.g. disable switch while switch state is INITIALIZING).
  */
class ConradModel : public VRelayCardModel, public AbstractDeviceModel<ConradController_t>
{
  Q_OBJECT

public:

  explicit ConradModel(const std::string& port, QObject* parent=nullptr);
  virtual ~ConradModel();

  // Methods for power control and status querying of the devices connected to the switch
  const State& getSwitchState( int device ) const;

public slots:
  // Methods for control and status querying of the device itself, as specified
  // by the abstract parent class
  void setDeviceEnabled( bool enabled );
  void enableSwitch( int device );
  void disableSwitch( int device );
  void setControlsEnabled(bool enabled);
  void setSwitchEnabled(int device, bool enabled);

protected:

  void initialize();
  void close();

  // Last known communication state
  void setDeviceState( State state );

  std::vector<State> switchStates_;
  void setSwitchState( int device, State state );

private:
  void setDeviceFullOff();
  void setAllSwitchesReady( const std::vector<bool>& ready );
  void setSwitchEnabledRaw( int device, bool enabled );

signals:
  // Classname identifiers are needed because Qt can't resolve internal enums
  void deviceStateChanged(State newState);
  void switchStateChanged(int device, State newState);
  void controlStateChanged(bool);
};

/** @} */

/** @} */

#endif // CONRADMODEL_H
