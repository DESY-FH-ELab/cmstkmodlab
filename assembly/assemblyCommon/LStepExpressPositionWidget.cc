#include <iostream>
#include <nqlogger.h>
#include "ApplicationConfig.h"
#include "LStepExpressPositionWidget.h"

LStepExpressPositionWidget::LStepExpressPositionWidget(LStepExpressModel* model, QWidget *parent)
    : QWidget(parent),
      model_(model)
{
    position_ = std::vector<double>{0.0,0.0,0.0,0.0};

    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);

    //    QVBoxLayout* layoutLabels = new QVBoxLayout(this);
    QLabel *GoToLabel_ = new QLabel("Go To:",this);
    layout->addWidget(GoToLabel_);

    // Add all the axes displays                                                                                                                                        
    //    for (unsigned int i=0;i<4;++i) {
    xpos_ = new LStepExpressPositionAxisWidget(model_, 0, this);
    ypos_ = new LStepExpressPositionAxisWidget(model_, 1, this);
    zpos_ = new LStepExpressPositionAxisWidget(model_, 2, this);
    apos_ = new LStepExpressPositionAxisWidget(model_, 3, this);
    layout->addWidget(xpos_);
    layout->addWidget(ypos_);
    layout->addWidget(zpos_);
    layout->addWidget(apos_);
        //    }

    moveAbsoluteButton_ = new QPushButton("Move Absolute", this);
    layout->addWidget(moveAbsoluteButton_);

    moveRelativeButton_ = new QPushButton("Move Relative", this);
    layout->addWidget(moveRelativeButton_);

    connect(moveAbsoluteButton_, SIGNAL(clicked()),
	this, SLOT(moveAbsoluteButtonClicked()));
    connect(moveRelativeButton_, SIGNAL(clicked()),
	this, SLOT(moveRelativeButtonClicked()));
    connect(model_, SIGNAL(deviceStateChanged(State)),
            this, SLOT(lStepStateChanged(State)));

    connect(xpos_, SIGNAL(positionChanged(double, unsigned int)),
	this, SLOT(positionChanged(double, unsigned int)));
    connect(ypos_, SIGNAL(positionChanged(double, unsigned int)),
	this, SLOT(positionChanged(double, unsigned int)));
    connect(zpos_, SIGNAL(positionChanged(double, unsigned int)),
	this, SLOT(positionChanged(double, unsigned int)));
    connect(apos_, SIGNAL(positionChanged(double, unsigned int)),
	this, SLOT(positionChanged(double, unsigned int)));

    lStepStateChanged(model_->getDeviceState());

    spyMoveAbsoluteButton_ = new QSignalSpy(moveAbsoluteButton_, SIGNAL(clicked()));
    spyMoveRelativeButton_ = new QSignalSpy(moveRelativeButton_, SIGNAL(clicked()));
    connect(moveAbsoluteButton_, SIGNAL(clicked()),
	this, SLOT(printSpyInformation()));
    connect(moveRelativeButton_, SIGNAL(clicked()),
	this, SLOT(printSpyInformation()));
}

void LStepExpressPositionWidget::printSpyInformation()
{
    for(int i = 0; i < spyMoveAbsoluteButton_->size(); i++){
        std::cout<<"SPY LStepExpressPositionWidget " << "moveAbsoluteBotton_, signal clicked()"<<std::endl;
    }
    spyMoveAbsoluteButton_->clear();
    for(int i = 0; i < spyMoveRelativeButton_->size(); i++){
        std::cout<<"SPY LStepExpressPositionWidget " << "moveRelativeBotton_, signal clicked()"<<std::endl;
    }
    spyMoveRelativeButton_->clear();
}

void LStepExpressPositionWidget::lStepStateChanged( State newState)
{
    std::cout<<"LStepExpressPositionWidget " << "lStepStateChanged, newstate = "<<newState<<std::endl;
    if(newState == READY || newState == INITIALIZING){
        bool enabled = false;
        for(int i = 0; i < 4; i++){ enabled = model_->getAxisEnabled(i); if(enabled) break; }
        moveAbsoluteButton_->setEnabled(enabled);
        moveRelativeButton_->setEnabled(enabled);
    }else{
        std::cout<<"LStepExpressPositionWidget " << "lStepStateChanged, set enabled false"<<std::endl;
        moveAbsoluteButton_->setEnabled(false);
        moveRelativeButton_->setEnabled(false);
    }
}

void LStepExpressPositionWidget::moveAbsoluteButtonClicked()
{
    model_->moveAbsolute(position_);
}

void LStepExpressPositionWidget::moveRelativeButtonClicked()
{
    model_->moveRelative(position_);
}

void LStepExpressPositionWidget::positionChanged(double value, unsigned int axis)
{
    (position_)[axis] = value;
}

LStepExpressPositionAxisWidget::LStepExpressPositionAxisWidget(LStepExpressModel* model, unsigned int axis, QWidget *parent)
    : QWidget(parent),
      model_(model),
      axis_(axis),
      axisDimensionName_("usteps")
{
    position_ = 0.0;

    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    
    QLabel *Label_ = new QLabel(model_->getAxisName(axis_), this);
    Label_->setFont(QFont("Helvetica", 32));
    layout->addWidget(Label_);

    Edit_ = new QLineEdit("0.0", this);
    layout->addWidget(Edit_);

    Unit_ = new QLabel(axisDimensionName_, this);
    layout->addWidget(Unit_);

    connect(Edit_, SIGNAL(textChanged(QString)),
	this, SLOT(updateWidgets()));
    connect(model_, SIGNAL(informationChanged()),
            this, SLOT(updateWidgets()));

    spyEdit_ = new QSignalSpy(Edit_, SIGNAL(textChanged(QString)));
    spyPositionChanged_ = new QSignalSpy(this, SIGNAL(positionChanged()));
    
    connect(Edit_, SIGNAL(textChanged(QString)),
	this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(positionChanged()),
	this, SLOT(printSpyInformation()));
}

void LStepExpressPositionAxisWidget::printSpyInformation()
{
    for(int i = 0; i < spyEdit_->size(); i++){
        std::cout<<"SPY LStepExpressPositionAxisWidget " << "Edit_, signal textChanged()"<<std::endl;
    }
    spyEdit_->clear();
    for(int i = 0; i < spyPositionChanged_->size(); i++){
        std::cout<<"SPY LStepExpressPositionAxisWidget " << "this_, signal positionChanged()"<<std::endl;
    }
    spyPositionChanged_->clear();
}

void LStepExpressPositionAxisWidget::updateWidgets()
{
    double temp = Edit_->text().toDouble();
    if(temp != position_){
        position_ = temp;
        emit positionChanged(position_, axis_);
    }
    QString stemp = model_->getAxisDimensionShortName(axis_);
    if(stemp != axisDimensionName_){
        axisDimensionName_ = stemp;
        Unit_->setText(axisDimensionName_);
    }
}
