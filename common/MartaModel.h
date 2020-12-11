/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef MARTAMODEL_H
#define MARTAMODEL_H

#include <string>

#include <QString>
#include <QTimer>

#include "DeviceState.h"

#ifdef USE_FAKEIO
#include "devices/Marta/MartaFake.h"
typedef MartaFake Marta_t;
#else
#include "devices/Marta/Marta.h"
typedef Marta Marta_t;
#endif

/** @addtogroup common
 *  @{
 */

/** @addtogroup models
 *  @{
 */

/**
  \brief Model for controlling the Marta CO2 chiller.
  */
class MartaModel : public QObject, public AbstractDeviceModel<Marta_t>
{
  Q_OBJECT

public:

  explicit MartaModel(const char* ipaddress,
		      float updateInterval = 10, QObject* parent=nullptr);
  virtual ~MartaModel();

  void ReadRegisters(int addr, int nb, uint16_t *dest);
  
  float ToFloatABCD(uint16_t *reg);
  float ToFloatBADC(uint16_t *reg);
  float ToFloatCDAB(uint16_t *reg);
  float ToFloatDCBA(uint16_t *reg);
  
public slots:

  // Methods for control and status querying of the device itself, as specified
  // by the abstract parent class
  void setDeviceEnabled( bool enabled );
  void setControlsEnabled(bool enabled);

protected:

  const QString ipaddress_;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;

  // Last known communication state
  void setDeviceState( State state );

protected slots:

  void updateInformation();

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void controlStateChanged(bool);
};

/** @} */

/** @} */

#endif // MARTAMODEL_H
