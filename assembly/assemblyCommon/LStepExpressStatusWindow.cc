#include <nqlogger.h>

#include "LStepExpressStatusWindow.h"

LStepExpressStatusWindow::LStepExpressStatusWindow(LStepExpressModel* model,
                                       QWidget *parent)
    : QPlainTextEdit(parent),
      model_(model)
{
    this->setReadOnly(true);
    //    this->setVisible(true);

    timer_ = new QTimer(this);
    timer_->setInterval(1000);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateStatus()));
    connect(model_, SIGNAL(deviceStateChanged(State)), this, SLOT(startTimer(State)));
}

LStepExpressStatusWindow::~LStepExpressStatusWindow()
{
    if(timer_){delete timer_; timer_ = NULL;}
}

void LStepExpressStatusWindow::startTimer(State state)
{
    if(state == READY){
        timer_->start();
    }else{
        timer_->stop();
    }
}

void LStepExpressStatusWindow::updateStatus()
{
    bool status;
    model_->getStatus(status);
    int error;
    model_->getError(error);
    std::string value;
    model_->getSystemStatus(value);

    QString statustext = "Controller status is ";
    if(status){statustext.append("OK");}
    else{statustext.append("in ERROR");}
    QString systemstatustext = "System status is ";
    systemstatustext.append(QString::fromStdString(value));
    QString errortext = "Error state is ";
    errortext.append(error);
    this->appendPlainText(statustext);

    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());
}

