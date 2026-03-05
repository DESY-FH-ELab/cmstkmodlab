/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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

    void setLaserHead(int out);
    void setMeasurement(double value); //dummy method for testing

public slots:

    void setDeviceEnabled(bool enabled = true);
    void getMeasurement(double& value);
    void setSamplingRate(int mode);
    void setAveraging(int mode);
    void setDiffuseMode(int mode);
    void setMaterialMode(int mode);

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
