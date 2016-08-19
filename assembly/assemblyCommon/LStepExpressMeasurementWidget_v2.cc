#include <nqlogger.h>
#include <QString>
#include <QDateTime>
#include <iostream>
#include "LStepExpressMeasurementWidget_v2.h"

LStepExpressMeasurementWidget_v2::LStepExpressMeasurementWidget_v2(LStepExpressModel* model, LaserModel* laserModel, LStepExpressMeasurement* measurement, LStepExpressMeasurementTable* table,
                                       QWidget *parent)
    : QWidget(parent),
      model_(model),
      laserModel_(laserModel),
      measurement_model_(measurement),
      table_(table)
{
    //initialize all the widgets
    averageMeasCheckBox_ = new QCheckBox("Average measurement", this);
    buttonGeneratePos_ = new QPushButton("Generate positions", this);
    zigzagCheckBox_ = new QCheckBox("Zigzag motion (default is meander)", this);
    buttonStartMeasurement_ = new QPushButton("Start Measurement", this);
    buttonStartMeasurement_->setEnabled(false);
    checkBoxEnableLaser_ = new QCheckBox("Enable Laser", this);
    buttonStopMeasurement_ = new QPushButton("Stop Measurement", this);
    buttonStopMeasurement_->setEnabled(false);
    buttonStoreMeasurement_ = new QPushButton("Store Results Measurement", this);
    buttonStoreMeasurement_->setEnabled(false);

    x_min_ = new QLineEdit();
    x_max_ = new QLineEdit();
    y_min_ = new QLineEdit();
    y_max_ = new QLineEdit();
    x_stepsize_ = new QLineEdit();
    y_stepsize_ = new QLineEdit();
    
    QLabel* label_x_min = new QLabel("x min");
    QLabel* label_x_max = new QLabel("x max");
    QLabel* label_y_min = new QLabel("y min");
    QLabel* label_y_max = new QLabel("y max");
    QLabel* label_x_stepsize = new QLabel("stepsize x");
    QLabel* label_y_stepsize = new QLabel("stepsize y");

    //set the layout
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    
    QHBoxLayout *hlayout_x_min = new QHBoxLayout(this);
    hlayout_x_min->addWidget(label_x_min);
    hlayout_x_min->addWidget(x_min_);
    QHBoxLayout *hlayout_y_min = new QHBoxLayout(this);
    hlayout_y_min->addWidget(label_y_min);
    hlayout_y_min->addWidget(y_min_);
    QHBoxLayout *hlayout_x_max = new QHBoxLayout(this);
    hlayout_x_max->addWidget(label_x_max);
    hlayout_x_max->addWidget(x_max_);
    QHBoxLayout *hlayout_y_max = new QHBoxLayout(this);
    hlayout_y_max->addWidget(label_y_max);
    hlayout_y_max->addWidget(y_max_);
    QHBoxLayout *hlayout_x_stepsize = new QHBoxLayout(this);
    hlayout_x_stepsize->addWidget(label_x_stepsize);
    hlayout_x_stepsize->addWidget(x_stepsize_);
    QHBoxLayout *hlayout_y_stepsize = new QHBoxLayout(this);
    hlayout_y_stepsize->addWidget(label_y_stepsize);
    hlayout_y_stepsize->addWidget(y_stepsize_);

    QHBoxLayout *hlayout_checkbox = new QHBoxLayout(this);
    hlayout_checkbox->addWidget(averageMeasCheckBox_);
    hlayout_checkbox->addWidget(zigzagCheckBox_);

    QVBoxLayout *layout_xy = new QVBoxLayout(this);
    layout_xy->addLayout(hlayout_x_min);
    layout_xy->addLayout(hlayout_x_max);
    layout_xy->addLayout(hlayout_x_stepsize);
    layout_xy->addLayout(hlayout_y_min);
    layout_xy->addLayout(hlayout_y_max);
    layout_xy->addLayout(hlayout_y_stepsize);
    layout_xy->addLayout(hlayout_checkbox);
    layout_xy->addWidget(buttonGeneratePos_);
    
    layout->addLayout(layout_xy);

    table_view = new QTableView(this);
    table_view->setModel(table_);
    layout->addWidget(table_view);

    QVBoxLayout *vlayout_laser = new QVBoxLayout(this);
    vlayout_laser->addWidget(checkBoxEnableLaser_);

    laserWidget_ = new LaserWidget(laserModel_);
    vlayout_laser->addWidget(laserWidget_);

    QVBoxLayout *vlayout_buttons = new QVBoxLayout(this);
    vlayout_buttons->addWidget(buttonStartMeasurement_);
    vlayout_buttons->addWidget(buttonStopMeasurement_);
    vlayout_buttons->addWidget(buttonStoreMeasurement_);
    
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addLayout(vlayout_laser);
    vlayout->addLayout(vlayout_buttons);

    layout->addLayout(vlayout);
    
    //make all connections
    connect(checkBoxEnableLaser_, SIGNAL(toggled(bool)),
	laserModel_, SLOT(setDeviceEnabled(bool)));

    connect(checkBoxEnableLaser_, SIGNAL(toggled(bool)),
	measurement_model_, SLOT(setLaserEnabled(bool)));
    
    connect(averageMeasCheckBox_, SIGNAL(toggled(bool)),
            measurement_model_, SLOT(setAverageMeasEnabled(bool)));
    
    connect(buttonGeneratePos_, SIGNAL(clicked()),
            measurement_model_, SLOT(generatePositions()));
    
    connect(buttonStartMeasurement_, SIGNAL(clicked()),
	measurement_model_, SLOT(performScan()));
    
    connect(buttonStopMeasurement_, SIGNAL(clicked()),
	model_, SLOT(emergencyStop()));

    connect(buttonStoreMeasurement_, SIGNAL(clicked()), this, SLOT(storeResults()));
    
    connect(laserModel_, SIGNAL(deviceStateChanged(State)),
	this, SLOT(laserStateChanged(State)));

    connect(model_, SIGNAL(deviceStateChanged(State)),
	this, SLOT(lstepStateChanged(State)));

    connect(zigzagCheckBox_, SIGNAL(toggled(bool)), measurement_model_, SLOT(setZigZag(bool)));

    connect(measurement_model_, SIGNAL(informationChanged()),
	this, SLOT(updateWidget()));

    connect(x_min_, SIGNAL(textChanged(QString)),
	this, SLOT(setInit()));
    connect(x_max_, SIGNAL(textChanged(QString)),
	this, SLOT(setInit()));
    connect(y_min_, SIGNAL(textChanged(QString)),
	this, SLOT(setInit()));
    connect(y_max_, SIGNAL(textChanged(QString)),
	this, SLOT(setInit()));
    connect(x_stepsize_, SIGNAL(textChanged(QString)),
	this, SLOT(setInit()));
    connect(y_stepsize_, SIGNAL(textChanged(QString)),
	this, SLOT(setInit()));

    //make the signal spies and the connections
    spyAverageMeasCheckBox_ = new QSignalSpy(averageMeasCheckBox_, SIGNAL(toggled(bool)));
    spyButtonGeneratePos_ = new QSignalSpy(buttonGeneratePos_, SIGNAL(clicked()));
    spyButtonStartMeasurement_ = new QSignalSpy(buttonStartMeasurement_, SIGNAL(clicked()));
    spyButtonStopMeasurement_ = new QSignalSpy(buttonStopMeasurement_, SIGNAL(clicked()));
    spyButtonStoreMeasurement_ = new QSignalSpy(buttonStoreMeasurement_, SIGNAL(clicked()));
    spyCheckBoxEnableLaser_ = new QSignalSpy(checkBoxEnableLaser_, SIGNAL(toggled(bool)));
    spyZigzagCheckBox_ = new QSignalSpy(zigzagCheckBox_, SIGNAL(toggled(bool)));

    connect(checkBoxEnableLaser_, SIGNAL(toggled(bool)),
	this, SLOT(printSpyInformation()));
    
    connect(averageMeasCheckBox_, SIGNAL(toggled(bool)),
            this, SLOT(printSpyInformation()));
    
    connect(buttonGeneratePos_, SIGNAL(clicked()),
            this, SLOT(printSpyInformation()));
    
    connect(buttonStartMeasurement_, SIGNAL(clicked()),
	this, SLOT(printSpyInformation()));
    
    connect(buttonStopMeasurement_, SIGNAL(clicked()),
            this, SLOT(printSpyInformation()));
    
    connect(buttonStoreMeasurement_, SIGNAL(clicked()), this, SLOT(printSpyInformation()));
    
    connect(zigzagCheckBox_, SIGNAL(toggled(bool)), this, SLOT(printSpyInformation()));

    laserStateChanged(laserModel_->getDeviceState());
    lstepStateChanged(model_->getDeviceState());
}
    
LStepExpressMeasurementWidget_v2::~LStepExpressMeasurementWidget_v2()
{
    if(spyAverageMeasCheckBox_){delete spyAverageMeasCheckBox_; spyAverageMeasCheckBox_ = NULL;}
    if(spyButtonGeneratePos_){delete spyButtonGeneratePos_; spyButtonGeneratePos_ = NULL;}
    if(spyButtonStartMeasurement_){delete spyButtonStartMeasurement_; spyButtonStartMeasurement_ = NULL;}
    if(spyButtonStopMeasurement_){delete spyButtonStopMeasurement_; spyButtonStopMeasurement_ = NULL;}
    if(spyButtonStoreMeasurement_){delete spyButtonStoreMeasurement_; spyButtonStoreMeasurement_ = NULL;}
    if(spyCheckBoxEnableLaser_){delete spyCheckBoxEnableLaser_; spyCheckBoxEnableLaser_ = NULL;}
    if(spyZigzagCheckBox_){delete spyZigzagCheckBox_; spyZigzagCheckBox_ = NULL;}
    if(averageMeasCheckBox_){delete averageMeasCheckBox_; averageMeasCheckBox_ = NULL;}
    if(buttonGeneratePos_){delete buttonGeneratePos_; buttonGeneratePos_ = NULL;}
    if(buttonStartMeasurement_){delete buttonStartMeasurement_; buttonStartMeasurement_ = NULL;}
    if(buttonStopMeasurement_){delete buttonStopMeasurement_; buttonStopMeasurement_ = NULL;}
    if(buttonStoreMeasurement_){delete buttonStoreMeasurement_; buttonStoreMeasurement_ = NULL;}
    if(checkBoxEnableLaser_){delete checkBoxEnableLaser_; checkBoxEnableLaser_ = NULL;}
    if(zigzagCheckBox_){delete zigzagCheckBox_; zigzagCheckBox_ = NULL;}
}

void LStepExpressMeasurementWidget_v2::printSpyInformation()
{
    for(int i = 0; i < spyAverageMeasCheckBox_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget_v2", NQLog::Debug)<< "averageMeasCheckBox_, signal toggled("<<(spyAverageMeasCheckBox_->value(i))[0].toBool()<<")"  ;
    }
    spyAverageMeasCheckBox_->clear();
    for(int i = 0; i < spyButtonGeneratePos_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget_v2", NQLog::Debug)<< "buttonGeneratePos_, signal clicked()";
    }
    spyButtonGeneratePos_->clear();
    for(int i = 0; i < spyButtonStartMeasurement_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget_v2", NQLog::Debug)<< "buttonStartMeasurement_, signal clicked()"  ;
    }
    spyButtonStartMeasurement_->clear();
    for(int i = 0; i < spyButtonStopMeasurement_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget_v2", NQLog::Debug)<< "buttonStopMeasurement_, signal clicked()"  ;
    }
    spyButtonStopMeasurement_->clear();
    for(int i = 0; i < spyButtonStoreMeasurement_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget_v2", NQLog::Debug)<< "buttonStoreMeasurement_, signal clicked()"  ;
    }
    spyButtonStoreMeasurement_->clear();
    for(int i = 0; i < spyCheckBoxEnableLaser_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget_v2", NQLog::Debug)<< "checkBoxEnableLaser_, signal toggled("<<(spyCheckBoxEnableLaser_->value(i))[0].toBool()<<")"  ;
    }
    spyCheckBoxEnableLaser_->clear();
    for(int i = 0; i < spyZigzagCheckBox_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget_v2", NQLog::Debug)<< "zigzagCheckBox_, signal toggled("<<(spyZigzagCheckBox_->value(i))[0].toBool()<<")"  ;
    }
    spyZigzagCheckBox_->clear();
}

void LStepExpressMeasurementWidget_v2::laserStateChanged(State newState)
{
    NQLog("LStepExpressMeasurementWidget_v2 ", NQLog::Debug) << "laserStateChanged(State newState) " << newState    ;
    checkBoxEnableLaser_->setChecked(newState == READY || newState == INITIALIZING);
}

void LStepExpressMeasurementWidget_v2::lstepStateChanged(State newState)
{
    NQLog("LStepExpressMeasurementWidget_v2 ", NQLog::Debug) << "lstepStateChanged(State newState) " << newState    ;
    buttonStartMeasurement_->setEnabled(newState == READY || newState == INITIALIZING);
    buttonStopMeasurement_->setEnabled(newState == READY || newState == INITIALIZING);
    buttonStoreMeasurement_->setEnabled(newState == READY || newState == INITIALIZING);
}

void LStepExpressMeasurementWidget_v2::setInit()
{
    NQLog("LStepExpressMeasurementWidget_v2 ", NQLog::Debug) << "setInit()";
    measurement_model_->setInit(x_min_->text().toDouble(),x_max_->text().toDouble(),y_min_->text().toDouble(),y_max_->text().toDouble(),x_stepsize_->text().toDouble(),y_stepsize_->text().toDouble());
}

void LStepExpressMeasurementWidget_v2::updateWidget()
{
    NQLog("LStepExpressMeasurementWidget_v2 ", NQLog::Debug) << "updateWidget()";
    table_view->update();
}

//FIX ME! store results in which format? ROOT file, text file, mathematica file?
//for now use simple text file
void LStepExpressMeasurementWidget_v2::storeResults()
{
  QString filename = QFileDialog::getSaveFileName(this, "Write Settings", "~/", "*.lstep");
  if (filename.isNull()) return;

  QFile data(filename);
  QString dateString;
  if (data.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream out(&data);

    out << "Index \t Timestamp \t X_pos \t Y_pos \t Z_pos \t R_pos \t LaserVal\n";
    for (int r = 0; r < table_->rowCount(); r++)
      {
          dateString = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
          out << table_->data(table_->index(r,0), Qt::DisplayRole).toInt() << "\t" << dateString << "\t"  << table_->data(table_->index(r,1), Qt::DisplayRole).toInt() 
	    << "\t" << table_->data(table_->index(r,2), Qt::DisplayRole).toInt() << "\t" << table_->data(table_->index(r,3), Qt::DisplayRole).toInt() 
	  << "\t 0.000"
	    << "\t" << table_->data(table_->index(r,4), Qt::DisplayRole).toFloat() << "\n";
      }
    }

}

