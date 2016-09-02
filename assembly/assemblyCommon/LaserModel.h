#ifndef LASERMODEL_H
#define LASERMODEL_H

#include <cmath>
#include <vector>

#include <QString>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Keyence/KeyenceFake.h"
typedef KeyenceFake Keyence_t;
#else
#include "devices/Keyence/Keyence.h"
typedef Keyence Keyence_t;
#endif

class LaserModel : public QObject, public AbstractDeviceModel<Keyence_t>
{
    Q_OBJECT

public:

    explicit LaserModel(const char* port,
                               QObject *parent = 0);
    ~LaserModel();

    void setSamplingRate(int mode);
    void setAveraging(int mode);
    void setLaserHead(int out);
    void setMeasurement(double value); //dummy method for testing

public slots:

    void setDeviceEnabled(bool enabled = true);
    void getMeasurement(double& value);

protected:

    const QString Laser_PORT;
    
    void initialize();
    
    QMutex mutex_;
    QTimer* timer_;
    
    void setDeviceState( State state );
    
    int laserHead_;

    double value_;
    bool isInRange_;

protected slots:

    void updateInformation();
        
signals:
    
    void deviceStateChanged(State newState);
    void measurementChanged(double value);
    void inRangeStateChanged(bool inRange);
};

#endif // LASERMODEL_H
