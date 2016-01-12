#ifndef LSTEPEXPRESSMOTIONMANAGER_H
#define LSTEPEXPRESSMOTIONMANAGER_H

#include <QQueue>

#include "LStepExpressModel.h"

class LStepExpressMotion
{
public:

    LStepExpressMotion(std::vector<double> & values, bool mode);
    LStepExpressMotion(double x, double y, double z, double a, bool mode);
    LStepExpressMotion(unsigned int axis, double value, bool mode);

    bool getMode() const { return mode_; }
    double getX() const { return x_; }
    double getY() const { return y_; }
    double getZ() const { return z_; }
    double getA() const { return a_; }

protected:

    bool mode_;
    double x_, y_, z_, a_;
};

class LStepExpressMotionManager : public QObject
{
    Q_OBJECT

public:

    explicit LStepExpressMotionManager(LStepExpressModel* model, QObject *parent = 0);

    void myMoveToThread(QThread *thread);

public slots:

    void moveRelative(std::vector<double> & values);
    void moveRelative(double x, double y, double z, double a);
    void moveRelative(unsigned int axis, double value);
    void moveAbsolute(std::vector<double> & values);
    void moveAbsolute(double x = 0.0, double y = 0.0, double z = 0.0, double a = 0.0);
    void moveAbsolute(unsigned int axis, double value);

protected:

    void run();

    LStepExpressModel* model_;
    QQueue<LStepExpressMotion> motions_;
    bool inMotion_;

protected slots:

    void motionStarted();
    void motionFinished();

signals:

    void signalMoveRelative(double x, double y, double z, double a);
    void signalMoveAbsolute(double x, double y, double z, double a);
};

#endif // LSTEPEXPRESSMOTIONMANAGER_H
