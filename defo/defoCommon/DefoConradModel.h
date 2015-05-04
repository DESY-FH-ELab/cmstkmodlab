#ifndef DEFOCONRADMODEL_H
#define DEFOCONRADMODEL_H

#include <QStringList>
#include <QString>
#include <QDir>
#include <iostream>

#include "DeviceState.h"

#ifdef USE_FAKEIO
#include "devices/Conrad/ConradControllerFake.h"
typedef ConradControllerFake ConradController_t;
#else
#include "devices/Conrad/ConradController.h"
typedef ConradController ConradController_t;
#endif


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
  operation will be ignored (e.g. disable switch while switch state is
  INITIALIZING).
  */
class DefoConradModel :
    public QObject
  , public AbstractDeviceModel<ConradController_t>
{
  Q_OBJECT

public:
  /**
    Enumeration of the (relais) switches controlled by the device. The values
    correspond to the switch indices for setSwitchEnabled if they are wired up
    correctly.
    */
  enum DeviceSwitch {
      LIGHT_PANEL_1 = 0 /**< LED light panel number 1 */
    , LIGHT_PANEL_2     /**< LED light panel number 2 */
    , LIGHT_PANEL_3     /**< LED light panel number 3 */
    , LIGHT_PANEL_4     /**< LED light panel number 4 */
    , LIGHT_PANEL_5     /**< LED light panel number 5 */
    , CALIBRATION_LEDS  /**< Calibration LED on granite ground plate */
    , CAMERA            /**< Digital photo camera */
    , SWITCH_8          /**< Currently not used */
  };

  // TODO Add initialisation parameters
  explicit DefoConradModel(QObject *parent = 0);
  virtual ~DefoConradModel();

  // Methods for power control and status querying of the devices connected to
  // the switch
  void setSwitchEnabled( DeviceSwitch device, bool enabled );
  const State& getSwitchState( DeviceSwitch device ) const;
  const State& getSwitchState( int device ) const;

public slots:
  // Methods for control and status querying of the device itself, as specified
  // by the abstract parent class
  void setDeviceEnabled( bool enabled );
  void enableSwitch( DefoConradModel::DeviceSwitch device );
  void disableSwitch( DefoConradModel::DeviceSwitch device );
  void setControlsEnabled(bool enabled);

protected:
  void initialize();
  void close();

  // Last known communication state
  void setDeviceState( State state );

  std::vector<State> switchStates_;
  void setSwitchState( DeviceSwitch device, State state );

private:
  void setDeviceFullOff();
  void setAllSwitchesReady( const std::vector<bool>& ready );
  void setSwitchEnabledRaw( DeviceSwitch device, bool enabled );

signals:
  // Classname identifiers are needed because Qt can't resolve internal enums
  void deviceStateChanged(State newState);
  void switchStateChanged(DefoConradModel::DeviceSwitch device, State newState);
  void defoMessage(const QString & text);
  void controlStateChanged(bool);
};

#endif // DEFOCONRADMODEL_H
