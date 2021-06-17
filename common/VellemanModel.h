/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//                  Modeled on cmstkmodlab/common/ConradModel.h                //
//                          Last Updated August 27, 2019                       //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef VELLEMANMODEL_H
#define VELLEMANMODEL_H

#include <string>
#include "VRelayCardModel.h"

#include "DeviceState.h"

#ifdef USE_FAKEIO
#include "devices/Velleman/VellemanControllerFake.h"
typedef VellemanControllerFake VellemanController_t;
#else
#include "devices/Velleman/VellemanController.h"
typedef VellemanController VellemanController_t;
#endif // ends #ifdef USE_FAKEIO

/*** 
   Original DESY code written by Jan Olzem and Andreas Mussgiller.
   Invasive refactor by Sander Vanheule. Velleman iteration adapted 
   by Elise Hinkle.
***/

/***
    \brief Model for controlling the Velleman relay card.

    This model can be used to set and query the status of the
    relays. The response of the device will not be instantaneous
    and, as such, device control should be handled in a separate
    thread. Device states INITIALIZING and CLOSING signify that no
    operations on that device should be attempted.

    Currently, any status-changing command that does not comply 
    with the ongoing operation will be ignored (i.e. disable relay
    while relay state is INITIALIZING). 

    Here, "device" primarily refers to the relay card as a whole, 
    while "channel" primarily refers to a specific vacuum channel
    controlled by one relay in the relay card.
***/


class VellemanModel : public VRelayCardModel, public AbstractDeviceModel<VellemanController_t>

{
  Q_OBJECT

public:

  explicit VellemanModel(const std::string& port, QObject* parent=nullptr);
  virtual ~VellemanModel();

  // Methods for power control and querying statuses of devices connected to relay
  const State& getChannelState(int channel) const;

  const State& getSwitchState( int device ) const { return getChannelState(device); }


public slots:
  // Methods for control and querying statuses of device itself (as specified
  // by abstract parent class)
  void setDeviceEnabled(bool enabled);
  void enableChannel(int channel);
  void disableChannel(int channel);
  void setControlsEnabled(bool enabled);
  void setChannelEnabled(int channel, bool enabled);
  void enableSwitch( int device ) { enableChannel(device); }
  void disableSwitch( int device ) { disableChannel(device); }
  void setSwitchEnabled(int device, bool enabled) { setChannelEnabled(device, enabled); }

protected:


  void initialize();
  void close();

  // Last known communication state of device
  void setDeviceState(State state);

  std::vector<State> channelStates_;
  void setChannelState(int channel, State state);

private:

  void setAllChannelsOff();
  void setAllChannelsSame(const std::vector<bool>& ready);
  void setChannelEnabledRaw(int channel, bool enabled);
  
signals:
  // Classname identifiers needed because Qt can't resolve internal enums
  // NOTE: is this happening here though? 
  void deviceStateChanged(State newState);
  void channelStateChanged(int channel, State newState);
  void controlStateChanged(bool);
  
};

#endif // ends #ifndef VELLEMANMODEL_H
