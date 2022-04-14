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

#ifndef AgilentTwisTorr304MODEL_H_
#define AgilentTwisTorr304MODEL_H_

#include <vector>
#include <limits>
#include <chrono>

#include <QObject>
#include <QString>
#include <QTimer>

#include "DeviceState.h"

#ifdef USE_FAKEIO
#include "devices/Agilent/AgilentTwisTorr304Fake.h"
typedef AgilentTwisTorr304Fake AgilentTwisTorr304_t;
#else
#include "devices/Agilent/AgilentTwisTorr304.h"
typedef AgilentTwisTorr304 AgilentTwisTorr304_t;
#endif

/** @addtogroup common
 *  @{
 */

/** @addtogroup models
 *  @{
 */

class AgilentTwisTorr304Model : public QObject,
                                public AbstractDeviceModel<AgilentTwisTorr304_t>
{
  Q_OBJECT

public:

  explicit AgilentTwisTorr304Model(const char* port,
                                   int updateInterval = 5,
                                   QObject *parent = 0);


public slots:

  const std::string getPumpStatusText() const;
  unsigned int getPumpStatus() const { return pumpStatus_; }
  unsigned int getErrorCode() const { return errorCode_; }

  bool getPumpState() const { return pumpState_; }
  void switchPumpOn();
  void switchPumpOff();

  void setDeviceEnabled(bool enabled);
  void setControlsEnabled(bool enabled);

protected:

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  QString port_;
  int updateInterval_;
  QTimer* timer_;

  void setDeviceState( State state );

  bool pumpState_;
  unsigned int pumpStatus_;
  unsigned int errorCode_;

protected slots:

  void updateInformation();

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void controlStateChanged(bool);
};

/** @} */

/** @} */

#endif // AgilentTwisTorr304MODEL_H_
