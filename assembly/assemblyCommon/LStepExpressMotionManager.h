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

#include <LStepExpressModel.h>
#include <LStepExpressMotion.h>

class LStepExpressMotionManager : public QObject
{
 Q_OBJECT

  public:

    explicit LStepExpressMotionManager(LStepExpressModel*, QObject* parent=0);
    virtual ~LStepExpressMotionManager();

    void myMoveToThread(QThread*);

    bool is_connected() const { return is_connected_; }

    double get_position_X() const { return model_->getPosition(0); }
    double get_position_Y() const { return model_->getPosition(1); }
    double get_position_Z() const { return model_->getPosition(2); }
    double get_position_A() const { return model_->getPosition(3); }

  public slots:

    void  enable_motion();
    void disable_motion();

    void appendMotion(const LStepExpressMotion& motion);
    void appendMotions(const QQueue<LStepExpressMotion>& motions);
    void moveRelative(const std::vector<double>& values);
    void moveRelative(double x, double y, double z, double a);
    void moveRelative(unsigned int axis, double value);
    void moveAbsolute(std::vector<double>& values);
    void moveAbsolute(double x=0.0, double y=0.0, double z=0.0, double a=0.0);
    void moveAbsolute(unsigned int axis, double value);

  protected:

    void run();

    LStepExpressModel* model_;
    QQueue<LStepExpressMotion> motions_;
    bool inMotion_;

    bool is_connected_;

  protected slots:

    void start_motion();
    void finish_motion();

  signals:

    void motion_finished();

    void signalMoveRelative(double x, double y, double z, double a);
    void signalMoveAbsolute(double x, double y, double z, double a);
};

#endif // LSTEPEXPRESSMOTIONMANAGER_H
