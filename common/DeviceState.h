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

#ifndef DEVICESTATE_H
#define DEVICESTATE_H

#include <QObject>
#include <iostream>

/** @addtogroup common
 *  @{
 */

/**
  \brief Enumeration of possible states the device communication can be in.
  As initialization and closing might take a while, corresponding states are
  provided. When running device control in a seperate thread, the GUI may
  act accordingly to these intermediate states by e.g. temporarily disabling
  parameter adjustments.
  \par It is required that classes descending from this template class comply
  to the following set of rules:
  \li Also descend from QObject,
  \li Implement \c setDeviceEnabled as a public slot,
  \li Implement the \c deviceStateChanged(State) signal.
  */
enum State {
    OFF           /**< Device not connected or powered */
  , READY         /**< Device connected or powered */
  , INITIALIZING  /**< Device transitioning to READY state */
  , CLOSING       /**< Device transitioning to OFF state */
};

template <class Controller> class AbstractDeviceModel
{
 public:
    explicit AbstractDeviceModel() : controller_(NULL), state_(OFF) { }
  virtual ~AbstractDeviceModel() { 
      destroyController(); }

  /// Returns the current (cached) state of the device.
  const State& getDeviceState() const { 
      return state_; }

  /// Attempts to enable/disable the (communication with) the device.
  virtual void setDeviceEnabled( bool enabled ) {
     // To be enabled and off
    if (enabled && state_ == OFF){
      initialize();
    }
    // To be disabled and on
    else if (!enabled && state_ == READY){
      close();
    }
    /*
     If in 'busy state', a signal for OFF/READY will follow soon, reverting
     any changes ignored by only checking for steady states (i.e. OFF and READY).
     */
  }

protected:

  Controller* controller_; ///< Current device controller or NULL if not active.

  /// Renews the current Controller* for the given port.
  virtual void renewController( const QString& port ) {
    if ( controller_ ) delete controller_;
    controller_ = new Controller( port.toStdString().c_str() );
  }

  /// Destroys the current Controller* and sets it to NULL
  virtual void destroyController() {
    delete controller_;
    controller_ = NULL;
  }

  /// Sets up the communication with the device.
  virtual void initialize() = 0;

  /**
    Closes the connection with the device. In it simplest form this just sets
    the  correct device states and destroys the controller. Subclasses may
    extend this with on-device clean up.
    */
  virtual void close() {
    if ( state_ == READY ) {
      setDeviceState( CLOSING );
      destroyController();
      setDeviceState( OFF );
    }
  }

  State state_; ///< Cached device state.
  /**
    \brief Sets the current device state.
    To be implemented as a slot and emit a deviceStateChanged signal on changes.
    Due to MOC and template incompatibility, this cannot be implemented in
    this class.
    */
  virtual void setDeviceState( State state ) = 0;

};

/** @} */

#endif // DEVICESTATE_H
