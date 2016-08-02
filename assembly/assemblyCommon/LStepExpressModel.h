#ifndef LSTEPEXPRESSMODEL_H
#define LSTEPEXPRESSMODEL_H

#include <cmath>
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

    explicit LStepExpressModel(const char* port,
                               int updateInterval = 1000,
                               int motionUpdateInterval = 100,
                               QObject *parent = 0);

    bool isUpdating() { return isUpdating_; }
    void pauseUpdate();
    void continueUpdate();

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

    void validConfig();
    void validParameter();
    void saveConfig();
    void reset();

public slots:

    void setDeviceEnabled(bool enabled = true);
    void setControlsEnabled(bool enabled);
    void setAxisEnabled(unsigned int axis, bool enabled);
    void setJoystickEnabled(bool enabled);
    void setJoystickAxisEnabled(unsigned int axis, bool enabled);

    void moveRelative(std::vector<double> & values);
    void moveRelative(double x, double y, double z, double a);
    void moveRelative(unsigned int axis, double value);
    void moveAbsolute(std::vector<double> & values);
    void moveAbsolute(double x = 0.0, double y = 0.0, double z = 0.0, double a = 0.0);
    void moveAbsolute(unsigned int axis, double value);
    void calibrate();
    void emergencyStop();

protected:

    const QString LStepExpress_PORT;

    void initialize();

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

signals:

    void deviceStateChanged(State newState);
    void informationChanged();
    void motionInformationChanged();
    void message(const QString & text);
    void controlStateChanged(bool);

    void motionStarted();
    void motionFinished();
};

#endif // LSTEPEXPRESSMODEL_H
