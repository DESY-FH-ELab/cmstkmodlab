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
#include <array>
#include <tuple>

#include <QString>
#include <QStringList>
#include <QTimer>
#include <map>

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
  
  double getPT03() const { return PT03_; }
  double getPT05() const { return PT05_; }
  double getPT01CO2() const { return PT01CO2_; }
  double getPT02CO2() const { return PT02CO2_; }
  double getPT03CO2() const { return PT03CO2_; }
  double getPT04CO2() const { return PT04CO2_; }
  double getPT05CO2() const { return PT05CO2_; }
  double getPT06CO2() const { return PT06CO2_; }
  double getTT02() const { return TT02_; }
  double getTT01CO2() const { return TT01CO2_; }
  double getTT02CO2() const { return TT02CO2_; }
  double getTT03CO2() const { return TT03CO2_; }
  double getTT04CO2() const { return TT04CO2_; }
  double getTT05CO2() const { return TT05CO2_; }
  double getTT06CO2() const { return TT06CO2_; }
  double getTT07CO2() const { return TT07CO2_; }
  double getSH05() const { return SH05_; }
  double getSC01CO2() const { return SC01CO2_; }
  double getSC02CO2() const { return SC02CO2_; }
  double getSC03CO2() const { return SC03CO2_; }
  double getSC05CO2() const { return SC05CO2_; }
  double getSC06CO2() const { return SC06CO2_; }
  double getdP01CO2() const { return dP01CO2_; }
  double getdP02CO2() const { return dP02CO2_; }
  double getdP03CO2() const { return dP03CO2_; }
  double getdP04CO2() const { return dP04CO2_; }
  double getdT02CO2() const { return dT02CO2_; }
  double getdT03CO2() const { return dT03CO2_; }
  double getST01CO2() const { return ST01CO2_; }
  double getST02CO2() const { return ST02CO2_; }
  double getST03CO2() const { return ST03CO2_; }
  double getST04CO2() const { return ST04CO2_; }
  double getFT01CO2() const { return FT01CO2_; }
  
  double getSpeedSetpoint() const { return SpeedSetpoint_; }
  double getFlowSetpoint() const { return FlowSetpoint_; }
  double getTemperatureSetpoint() const { return TemperatureSetpoint_; }

  uint16_t getAlarms(int idx) const;
  const QStringList& getCurrentAlarms() const;
  uint16_t getAlarmStatus() const { return AlarmStatus_; }
  
  uint16_t getStatus() const { return Status_; }
  bool getChillerOn() const;
  bool getCO2On() const;
  bool getPumpFixedFlow() const;
  double getTemperatureSetpoint2() const { return TemperatureSetpoint2_; }
  double getSpeedSetpoint2() const { return SpeedSetpoint2_; }
  double getFlowSetpoint2() const { return FlowSetpoint2_; }

public slots:

  // Methods for control and status querying of the device itself, as specified
  // by the abstract parent class
  void setDeviceEnabled( bool enabled );
  void setControlsEnabled(bool enabled);

  void setStartChiller(bool value);
  void setStartCO2(bool value);
  void setPumpFixedFlow(bool value);
  void setTemperatureSetpoint(double value);
  void setSpeedSetpoint(double value);
  void setFlowSetpoint(double value);
  
protected:

  const QString ipaddress_;

  void initialize();

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  const double autoUpdateDelay_;
  QTimer* timer_;

  // Last known communication state
  void setDeviceState( State state );

  bool valueChanged(double &storage, double value, unsigned int precision = 3);
  bool valueChanged(uint16_t &storage, uint16_t value);

  double PT03_;
  double PT05_;
  double PT01CO2_;
  double PT02CO2_;
  double PT03CO2_;
  double PT04CO2_;
  double PT05CO2_;
  double PT06CO2_;
  double TT02_;
  double TT01CO2_;
  double TT02CO2_;
  double TT03CO2_;
  double TT04CO2_;
  double TT05CO2_;
  double TT06CO2_;
  double TT07CO2_;
  double SH05_;
  double SC01CO2_;
  double SC02CO2_;
  double SC03CO2_;
  double SC05CO2_;
  double SC06CO2_;
  double dP01CO2_;
  double dP02CO2_;
  double dP03CO2_;
  double dP04CO2_;
  double dT02CO2_;
  double dT03CO2_;
  double ST01CO2_;
  double ST02CO2_;
  double ST03CO2_;
  double ST04CO2_;
  double FT01CO2_;
  double SpeedSetpoint_;
  double FlowSetpoint_;
  double TemperatureSetpoint_;

  std::array<uint16_t,4> Alarms_;
  QStringList CurrentAlarmTexts_;
  void initializeAlarmTexts();
  // std::array<std::map<uint16_t,std::string>,4> AllAlarmTexts_;

  typedef std::tuple<std::string,std::string,bool> Alarm_t;
  std::array<std::map<uint16_t,Alarm_t>,4> AllAlarmTexts_;

  uint16_t AlarmStatus_;

  uint16_t Status_;
  double TemperatureSetpoint2_;
  double SpeedSetpoint2_;
  double FlowSetpoint2_;
  
protected slots:

  void updateInformation();

signals:

  void deviceStateChanged(State newState);
  void informationChanged();
  void alarmsChanged();
  void message(const QString & text);
  void controlStateChanged(bool);
};

/** @} */

/** @} */

#endif // MARTAMODEL_H
