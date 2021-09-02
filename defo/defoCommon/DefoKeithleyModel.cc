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

#include <unistd.h>
#include <thread>

#include <QApplication>

#include <nqlogger.h>

#include "DefoKeithleyModel.h"

DefoKeithleyModel::DefoKeithleyModel(const char* port,
				     int updateInterval,
				     QObject * /*parent*/)
  : KeithleyModel(port, updateInterval)
{

}

/**
  Calls the Keithley controller to get the current reading on the temperatures.
  These are then cached and signals are emitted upon changes.
  */
void DefoKeithleyModel::scanTemperatures()
{
  VKeithley2700::reading_t reading = controller_->Scan();

  // Good scan, cache the retrieved temperatures
  if (controller_->IsScanOk()) {

    NQLog("KeithleyModel", NQLog::Debug) << reading.size() << " temperature readings";

    for (VKeithley2700::reading_t::const_iterator it = reading.begin();
         it < reading.end();
         ++it) {

      unsigned int sensor = it->first;
      double temperature = it->second;

      // Check for changes
      if ( temperatures_.at(sensor) != temperature ) {
        temperatures_[sensor] = temperature;
        emit temperatureChanged(sensor, temperature);
      }

      // Check for OVERFLOW readings, i.e. disconnected or malfunction
      // TODO log OVERFLOW readings
      NQLog("KeithleyModel", NQLog::Debug) << sensor << " : " << temperature;
      // if ( temperature == std::numeric_limits<float>::infinity() )
      //   setSensorEnabled(sensor, false);

    }

    timeBuffer_.push_back(absoluteTime_);
    temperatureBuffer_.push_back(temperatures_);

    std::chrono::time_point<std::chrono::system_clock> lastTime = timeBuffer_.get();
    const std::vector<double> &lastTemperatures = temperatureBuffer_.get();
    std::chrono::duration<double> dt = absoluteTime_ - lastTime;
    
    if (dt.count()>=30) {
      for (unsigned int i=0;i<SENSOR_COUNT;++i) {
	if (sensorStates_[i] != READY) continue;
	
	double gradient = (temperatures_[i] - lastTemperatures[i]) / (dt.count()/60.);
	if ( gradients_.at(i) != gradient ) {
	  gradients_[i] = gradient;
	  emit temperatureGradientChanged(i, gradient);
	}
      }
    }
  } else {
    
    NQLog("KeithleyModel", NQLog::Message) << " scanTemperatures failed";

    /*
    channels_t activeChannels = controller_->GetActiveChannels();

    controller_->Reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    controller_->SetActiveChannels(activeChannels);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    */
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  VKeithley2700::channels_t activeChannels = controller_->GetActiveChannels();
  for (VKeithley2700::channels_t::iterator it=activeChannels.begin();it!=activeChannels.end();++it) {
    NQLog("KeithleyModel", NQLog::Message) << " scanTemperatures active channel: " << *it;
  }
  controller_->Reset();
  
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  
  controller_->SetActiveChannels(activeChannels);
  
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  absoluteTime_ = std::chrono::system_clock::now();
}
