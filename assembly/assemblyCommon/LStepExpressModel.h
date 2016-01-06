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
    bool getAxisEnabled(unsigned int axis);
    bool getAxisState(unsigned int axis);
    double getPosition(unsigned int axis);
    void moveRelative(std::vector<double> & values);

public slots:

    void setDeviceEnabled(bool enabled = true);
    void setControlsEnabled(bool enabled);
    void setAxisEnabled(unsigned int axis, bool enabled);

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

    std::vector<int> axisStatus_;
    std::vector<double> position_;

protected slots:

    void updateInformation();
    void updateMotionInformation();

signals:

    void deviceStateChanged(State newState);
    void informationChanged();
    void motionInformationChanged();
    void message(const QString & text);
    void controlStateChanged(bool);
};

#endif // LSTEPEXPRESSMODEL_H
