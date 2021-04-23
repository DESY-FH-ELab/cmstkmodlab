/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef VRELAYCARDMODEL_H
#define VRELAYCARDMODEL_H

#include <QObject>
#include <QString>

#include "DeviceState.h"

/** @addtogroup common
 *  @{
 */

/** @addtogroup models
 *  @{
 */

/**
  \brief Virtual Model class for relay cards
  */
class VRelayCardModel : public QObject
{
  Q_OBJECT

public:

  explicit VRelayCardModel(const std::string& port, QObject* parent=nullptr);
  virtual ~VRelayCardModel();

  // Methods for power control and status querying of the devices connected to the switch
  virtual const State& getSwitchState( int device ) const = 0;

public slots:
  // Methods for control and status querying of the device itself, as specified
  // by the abstract parent class
  virtual void setDeviceEnabled( bool enabled ) = 0;
  virtual void enableSwitch( int device ) = 0;
  virtual void disableSwitch( int device ) = 0;
  virtual void setControlsEnabled(bool enabled) = 0;
  virtual void setSwitchEnabled(int device, bool enabled) = 0;

protected:

  const QString port_;

  // Last known communication state
  virtual void setDeviceState( State state ) = 0;

signals:
  // Classname identifiers are needed because Qt can't resolve internal enums
  void deviceStateChanged(State newState);
  void switchStateChanged(int device, State newState);
  void controlStateChanged(bool);
};

/** @} */

/** @} */

#endif // VRELAYCARDMODEL_H
