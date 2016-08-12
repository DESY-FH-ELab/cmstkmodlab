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

    connect(this, SIGNAL(signalMoveAbsolute(double,double,double,double)),
            this, SLOT(printSpyInformation()));

    connect(this, SIGNAL(signalMoveRelative(double,double,double,double)),
            this, SLOT(printSpyInformation()));

    spySignalMoveAbsolute = new QSignalSpy(this, SIGNAL(signalMoveAbsolute(double,double,double,double)));
    spySignalMoveRelative = new QSignalSpy(this, SIGNAL(signalMoveRelative(double,double,double,double)));
    
}

LStepExpressMotionManager::~LStepExpressMotionManager()
{
    if(spySignalMoveAbsolute){delete spySignalMoveAbsolute; spySignalMoveAbsolute = NULL;}
    if(spySignalMoveRelative){delete spySignalMoveRelative; spySignalMoveRelative = NULL;}
}

void LStepExpressMotionManager::printSpyInformation()
{

    for(int i = 0; i < spySignalMoveAbsolute->size(); i++){
        std::cout<<"SPY LStepExpressMotionManager "<< "this_, signal signalMoveAbsolute()"<<std::endl;
    }
    spySignalMoveAbsolute->clear();
    for(int i = 0; i < spySignalMoveRelative->size(); i++){
        std::cout<<"SPY LStepExpressMotionManager "<< "this_, signal signalMoveRelative()"<<std::endl;
    }
    spySignalMoveRelative->clear();
}

void LStepExpressMotionManager::run()
{
    std::cout<<"LStepExpressMotionManager "<< "run"<<std::endl;

    if (inMotion_) return;

    std::cout<<"LStepExpressMotionManager "<< "run in motion"<<std::endl;

    if (motions_.empty()) return;

    std::cout<<"LStepExpressMotionManager "<< "run in motion, not empty"<<std::endl;

    LStepExpressMotion motion = motions_.dequeue();

    inMotion_ = true;

    if (motion.getMode()==true) {
        std::cout<<"LStepExpressMotionManager "<< "run, move absolute"<<std::endl;
        emit signalMoveAbsolute(motion.getX(), motion.getY(), motion.getZ(), motion.getA());
    } else {
            std::cout<<"LStepExpressMotionManager "<< "run, move relative"<<std::endl;
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
  std::cout<<"LStepExpressMotionManager "<< "appendMotion"<<std::endl;
    motions_.enqueue(motion);
    run();
}

void LStepExpressMotionManager::appendMotions(const QQueue<LStepExpressMotion>& motions)
{
  std::cout<<"LStepExpressMotionManager "<< "appendMotions"<<std::endl;
    motions_.append(motions);
    run();
}

void LStepExpressMotionManager::moveRelative(std::vector<double> & values)
{
    std::cout<<"LStepExpressMotionManager "<< "moveRelative() v1"<<std::endl;
    motions_.enqueue(LStepExpressMotion(values, false));
    run();
}

void LStepExpressMotionManager::moveRelative(double x, double y, double z, double a)
{
    std::cout<<"LStepExpressMotionManager "<< "moveRelative() v2"<<std::endl;
    motions_.enqueue(LStepExpressMotion(x, y, z, a, false));
    run();
}

void LStepExpressMotionManager::moveRelative(unsigned int axis, double value)
{
    std::cout<<"LStepExpressMotionManager "<< "moveRelative() v3"<<std::endl;
    motions_.enqueue(LStepExpressMotion(axis, value, false));
    run();
}

void LStepExpressMotionManager::moveAbsolute(std::vector<double> & values)
{
    std::cout<<"LStepExpressMotionManager "<< "moveAbsolute() v1"<<std::endl;
    motions_.enqueue(LStepExpressMotion(values, true));
    run();
}

void LStepExpressMotionManager::moveAbsolute(double x, double y, double z, double a)
{
    std::cout<<"LStepExpressMotionManager "<< "moveAbsolute() v2"<<std::endl;
    motions_.enqueue(LStepExpressMotion(x, y, z, a, true));
    run();
}

void LStepExpressMotionManager::moveAbsolute(unsigned int axis, double value)
{
  std::cout<<"LStepExpressMotionManager "<< "moveAbsolute() v3"<<std::endl;
    motions_.enqueue(LStepExpressMotion(axis, value, true));
    run();
}

void LStepExpressMotionManager::motionStarted()
{
    std::cout<<"LStepExpressMotionManager "<< "motionStarted()"<<std::endl;
    inMotion_ = true;
}

void LStepExpressMotionManager::motionFinished()
{
    std::cout<<"LStepExpressMotionManager "<< "motionFinished()"<<std::endl;
    inMotion_ = false;

    this->run();
}
