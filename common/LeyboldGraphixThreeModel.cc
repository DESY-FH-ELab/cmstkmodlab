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

#include <QApplication>

#include <nqlogger.h>

#include "LeyboldGraphixThreeModel.h"

LeyboldGraphixThreeModel::LeyboldGraphixThreeModel(const char* port,
                                                   double updateInterval,
                                                   QObject * /*parent*/)
  : QObject(),
    AbstractDeviceModel<LeyboldGraphixThree_t>(),
    LeyboldGraphixThree_PORT(port),
    updateInterval_(updateInterval)
{
  for (int i=0;i<3;i++) status_[i] = LeyboldGraphixThree_t::SensorStatus_unknown;
  for (int i=0;i<3;i++) pressure_[i] = 1.013;
  displayUnit_ = LeyboldGraphixThree_t::DisplayUnit_unknown;

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

  setDeviceEnabled(true);

  updateInformation();
}

LeyboldGraphixThree_t::SensorDetectionMode LeyboldGraphixThreeModel::getSensorDetectionMode(int sensor) const
{
  return controller_->GetSensorDetectionMode(sensor);
}

void LeyboldGraphixThreeModel::setSensorDetectionMode(int sensor, LeyboldGraphixThree_t::SensorDetectionMode mode)
{
  controller_->SetSensorDetectionMode(sensor, mode);
}

LeyboldGraphixThree_t::SensorType LeyboldGraphixThreeModel::getSensorType(int sensor) const
{
  return controller_->GetSensorType(sensor);
}

void LeyboldGraphixThreeModel::setSensorType(int sensor, LeyboldGraphixThree_t::SensorType type)
{
  controller_->SetSensorType(sensor, type);
}

const std::string LeyboldGraphixThreeModel::getSensorName(int sensor) const
{
  return controller_->GetSensorName(sensor);
}

void LeyboldGraphixThreeModel::setSensorName(int sensor, const std::string& name)
{
  controller_->SetSensorName(sensor, name);
}

LeyboldGraphixThree_t::SensorStatus LeyboldGraphixThreeModel::getSensorStatus(int sensor) const
{
  if (sensor<1 || sensor>3) return LeyboldGraphixThree_t::SensorStatus_unknown;
  return status_[sensor-1];
}

double LeyboldGraphixThreeModel::getPressure(int sensor) const
{
  if (sensor<1 || sensor>3) return -1.;
  return pressure_[sensor-1];
}

LeyboldGraphixThree_t::DisplayUnit LeyboldGraphixThreeModel::getDisplayUnit() const
{
  return displayUnit_;
}

void LeyboldGraphixThreeModel::setDisplayUnit(LeyboldGraphixThree_t::DisplayUnit unit)
{
  if (displayUnit_ != unit) {
    controller_->SetDisplayUnit(unit);
    displayUnit_ = unit;
    emit informationChanged();
  }
}

const QDateTime LeyboldGraphixThreeModel::getDateTime() const
{
  std::string date = controller_->GetDate();
  std::string time = controller_->GetTime();

  QDateTime dt(QDate::fromString(date.c_str(), "yyyy-MM-dd"),
	       QTime::fromString(time.c_str(), "hh:mm:ss"));
 
  return dt;
}

void LeyboldGraphixThreeModel::setDateTime(const QDateTime& dt)
{
  NQLog("LeyboldGraphixThreeModel") << "setDateTime: "
				    << dt.toString("yyyy-MM-dd").toStdString()
				    << " "
				    << dt.toString("hh:mm:ss").toStdString();
  
  controller_->SetDate(dt.toString("yyyy-MM-dd").toStdString());
  controller_->SetTime(dt.toString("hh:mm:ss").toStdString());
}

void LeyboldGraphixThreeModel::initialize()
{
  NQLog("LeyboldGraphixThreeModel") << "initialize() " << LeyboldGraphixThree_PORT;

  setDeviceState(INITIALIZING);

  renewController(LeyboldGraphixThree_PORT);

  bool enabled = ( controller_ != NULL ) && ( controller_->DeviceAvailable() );

  if ( enabled ) {
    setDeviceState(READY);
    // updateInformation();
  }
  else {
    setDeviceState( OFF );
    delete controller_;
    controller_ = NULL;
  }
}

/// \see AbstractDeviceModel::setDeviceState
void LeyboldGraphixThreeModel::setDeviceState( State state )
{
  if ( state_ != state ) {
    state_ = state;

    // No need to run the timer if the chiller is not ready
    if ( state_ == READY ) {
      timer_->start();
    } else {
      timer_->stop();
    }

    emit deviceStateChanged(state);
  }
}

void LeyboldGraphixThreeModel::updateInformation()
{
  // NQLog("LeyboldGraphixThreeModel", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    // NQLog("LeyboldGraphixThreeModel", NQLog::Debug) << " running in main application thread";
  } else {
    // NQLog("LeyboldGraphixThreeModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  if ( state_ == READY ) {

    // unsigned int status = controller_->GetStatus();

    std::array<LeyboldGraphixThree_t::SensorStatus,3> status;
    std::array<double,3> pressure;

    for (int i=0;i<3;++i) {
      status[i] = controller_->GetSensorStatus(i+1);
      pressure[i] = controller_->GetPressure(i+1);
    }

    LeyboldGraphixThree_t::DisplayUnit displayUnit = controller_->GetDisplayUnit();

    if (status != status_ ||
        pressure != pressure_ ||
        displayUnit != displayUnit_) {

      status_ = status;
      pressure_ = pressure;

      displayUnit_ = displayUnit;

      // NQLog("LeyboldGraphixThreeModel", NQLog::Spam) << "information changed";

      emit informationChanged();
    }
  }
}

/// Attempts to enable/disable the (communication with) the LeyboldGraphixThree controller.
void LeyboldGraphixThreeModel::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel<LeyboldGraphixThree_t>::setDeviceEnabled(enabled);
}

void LeyboldGraphixThreeModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}
