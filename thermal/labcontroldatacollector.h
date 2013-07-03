#ifndef LABCONTROLDATACOLLECTOR_H
#define LABCONTROLDATACOLLECTOR_H

#include <vector>
#include <iostream>
#include <fstream>

#include <QThread>
#include <QDateTime>
#include <QVector>

#include "devices/Julabo/VJulaboFP50.h"
#include "devices/Hameg/VHameg8143.h"
#include "devices/Keithley/VKeithley2700.h"
#include "devices/Greisinger/VGMH3750.h"

#include "labcontroldata.h"
#include "labcontrolcalibration.h"

class LabControlDataCollector;

class LabControlDAQThread : public QThread
{
    friend class LabControlDataCollector;

 public:

    LabControlDAQThread(LabControlDataCollector *);

    virtual void run();

 protected:

    LabControlDataCollector* dataCollector_;
};

class LabControlHamegStatus
{
    friend class LabControlDataCollector;
    friend class LabControlDAQThread;

public:

    LabControlHamegStatus();

    unsigned int statusBits() const { return statusBits_; }
    float setPointVoltage(int idx) const { return setPointVoltage_[idx]; }
    float voltage(int idx) const { return voltage_[idx]; }
    float currentLimit(int idx) const { return currentLimit_[idx]; }
    float current(int idx) const { return current_[idx]; }

    const LabControlHamegStatus& operator=(const LabControlHamegStatus& other);
    bool operator==(const LabControlHamegStatus& other) const;
    bool operator!=(const LabControlHamegStatus& other) const { return !operator==(other); }

protected:

    void setStatusBits(unsigned int bits) { statusBits_ = bits; }
    void setSetPointVoltage(unsigned int channel, float v) { setPointVoltage_[channel] = v; }
    void setVoltage(unsigned int channel, float v) { voltage_[channel] = v; }
    void setCurrentLimit(unsigned int channel, float v) { currentLimit_[channel] = v; }
    void setCurrent(unsigned int channel, float v) { current_[channel] = v; }

private:

    unsigned int statusBits_;
    float setPointVoltage_[2];
    float voltage_[2];
    float currentLimit_[2];
    float current_[2];
};

class LabControlDataCollector
{
    friend class LabControlDAQThread;

 public:

    LabControlDataCollector();
    LabControlDataCollector(VJulaboFP50* julabo, VHameg8143* hameg8143);

    LabControlCalibration & calibration() { return calibration_; }

    void startDAQ(std::ofstream* file, int frequency);
    void stopDAQ();
    void clear() { dataVec_.clear(); }

    void setDAQTime(unsigned int time) { data_.setDAQTime(time); }

    void collectStatusData();
    void collectData();

    const LabControlData & data() const { return data_; }
    LabControlData & data() { return data_; }

    float pidXp() const { return pidXp_; }
    int pidTn() const { return pidTn_; }
    int pidTv() const { return pidTv_; }
    float workPoint() const { return workPoint_; }
    unsigned int pumpPressure() const { return pumpPressure_; }
    bool circulatorState() const { return circulatorState_; }
    void setPidXp(float v) { pidXp_ = v; }
    void setPidTn(int v) { pidTn_ = v; }
    void setPidTv(int v) { pidTv_ = v; }
    void setWorkPoint(float v) { workPoint_ = v; }
    void setPumpPressure(unsigned int v) { pumpPressure_ = v; }
    void setCirculatorState(bool v) { circulatorState_ = v; }

    int activeChannels() const { return activeChannels_; }
    bool activeChannel(unsigned int channel) const { return activeChannel_[channel]; }
    void setActiveChannel(unsigned int channel, bool v) { activeChannel_[channel] = v; }

    bool referenceActive() const { return referenceActive_; }
    void setReferenceActive(bool v) { referenceActive_ = v; }

    const std::pair<int,std::string> & julaboStatus() const { return julaboStatus_; }

    const LabControlHamegStatus& hamegStatus() const { return hamegStatus_; }
    const LabControlHamegStatus& lastHamegStatus() const { return lastHamegStatus_; }

    void writeFileHeader(std::ostream & os, const QDateTime & dt);
    void writeData(std::ostream & os);
    void writeStatus(std::ostream & os);
    void writeHamegStatus(std::ostream & os);
    void writeHameg(std::ostream & os);

    void daqTimeVector(unsigned int startTime, unsigned int stopTime,
                       QVector<double> & vector);
    void temperatureVector(unsigned int channel,
                           unsigned int startTime, unsigned int stopTime,
                           QVector<double> & vector);
    void referenceTemperatureVector(unsigned int startTime, unsigned int stopTime,
                                    QVector<double> & vector);
    void bathTemperatureVector(unsigned int startTime, unsigned int stopTime,
                               QVector<double> & vector);
    void safetySensorTemperatureVector(unsigned int startTime, unsigned int stopTime,
                                       QVector<double> & vector);
    void heatingPowerVector(unsigned int startTime, unsigned int stopTime,
                            QVector<double> & vector);

protected:

    VKeithley2700 * keithley() { return keithley_; }
    VHameg8143 * hameg8143() { return hameg8143_; }
    void pushData();

    LabControlDAQThread* daqThread_;

    VJulaboFP50* julabo_;
    VHameg8143* hameg8143_;
    VKeithley2700* keithley_;
    VGMH3750* gmh3750_;

    LabControlCalibration calibration_;

    LabControlData data_;
    std::vector<LabControlData> dataVec_;

    float pidXp_;
    int pidTn_;
    int pidTv_;

    float workPoint_;
    unsigned int pumpPressure_;
    bool circulatorState_;

    bool activeChannel_[10];
    int activeChannels_;
    bool referenceActive_;

    std::pair<int,std::string> julaboStatus_;
    LabControlHamegStatus hamegStatus_;
    LabControlHamegStatus lastHamegStatus_;

    int daqFrequency;
    std::ofstream* daqFile_;
};

#endif // LABCONTROLDATACOLLECTOR_H
