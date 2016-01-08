#ifndef LSTEPEXPRESSMODEL_H
#define LSTEPEXPRESSMODEL_H

#include <cmath>
#include <vector>

#include <QString>
#include <QTimer>

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

    QString getAxisName(unsigned int axis);
    QString getAxisStatusText(unsigned int axis);
    bool getAxisEnabled(unsigned int axis);
    bool getAxisState(unsigned int axis);
    double getPosition(unsigned int axis);

    bool isInMotion() const { return inMotion_; }

    void moveRelative(std::vector<double> & values);

    bool getJoystickEnabled();
    bool getJoystickAxisEnabled(unsigned int axis);

public slots:

    void setDeviceEnabled(bool enabled = true);
    void setControlsEnabled(bool enabled);
    void setAxisEnabled(unsigned int axis, bool enabled);
    void setJoystickEnabled(bool enabled);
    void setJoystickAxisEnabled(unsigned int axis, bool enabled);

protected:

    const QString LStepExpress_PORT;

    void initialize();

    /// Time interval between cache refreshes; in milliseconds.
    const int updateInterval_;
    const int motionUpdateInterval_;
    QTimer* timer_;
    QTimer* motionTimer_;

    void setDeviceState( State state );

    std::vector<int> axis_;
    std::vector<int> axisDirection_;
    std::vector<int> dim_;
    std::vector<int> pa_;
    bool joystickEnabled_;
    std::vector<int> joystickAxisEnabled_;

    std::vector<int> axisStatus_;
    std::vector<double> position_;

    bool inMotion_;

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
