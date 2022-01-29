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

#include "LeyboldGraphixOneModel.h"

LeyboldGraphixOneModel::LeyboldGraphixOneModel(const char* port,
                                               double updateInterval,
                                               QObject * /*parent*/)
  : QObject(),
    AbstractDeviceModel<LeyboldGraphixOne_t>(),
    LeyboldGraphixOne_PORT(port),
    updateInterval_(updateInterval)
{
  status_ = LeyboldGraphixOne_t::SensorStatus_unknown;
  pressure_ = 1.013;
  displayUnit_ = LeyboldGraphixOne_t::DisplayUnit_unknown;

  timer_ = new QTimer(this);
  timer_->setInterval(updateInterval_ * 1000);
  connect( timer_, SIGNAL(timeout()), this, SLOT(updateInformation()));

  setDeviceEnabled(true);

  updateInformation();
}

LeyboldGraphixOne_t::SensorDetectionMode LeyboldGraphixOneModel::getSensorDetectionMode() const
{
  return controller_->GetSensorDetectionMode();
}

void LeyboldGraphixOneModel::setSensorDetectionMode(LeyboldGraphixOne_t::SensorDetectionMode mode)
{
  controller_->SetSensorDetectionMode(mode);
}

LeyboldGraphixOne_t::SensorType LeyboldGraphixOneModel::getSensorType() const
{
  return controller_->GetSensorType();
}

void LeyboldGraphixOneModel::setSensorType(LeyboldGraphixOne_t::SensorType type)
{
  controller_->SetSensorType(type);
}

const std::string LeyboldGraphixOneModel::getSensorName() const
{
  return controller_->GetSensorName();
}

void LeyboldGraphixOneModel::setSensorName(const std::string& name)
{
  controller_->SetSensorName(name);
}

LeyboldGraphixOne_t::SensorStatus LeyboldGraphixOneModel::getSensorStatus() const
{
  return status_;
}

const std::string LeyboldGraphixOneModel::getSensorStatusText(LeyboldGraphixOne_t::SensorStatus status) const
{
  return controller_->GetSensorStatusText(status);
}

double LeyboldGraphixOneModel::getPressure() const
{
  return pressure_;
}

LeyboldGraphixOne_t::DisplayUnit LeyboldGraphixOneModel::getDisplayUnit() const
{
  return displayUnit_;
}

void LeyboldGraphixOneModel::setDisplayUnit(LeyboldGraphixOne_t::DisplayUnit unit)
{
  if (displayUnit_ != unit) {
    controller_->SetDisplayUnit(unit);
    displayUnit_ = unit;
    emit informationChanged();
  }
}

const QDateTime LeyboldGraphixOneModel::getDateTime() const
{
  std::string date = controller_->GetDate();
  std::string time = controller_->GetTime();

  QDateTime dt(QDate::fromString(date.c_str(), "yyyy-MM-dd"),
	       QTime::fromString(time.c_str(), "hh:mm:ss"));
 
  return dt;
}

void LeyboldGraphixOneModel::setDateTime(const QDateTime& dt)
{
  NQLog("LeyboldGraphixOneModel") << "setDateTime: "
				    << dt.toString("yyyy-MM-dd").toStdString()
				    << " "
				    << dt.toString("hh:mm:ss").toStdString();
  
  controller_->SetDate(dt.toString("yyyy-MM-dd").toStdString());
  controller_->SetTime(dt.toString("hh:mm:ss").toStdString());
}

void LeyboldGraphixOneModel::initialize()
{
  NQLog("LeyboldGraphixOneModel") << "initialize() ";

  setDeviceState(INITIALIZING);

  renewController(LeyboldGraphixOne_PORT);

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
void LeyboldGraphixOneModel::setDeviceState( State state )
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

void LeyboldGraphixOneModel::updateInformation()
{
  NQLog("LeyboldGraphixOneModel", NQLog::Debug) << "updateInformation()";

  if (thread()==QApplication::instance()->thread()) {
    // NQLog("LeyboldGraphixOneModel", NQLog::Debug) << " running in main application thread";
  } else {
    // NQLog("LeyboldGraphixOneModel", NQLog::Debug) << " running in dedicated DAQ thread";
  }

  if ( state_ == READY ) {

    LeyboldGraphixOne_t::SensorStatus status;
    double pressure;

    status = controller_->GetSensorStatus();
    pressure = controller_->GetPressure();

    LeyboldGraphixOne_t::DisplayUnit displayUnit = controller_->GetDisplayUnit();

    if (status != status_ ||
        pressure != pressure_ ||
        displayUnit != displayUnit_) {

      status_ = status;
      pressure_ = pressure;

      displayUnit_ = displayUnit;

      NQLog("LeyboldGraphixOneModel", NQLog::Spam) << "information changed";

      emit informationChanged();
    }
  }
}

/// Attempts to enable/disable the (communication with) the LeyboldGraphixOne controller.
void LeyboldGraphixOneModel::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel<LeyboldGraphixOne_t>::setDeviceEnabled(enabled);
}

void LeyboldGraphixOneModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}

void LeyboldGraphixOneModel::statusMessage(const QString & text)
{
  emit message(text);
}
