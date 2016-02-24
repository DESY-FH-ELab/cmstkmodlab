#ifndef ASSEMBLYVUEYECAMERA_H
#define ASSEMBLYVUEYECAMERA_H

#include <opencv2/opencv.hpp>

#include <QObject>

#include <DeviceState.h>

class AssemblyVUEyeCamera : public QObject
{
    Q_OBJECT
public:
    explicit AssemblyVUEyeCamera(QObject *parent);
    ~AssemblyVUEyeCamera();

    void setCameraID(unsigned int id) { cameraID_ = id; }
    void setDeviceID(unsigned int id) { deviceID_ = id; }
    void setSensorID(unsigned int id) { sensorID_ = id; }
    void setSerialNumber(unsigned long serNo) { serialNumber_ = serNo; }
    void setModelName(const QString& name) { modelName_ = name; }
    void setFullModelName(const QString& name) { fullModelName_ = name; }
    void setStatus(unsigned int status) { status_ = status; }

    void setID(const QString& id) { id_ = id; }
    void setVersion(const QString& version) { version_ = version; }
    void setDate(const QString& date) { date_ = date; }

    void setSensorName(const QString& name) { sensorName_ = name; }
    void setColorMode(unsigned int colorMode) { colorMode_ = colorMode; }
    void setMaxWidth(unsigned int maxWidth) { maxWidth_ = maxWidth; }
    void setMaxHeight(unsigned int maxHeight) { maxHeight_ = maxHeight; }
    void setMasterGain(bool masterGain) { masterGain_ = masterGain; }
    void setRedGain(bool gain) { redGain_ = gain; }
    void setGreenGain(bool gain) { greenGain_ = gain; }
    void setBlueGain(bool gain) { blueGain_ = gain; }
    void setGlobalShutter(bool globalShutter) { globalShutter_ = globalShutter; }
    void setPixelSize(unsigned int pixelSize) { pixelSize_ = pixelSize; }

    unsigned int getCameraID() const { return cameraID_; }
    unsigned int getDeviceID() const { return deviceID_; }
    unsigned int getSensorID() const { return sensorID_; }
    unsigned long getSerialNumber() const { return serialNumber_; }
    const QString& getModelName() const { return modelName_; }
    const QString& getFullModelName() const { return fullModelName_; }
    unsigned int getStatus() const { return status_; }

    const QString& getID() const { return id_; }
    const QString& getVersion() const { return version_; }
    const QString& getDate() const { return date_; }

    const QString& getSensorName() const { return sensorName_; }
    unsigned int getColorMode() const { return colorMode_; }
    unsigned int getMaxWidth() const { return maxWidth_; }
    unsigned int getMaxHeight() const { return maxHeight_; }
    bool getMasterGain() const { return masterGain_; }
    bool getRedGain() const { return redGain_; }
    bool getGreenGain() const { return greenGain_; }
    bool getBlueGain() const { return blueGain_; }
    bool getGlobalShutter() const { return globalShutter_; }
    unsigned int getPixelSize() const { return pixelSize_; }

    size_t getNumberOfPixelClocks() const { return pixelClocks_.size(); }
    const std::vector<unsigned int>& getPixelClockList() const { return pixelClocks_; }
    unsigned int getCurrentPixelClock() const { return currentPixelClock_; }

    double getExposureTimeMin() const { return exposureTimeMin_; }
    double getExposureTimeMax() const { return exposureTimeMax_; }
    double getExposureTimeInc() const { return exposureTimeInc_; }
    double getCurrentExposureTime() const { return currentExposureTime_; }

    virtual void updateInformation() = 0;
    virtual void updatePixelClock() = 0;
    virtual void updateExposureTime() = 0;

    virtual bool isAvailable() const = 0;
    virtual bool isOpen() const { return (cameraState_==State::READY); }
    State getDeviceState() const { return cameraState_; }

public slots:

    virtual void open() = 0;
    virtual void close() = 0;

    virtual void acquireImage() = 0;

    virtual void setPixelClock(unsigned int) = 0;
    virtual void setExposureTime(double) = 0;
    virtual  void calibrateSettings();

protected slots:

protected:

    unsigned int cameraID_;
    unsigned int deviceID_;
    unsigned int sensorID_;
    unsigned long serialNumber_;
    QString modelName_;

    QString fullModelName_;
    unsigned int status_;

    QString id_;
    QString version_;
    QString date_;

    QString sensorName_;
    unsigned int colorMode_;
    unsigned int maxWidth_;
    unsigned int maxHeight_;
    bool masterGain_;
    bool redGain_;
    bool greenGain_;
    bool blueGain_;
    bool globalShutter_;
    unsigned int pixelSize_;

    std::vector<unsigned int> pixelClocks_;
    unsigned int currentPixelClock_;
    size_t getCurrentPixelClockIndex() const;

    double exposureTimeMin_;
    double exposureTimeMax_;
    double exposureTimeInc_;
    double currentExposureTime_;

    State cameraState_;

signals:

    void cameraInformationChanged();

    void cameraOpened();
    void cameraClosed();

    void pixelClockChanged(unsigned int);
    void pixelClockListChanged(unsigned int);

    void exposureTimeChanged(double);
    void exposureTimeRangeChanged(double);

    void imageAcquired(const cv::Mat&);

private:

};

#endif // ASSEMBLYVUEYECAMERA_H
