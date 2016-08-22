#include <QApplication>

#include <nqlogger.h>

#include "LStepExpressMotionManager.h"

LStepExpressMotionManager::LStepExpressMotionManager(LStepExpressModel* model, QObject *parent)
    : QObject(parent),
      model_(model),
      inMotion_(false)
{
    connect(model_, SIGNAL(motionStarted()),
            this, SLOT(motionStarted()));

    connect(model_, SIGNAL(motionFinished()),
            this, SLOT(motionFinished()));

    connect(this, SIGNAL(signalMoveAbsolute(double,double,double,double)),
            model_, SLOT(moveAbsolute(double,double,double,double)));

    connect(this, SIGNAL(signalMoveRelative(double,double,double,double)),
            model_, SLOT(moveRelative(double,double,double,double)));

    //spySignalMoveAbsolute = new QSignalSpy(this, SIGNAL(signalMoveAbsolute(double,double,double,double)));
    //spySignalMoveRelative = new QSignalSpy(this, SIGNAL(signalMoveRelative(double,double,double,double)));

    /*
    connect(this, SIGNAL(signalMoveAbsolute(double,double,double,double)),
            this, SLOT(printSpyInformation()));

    connect(this, SIGNAL(signalMoveRelative(double,double,double,double)),
            this, SLOT(printSpyInformation()));
    */
}

LStepExpressMotionManager::~LStepExpressMotionManager()
{
  std::cout<<"destructor lstepexpressmotionmanager"<<std::endl;
  //    if(spySignalMoveAbsolute){delete spySignalMoveAbsolute; spySignalMoveAbsolute = NULL;}
  //if(spySignalMoveRelative){delete spySignalMoveRelative; spySignalMoveRelative = NULL;}
}

void LStepExpressMotionManager::printSpyInformation()
{
  /*
    for(int i = 0; i < spySignalMoveAbsolute->size(); i++){
      NQLog("SPY LStepExpressMotionManager ", NQLog::Debug)<< "this_, signal signalMoveAbsolute()";
    }
    spySignalMoveAbsolute->clear();
    for(int i = 0; i < spySignalMoveRelative->size(); i++){
        NQLog("SPY LStepExpressMotionManager ", NQLog::Debug)<< "this_, signal signalMoveRelative()";
    }
    spySignalMoveRelative->clear();
  */
}

void LStepExpressMotionManager::run()
{
    if (inMotion_) return;

    if (motions_.empty()) return;

    //    NQLog("LStepExpressMotionManager", NQLog::Debug) << "run";

    LStepExpressMotion motion = motions_.dequeue();

    inMotion_ = true;

    if (motion.getMode()==true) {
        emit signalMoveAbsolute(motion.getX(), motion.getY(), motion.getZ(), motion.getA());
    } else {
        emit signalMoveRelative(motion.getX(), motion.getY(), motion.getZ(), motion.getA());
    }
}

void LStepExpressMotionManager::myMoveToThread(QThread *thread)
{
    model_->moveToThread(thread);
    this->moveToThread(thread);
}

void LStepExpressMotionManager::appendMotion(const LStepExpressMotion& motion)
{
    motions_.enqueue(motion);
    run();
}

void LStepExpressMotionManager::appendMotions(const QQueue<LStepExpressMotion>& motions)
{
    motions_.append(motions);
    run();
}

void LStepExpressMotionManager::moveRelative(std::vector<double> & values)
{
    motions_.enqueue(LStepExpressMotion(values, false));
    run();
}

void LStepExpressMotionManager::moveRelative(double x, double y, double z, double a)
{
    motions_.enqueue(LStepExpressMotion(x, y, z, a, false));
    run();
}

void LStepExpressMotionManager::moveRelative(unsigned int axis, double value)
{
    motions_.enqueue(LStepExpressMotion(axis, value, false));
    run();
}

void LStepExpressMotionManager::moveAbsolute(std::vector<double> & values)
{
    motions_.enqueue(LStepExpressMotion(values, true));
    run();
}

void LStepExpressMotionManager::moveAbsolute(double x, double y, double z, double a)
{
    motions_.enqueue(LStepExpressMotion(x, y, z, a, true));
    run();
}

void LStepExpressMotionManager::moveAbsolute(unsigned int axis, double value)
{
    motions_.enqueue(LStepExpressMotion(axis, value, true));
    run();
}

void LStepExpressMotionManager::motionStarted()
{
  // NQLog("LStepExpressMotionManager", NQLog::Debug) << "motionStarted()";
    inMotion_ = true;
}

void LStepExpressMotionManager::motionFinished()
{
  //    NQLog("LStepExpressMotionManager", NQLog::Debug) << "motionFinished()";
    inMotion_ = false;

    this->run();
}
