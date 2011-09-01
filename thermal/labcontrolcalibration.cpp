#include "labcontrolcalibration.h"

LabControlCalibration::LabControlCalibration()
{
    for (unsigned int i=0;i<10;i++) {
        calibration_[i] = std::make_pair<float,float>(1.0, 0.0);
    }
}

float LabControlCalibration::calibratedTemperature(int channel, float temperature)
{
    return calibration_[channel].second + calibration_[channel].first * temperature;
}

void LabControlCalibration::setCalibration(int channel, const std::pair<float,float> & calibration)
{
    if (channel<0 || channel>9) return;
    calibration_[channel].first = calibration.first;
    calibration_[channel].second = calibration.second;
}
