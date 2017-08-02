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

#ifndef LSTEPEXPRESSMOTIONMANAGER_H
#define LSTEPEXPRESSMOTIONMANAGER_H

#include <QQueue>

#include "LStepExpressModel.h"
#include "LStepExpressMotion.h"

class LStepExpressMotionManager : public QObject
{
    Q_OBJECT

public:

    explicit LStepExpressMotionManager(LStepExpressModel* model, QObject *parent = 0);
    ~LStepExpressMotionManager();

    void myMoveToThread(QThread *thread);

public slots:

    void appendMotion(const LStepExpressMotion& motion);
    void appendMotions(const QQueue<LStepExpressMotion>& motions);
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
