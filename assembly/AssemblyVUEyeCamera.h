#ifndef ASSEMBLYVUEYECAMERA_H
#define ASSEMBLYVUEYECAMERA_H

#include <QObject>

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

    unsigned int getCameraID() const { return cameraID_; }
    unsigned int getDeviceID() const { return deviceID_; }
    unsigned int getSensorID() const { return sensorID_; }
    unsigned long getSerialNumber() const { return serialNumber_; }
    const QString& getModelName() const { return modelName_; }
    const QString& getFullModelName() const { return fullModelName_; }
    unsigned int getStatus() const { return status_; }

    virtual void updateInformation() = 0;
    virtual bool isAvailable() const = 0;

    virtual bool initialize() = 0;
    virtual bool exit() = 0;

public slots:

    virtual void aquireImage() = 0;

protected slots:

protected:

    unsigned int cameraID_;
    unsigned int deviceID_;
    unsigned int sensorID_;
    unsigned long serialNumber_;
    QString modelName_;
    QString fullModelName_;
    unsigned int status_;


signals:

private:

};

#endif // ASSEMBLYVUEYECAMERA_H
