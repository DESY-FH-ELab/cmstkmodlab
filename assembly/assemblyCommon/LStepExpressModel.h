/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef LSTEPEXPRESSMODEL_H
#define LSTEPEXPRESSMODEL_H

#include <vector>

#include <QString>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>

#include "DeviceState.h"
#include "DeviceParameter.h"

#ifdef USE_FAKEIO
#include "devices/Lang/LStepExpressFake.h"
typedef LStepExpressFake LStepExpress_t;
#else
#include "devices/Lang/LStepExpress.h"
typedef LStepExpress LStepExpress_t;
#endif

class LStepExpressModel : public QObject, public AbstractDeviceModel<LStepExpress_t>
{
 Q_OBJECT

  public:

    explicit LStepExpressModel(const char* port, int updateInterval=1000, int motionUpdateInterval=100, QObject *parent=0);
    ~LStepExpressModel();

    bool isUpdating() { return isUpdating_; }
    void pauseUpdate();
    void continueUpdate();

    void initialize();

    QString getAxisName(unsigned int axis);
    QString getAxisDimensionShortName(unsigned int axis);
    QString getAxisStatusText(unsigned int axis);
    bool getAxisEnabled(unsigned int axis);
    bool getAxisState(unsigned int axis);
    double getPosition(unsigned int axis);

    bool isInMotion() const { return inMotion_; }

    bool getJoystickEnabled();
    bool getJoystickAxisEnabled(unsigned int axis);

    void setValue(const QString & command, const QString & value);
    void getValue(const QString & command, QString & value);

    void getStatus(bool& status);
    void getError(int& error);
    void getSystemStatus(std::string& value);

    void validConfig();
    void validParameter();
    void saveConfig();
    void reset();

  public slots:

    void setDeviceEnabled(bool enabled=true);
    void setControlsEnabled(bool enabled);
    void setAxisEnabled(unsigned int axis, bool enabled);
    void setJoystickEnabled(bool enabled);
    void setJoystickAxisEnabled(unsigned int axis, bool enabled);

    void moveRelative(const std::vector<double>& values);
    void moveRelative(const double x, const double y, const double z, const double a);
    void moveRelative(const unsigned int axis, const double value);
    void moveAbsolute(const std::vector<double> & values);
    void moveAbsolute(const double x=0., const double y=0., const double z=0., const double a=0.);
    void moveAbsolute(const unsigned int axis, const double value);

    void calibrate();

    void emergencyStop();

  protected:

    const QString LStepExpress_PORT;

    QMutex mutex_;

    /// Time interval between cache refreshes; in milliseconds.
    const int updateInterval_;
    const int motionUpdateInterval_;
    QTimer* timer_;
    int updateCount_;

    void setDeviceState( State state );

    std::vector<int> axis_;
    std::vector<int> axisDirection_;
    std::vector<int> dim_;
    std::vector<int> pa_;
    int joystickEnabled_;
    std::vector<int> joystickAxisEnabled_;

    std::vector<int> axisStatus_;
    std::vector<double> position_;

    bool inMotion_;
    bool isPaused_;
    bool isUpdating_;
    bool finishedCalibrating_;

  protected slots:

    void updateInformation();
    void updateMotionInformation();
    void updateMotionInformationFromTimer();

  signals:

    void deviceStateChanged(State newState);
    void informationChanged();
    void motionInformationChanged();
    void message(const QString & text);
    void controlStateChanged(bool);

    void motionStarted();
    void motionFinished();
    void emergencyStopSignal();
};

#endif // LSTEPEXPRESSMODEL_H
