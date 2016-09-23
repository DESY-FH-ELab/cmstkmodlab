#ifndef LSTEPEXPRESSMODEL_H
#define LSTEPEXPRESSMODEL_H

#include <cmath>
#include <vector>

#include <QString>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QSignalSpy>

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

  

    QMutex mutex_;

    /// Time interval between cache refreshes; in milliseconds.
    const int updateInterval_;
    const int motionUpdateInterval_;
    QTimer* timer_;
    QTimer* spyTimer_;
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

    QSignalSpy *spyDeviceStateChanged;
    QSignalSpy *spyInformationChanged;
    QSignalSpy *spyMotionInformationChanged;
    QSignalSpy *spyMessage;
    QSignalSpy *spyControlStateChanged;
    QSignalSpy *spyMotionStarted;
    QSignalSpy *spyMotionFinished;
    QSignalSpy *spyTimer;

protected slots:

    void updateInformation();
    void updateMotionInformation();
    void updateMotionInformationFromTimer();
    void printSpyInformation();

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
