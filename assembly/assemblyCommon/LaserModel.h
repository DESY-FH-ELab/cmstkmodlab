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

    void setSamplingRate(int mode);
    void setAveraging(int mode);
    void setLaserHead(int out);
    void setMeasurement(double value); //dummy method for testing

public slots:

    void getMeasurement();

protected:

    const QString Laser_PORT;
    
    void initialize();
    
    QMutex mutex_;
    
    void setDeviceState( State state );
    
    int laserHead_;

protected slots:
        
signals:
    
    void deviceStateChanged(State newState);
    void measurementChanged(double value);
};

#endif // LASERMODEL_H
