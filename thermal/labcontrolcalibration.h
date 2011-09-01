#ifndef LABCONTROLCALIBRATION_H
#define LABCONTROLCALIBRATION_H

#include <utility>

class LabControlCalibration
{
    friend class LabControl;
public:
    LabControlCalibration();

    float calibratedTemperature(int channel, float temperature);

protected:

    void setCalibration(int channel, const std::pair<float,float> & calibration);

private:

    std::pair<float,float> calibration_[10];
};

#endif // LABCONTROLCALIBRATION_H
