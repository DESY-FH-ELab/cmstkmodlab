#include <iostream>
#include <iomanip>
#include <ios>

#include <QMutex>

#ifdef USE_FAKEIO
#include "devices/Keithley/Keithley2700Fake.h"
#include "devices/Greisinger/GMH3750Fake.h"
#else
#include "devices/Keithley/Keithley2700.h"
#include "devices/Greisinger/GMH3750.h"
#endif

#include "labcontroldatacollector.h"

const int LabControlFileFormat = 1;

LabControlDAQThread::LabControlDAQThread(LabControlDataCollector * collector)
  : dataCollector_(collector)
{

}

void LabControlDAQThread::run()
{
    reading_t theReading = dataCollector_->keithley()->Scan();
    if (!dataCollector_->keithley()->IsScanOk()) return;

    QMutex mutex;
    mutex.lock();
    int channelsBad = 0;
    for (reading_t::const_iterator it = theReading.begin();
         it < theReading.end();
         ++it) {
        if (it->second<-40.0 || it->second>80) {
            channelsBad++;
            continue;
        }

        dataCollector_->data().setTemperature(it->first,
                                              dataCollector_->calibration().calibratedTemperature(it->first,
                                                                                                  it->second));
    }
    mutex.unlock();

    if (channelsBad<dataCollector_->activeChannels())
        dataCollector_->pushData();
}

LabControlHamegStatus::LabControlHamegStatus()
  :statusBits_(0)
{
  setPointVoltage_[0] = -1;
  setPointVoltage_[1] = -1;
  voltage_[0] = -1;
  voltage_[1] = -1;
  currentLimit_[0] = -1;
  currentLimit_[1] = -1;
  current_[0] = -1;
  current_[1] = -1;
}

const LabControlHamegStatus& LabControlHamegStatus::operator=(const LabControlHamegStatus& other)
{
  statusBits_ = other.statusBits_;
  setPointVoltage_[0] = other.setPointVoltage_[0];
  setPointVoltage_[1] = other.setPointVoltage_[1];
  voltage_[0] = other.voltage_[0];
  voltage_[1] = other.voltage_[1];
  currentLimit_[0] = other.currentLimit_[0];
  currentLimit_[1] = other.currentLimit_[1];
  current_[0] = other.current_[0];
  current_[1] = other.current_[1];
  return *this;
}

bool LabControlHamegStatus::operator==(const LabControlHamegStatus& other) const
{
  if (statusBits_!=other.statusBits_) return false;
  if (setPointVoltage_[0]!=other.setPointVoltage_[0]) return false;
  if (setPointVoltage_[1]!=other.setPointVoltage_[1]) return false;
//   if (voltage_[0]!=other.voltage_[0]) return false;
//   if (voltage_[1]!=other.voltage_[1]) return false;
  if (currentLimit_[0]!=other.currentLimit_[0]) return false;
  if (currentLimit_[1]!=other.currentLimit_[1]) return false;
//   if (current_[0]!=other.current_[0]) return false;
//   if (current_[1]!=other.current_[1]) return false;
  return true;
}

LabControlDataCollector::LabControlDataCollector()
  : daqThread_(0),
    julabo_(0),
    hameg8143_(0),
    keithley_(0),
    gmh3750_(0),
    daqFrequency(10)
{

}

LabControlDataCollector::LabControlDataCollector(VJulaboFP50* julabo, VHameg8143* hameg8143)
  : daqThread_(0),
    julabo_(julabo),
    hameg8143_(hameg8143),
    keithley_(0),
    gmh3750_(0),
    daqFrequency(10)
{

}

void LabControlDataCollector::startDAQ(std::ofstream* file, int frequency)
{
    daqFrequency = frequency;
    daqFile_ = file;

    activeChannels_ = 0;
    QString activeChannelString;
    for (unsigned int i=0;i<10;i++) {
        if (activeChannel(i)) {
            if (activeChannels_>0) activeChannelString += ",";
            activeChannelString += QString::number(i);
            activeChannels_++;
            data_.setTemperature(i, 0.0);
        }
    }

    if (activeChannels_>0) {

#ifdef USE_FAKEIO
        keithley_ = new Keithley2700Fake("/dev/ttyS5");
#else
        keithley_ = new Keithley2700("/dev/ttyS5");
#endif

        keithley_->SetActiveChannels(activeChannelString.toStdString());

        std::cout << " [LabControlDataCollector::startDAQ] -- \n"
                << "  using Keithley channels " << activeChannelString.toStdString() << std::endl;
    }

    if (referenceActive()) {
#ifdef USE_FAKEIO
        gmh3750_ = new GMH3750Fake("/dev/ttyS4");
#else
        gmh3750_ = new GMH3750("/dev/ttyS4");
#endif
        std::cout << " [LabControlDataCollector::startDAQ] -- using Greisinger GMH3750" << std::endl;
    }
}

void LabControlDataCollector::stopDAQ()
{
    if (daqThread_) {
        while (daqThread_->isRunning()) usleep(100);
        delete daqThread_;
        daqThread_ = 0;
    }

    if (keithley_) {
        delete keithley_;
        keithley_ = 0;
    }
    if (gmh3750_) {
        delete gmh3750_;
        gmh3750_ = 0;
    }
}

void LabControlDataCollector::collectStatusData()
{
    std::cout << " [LabControlDataCollector::collectStatusData] -- " << std::endl;

    setPumpPressure(julabo_->GetPumpPressure());
    setCirculatorState(julabo_->GetCirculatorStatus());
    data_.setHeatingPower(julabo_->GetHeatingPower());

    // temporary fix to get bath temperature logging in data file correct
    float temperature = julabo_->GetBathTemperature();
    if (temperature!=data_.heatingPower()) data_.setBathTemperture(julabo_->GetBathTemperature());

    data_.setSafetySensorTemperture(julabo_->GetSafetySensorTemperature());

    julaboStatus_ = julabo_->GetStatus();

    lastHamegStatus_ = hamegStatus_;
    if (hameg8143_->DeviceAvailable()) {
        hamegStatus_.setStatusBits(hameg8143_->GetStatus());
        hamegStatus_.setSetPointVoltage(0, hameg8143_->GetSetVoltage(1));
        hamegStatus_.setVoltage(0, hameg8143_->GetVoltage(1));
        hamegStatus_.setCurrent(0, hameg8143_->GetCurrent(1));
        hamegStatus_.setCurrentLimit(0, hameg8143_->GetSetCurrent(1));
        hamegStatus_.setSetPointVoltage(1, hameg8143_->GetSetVoltage(2));
        hamegStatus_.setVoltage(1, hameg8143_->GetVoltage(2));
        hamegStatus_.setCurrent(1, hameg8143_->GetCurrent(2));
        hamegStatus_.setCurrentLimit(1, hameg8143_->GetSetCurrent(2));
    } else {
        hamegStatus_.setStatusBits(0);
        hamegStatus_.setSetPointVoltage(0, 0.0);
        hamegStatus_.setVoltage(0, 0.0);
        hamegStatus_.setCurrent(0, 0.0);
        hamegStatus_.setCurrentLimit(0, 0.0);
        hamegStatus_.setSetPointVoltage(1, 0.0);
        hamegStatus_.setVoltage(1, 0.0);
        hamegStatus_.setCurrent(1, 0.0);
        hamegStatus_.setCurrentLimit(1, 0.0);
    }
}

void LabControlDataCollector::collectData()
{
    std::cout << " [LabControlDataCollector::collectData] -- " << std::endl;

    collectStatusData();

    if (daqThread_ && daqThread_->isRunning()) return;

    if (referenceActive()) {
        double temperature;
        if (gmh3750_->Read(temperature)) {
            data_.setReferenceTemperature(temperature);
        } else {
            std::cout << " [LabControlDataCollector::collectData] -- \n"
                    "  failure reading from Greisinger GMH3750" << std::endl;
            data_.setReferenceTemperature(0.0);

            // do we want this here?
            return;
        }
    }

    if (data_.daqTime()>0 && (data_.daqTime()%daqFrequency)!=0) return;

    if (activeChannels_>0) {
        if (!daqThread_) daqThread_ = new LabControlDAQThread(this);
        std::cout << " [LabControlDataCollector::collectData] -- start DAQ thread" << std::endl;
        daqThread_->start();
    } else {
        std::cout << " [LabControlDataCollector::collectData] -- no active channels" << std::endl;
        pushData();
    }
}

void LabControlDataCollector::pushData()
{
    static int count = 0;

    std::cout << " [LabControlDataCollector::pushData] -- time= " << data_.daqTime() << std::endl;
    dataVec_.push_back(data_);
    std::cout << " [LabControlDataCollector::pushData] -- measurements= " << dataVec_.size() << std::endl;
    writeData(*daqFile_);

    if (hamegStatus_!=lastHamegStatus_) {
      writeHamegStatus(*daqFile_);
    }
    count++;
    if (count==10 || hamegStatus_!=lastHamegStatus_) {
        writeHameg(*daqFile_);
        count = 0;
    }
}

void LabControlDataCollector::daqTimeVector(unsigned int startTime,
                                            unsigned int stopTime,
                                            QVector<double> & vector)
{
    for (std::vector<LabControlData>::const_iterator it = dataVec_.begin();
         it != dataVec_.end();
         ++it) {
        if (startTime<=(*it).daqTime() &&
            stopTime>=(*it).daqTime()) {
            vector.append(static_cast<double>((*it).daqTime()));
        }
    }
}

void LabControlDataCollector::temperatureVector(unsigned int channel,
                                                unsigned int startTime,
                                                unsigned int stopTime,
                                                QVector<double> & vector)
{
    for (std::vector<LabControlData>::const_iterator it = dataVec_.begin();
         it != dataVec_.end();
         ++it) {
        if (startTime<=(*it).daqTime() &&
            stopTime>=(*it).daqTime()) {
            vector.append(static_cast<double>((*it).temperature(channel)));
        }
    }
}

void LabControlDataCollector::referenceTemperatureVector(unsigned int startTime,
                                                         unsigned int stopTime,
                                                         QVector<double> & vector)
{
    for (std::vector<LabControlData>::const_iterator it = dataVec_.begin();
         it != dataVec_.end();
         ++it) {
        if (startTime<=(*it).daqTime() &&
            stopTime>=(*it).daqTime()) {
            vector.append(static_cast<double>((*it).referenceTemperature()));
        }
    }
}

void LabControlDataCollector::bathTemperatureVector(unsigned int startTime,
                                                    unsigned int stopTime,
                                                    QVector<double> & vector)
{
    for (std::vector<LabControlData>::const_iterator it = dataVec_.begin();
         it != dataVec_.end();
         ++it) {
        if (startTime<=(*it).daqTime() &&
            stopTime>=(*it).daqTime()) {
            vector.append(static_cast<double>((*it).bathTemperature()));
        }
    }
}

void LabControlDataCollector::safetySensorTemperatureVector(unsigned int startTime,
                                                            unsigned int stopTime,
                                                            QVector<double> & vector)
{
    for (std::vector<LabControlData>::const_iterator it = dataVec_.begin();
         it != dataVec_.end();
         ++it) {
        if (startTime<=(*it).daqTime() &&
            stopTime>=(*it).daqTime()) {
            vector.append(static_cast<double>((*it).safetySensorTemperature()));
        }
    }
}

void LabControlDataCollector::heatingPowerVector(unsigned int startTime,
                                                 unsigned int stopTime,
                                                 QVector<double> & vector)
{
    for (std::vector<LabControlData>::const_iterator it = dataVec_.begin();
         it != dataVec_.end();
         ++it) {
        if (startTime<=(*it).daqTime() &&
            stopTime>=(*it).daqTime()) {
            vector.append(0.1*static_cast<double>((*it).heatingPower()));
        }
    }
}

void LabControlDataCollector::writeFileHeader(std::ostream &os, const QDateTime & dt)
{
    os << "<HEADER> fileformat " << LabControlFileFormat << std::endl;

    os << "<HEADER> time  " << dt.toString("yyyyMMdd_hhmmss").toStdString() << std::endl;
    os << "<HEADER> utime " << dt.toTime_t() << std::endl;

    int nActiveChannels = 0;
    for (unsigned int i=0;i<10;++i) {
        if (activeChannel(i)) nActiveChannels++;
    }
    os << "<HEADER> activeChannels " << nActiveChannels << "  ";
    for (unsigned int i=0;i<10;++i) {
        if (activeChannel(i)) {
            os << i;
            if (nActiveChannels>1) os << " ";
            nActiveChannels--;
        }
    }
    os << std::endl;

    os << "<HEADER> referenceActive " << (int)referenceActive() << std::endl;
    os << "<STATUSLAYOUT> time<int> Xp<float> Tn<int> Tv<int> workpoint<float> pressure<int> circulator<int>" << std::endl;
    os << "<HAMEGLAYOUT> time<int> bits<int> U1set<float> I1limit<float> U2set<float> I2limit<float>" << std::endl;
    os << "<HAMEGDATALAYOUT> time<int> U1<float> I1<float> U2<float> I2<float>" << std::endl;
    os << "<DATALAYOUT> time<int> [ch0-ch9<float>] [ref<float>] bath<float> safety<float> power<int>" << std::endl;
    os << "<LOGLAYOUT> time<int> log message" << std::endl;
}

void LabControlDataCollector::writeData(std::ostream &os)
{
    os << "<DATA> ";

    os << std::setw(12) << std::right << data().daqTime();

    for (unsigned int i=0;i<10;++i) {
        if (activeChannel_[i]) {
            os << std::setw(8) << std::right << std::fixed << std::setprecision(3);
            os << data().temperature(i);
        }
    }

    if (referenceActive()) {
        os << std::setw(8) << std::right << std::fixed << std::setprecision(3);
        os << data().referenceTemperature();
    }

    os << std::setw(8) << std::right << std::fixed << std::setprecision(2);
    os << data().bathTemperature();

    os << std::setw(8) << std::right << std::fixed << std::setprecision(2);
    os << data().safetySensorTemperature();

    os << std::setw(5) << std::right;
    os << data().heatingPower();

    os << std::endl;
}

void LabControlDataCollector::writeStatus(std::ostream &os)
{
    os << "<STATUS> ";

    os << std::setw(10) << std::right << data().daqTime();

    os << std::setw(8) << std::right << std::fixed << std::setprecision(2) << pidXp();
    os << std::setw(8) << std::right << pidTn();
    os << std::setw(8) << std::right << pidTv();

    os << std::setw(8) << std::right << std::fixed << std::setprecision(2);
    os << workPoint();

    os << std::setw(8) << std::right << pumpPressure();

    os << std::setw(8) << std::right << static_cast<int>(circulatorState());

    os << std::endl;
}

void LabControlDataCollector::writeHamegStatus(std::ostream &os)
{
    os << "<HAMEG>  ";

    os << std::setw(10) << std::right << data().daqTime();

    os << std::setw(8) << std::right << static_cast<int>(hamegStatus().statusBits());

    os << std::setw(8) << std::right << std::fixed << std::setprecision(2) << hamegStatus().setPointVoltage(0);
    os << std::setw(8) << std::right << std::fixed << std::setprecision(3) << hamegStatus().currentLimit(0);
    os << std::setw(8) << std::right << std::fixed << std::setprecision(2) << hamegStatus().setPointVoltage(1);
    os << std::setw(8) << std::right << std::fixed << std::setprecision(3) << hamegStatus().currentLimit(1);

    os << std::endl;
}

void LabControlDataCollector::writeHameg(std::ostream &os)
{
    os << "<HAMEGDATA>  ";

    os << std::setw(10) << std::right << data().daqTime();

    os << std::setw(8) << std::right << std::fixed << std::setprecision(2) << hamegStatus().voltage(0);
    os << std::setw(8) << std::right << std::fixed << std::setprecision(3) << hamegStatus().current(0);
    os << std::setw(8) << std::right << std::fixed << std::setprecision(2) << hamegStatus().voltage(1);
    os << std::setw(8) << std::right << std::fixed << std::setprecision(3) << hamegStatus().current(1);

    os << std::endl;
}
