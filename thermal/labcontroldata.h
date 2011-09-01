#ifndef LABCONTROLDATA_H
#define LABCONTROLDATA_H

#include <iostream>

class LabControlData
{
    friend class LabControlDataCollector;
    friend class LabControlDAQThread;

public:
    LabControlData();

    unsigned int daqTime() const { return daqTime_; }

    int heatingPower() const { return heatingPower_; }

    float bathTemperature() const { return bathTemperature_; }
    float safetySensorTemperature() const { return safetySensorTemperature_; }

    float temperature(unsigned int channel) const { return temperature_[channel]; }
    float referenceTemperature() const { return referenceTemperature_; }

protected:

    void setDAQTime(unsigned int time) { daqTime_ = time; }

    void setHeatingPower(unsigned int v) { heatingPower_ = v; }

    void setBathTemperture(float v) { bathTemperature_ = v; }
    void setSafetySensorTemperture(float v) { safetySensorTemperature_ = v; }

    void setTemperature(unsigned int channel, float v) { temperature_[channel] = v; }
    void setReferenceTemperature(float v) { referenceTemperature_ = v; }

private:

    unsigned int daqTime_;

    int heatingPower_;

    float bathTemperature_;
    float safetySensorTemperature_;

    float temperature_[10];

    float referenceTemperature_;
};

#endif // LABCONTROLDATA_H
