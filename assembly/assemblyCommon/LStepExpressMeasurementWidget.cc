#include <nqlogger.h>
#include <QString>
#include <QDateTime>
#include <iostream>
#include "LStepExpressMeasurementWidget.h"

LStepExpressMeasurementWidget::LStepExpressMeasurementWidget(LStepExpressModel* model, LStepExpressMotionManager* manager, LaserModel* laserModel,
                                       QWidget *parent)
    : QWidget(parent),
      model_(model),
      manager_(manager),
      laserModel_(laserModel)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    
    //    QVBoxLayout* vlayout = new QVBoxLayout(this);
    
    averageMeasCheckBox_ = new QCheckBox("Average measurement", this);
    buttonGeneratePos_ = new QPushButton("Generate positions", this);
    zigzagCheckBox_ = new QCheckBox("Zigzag motion (default is meander)", this);
    
    averageMeasEnabled_ = false;
    
    QLabel* label_x_min = new QLabel("x min");
    QLabel* label_x_max = new QLabel("x max");
    QLabel* label_y_min = new QLabel("y min");
    QLabel* label_y_max = new QLabel("y max");
    //    QLabel* nstepsxLabel = new QLabel("steps x-direction");
    //QLabel* nstepsyLabel = new QLabel("steps y-direction");
    QLabel* label_x_stepsize = new QLabel("stepsize x");
    QLabel* label_y_stepsize = new QLabel("stepsize y");
    
    //nstepsx_ = new QLineEdit();
    //nstepsy_ = new QLineEdit();
    x_min_ = new QLineEdit();
    x_max_ = new QLineEdit();
    y_min_ = new QLineEdit();
    y_max_ = new QLineEdit();
    x_stepsize_ = new QLineEdit();
    y_stepsize_ = new QLineEdit();

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
    //    layout->addLayout(vlayout);
    
    //initialise the x, y, z positions
    z_init = model_->getPosition(2); //FIX ME! is 2 z-axis?
    x_min = -150.0; //FIX ME! take right upper corner of table
    y_min = -150.0; //FIX ME! take right upper corner of table
    x_max = -150.0; //FIX ME! take right upper corner of table
    y_max = -150.0; //FIX ME! take right upper corner of table
    x_stepsize = 10.0;
    y_stepsize = 10.0;
    isZigZag_ = false;
    currentIndex_ = -1;
    
    //table model/view
    table_model = new LStepExpressMeasurementTable();
    table_view = new QTableView(this);
    table_view->setModel(table_model);
    layout->addWidget(table_view);

    QVBoxLayout *vlayout_laser = new QVBoxLayout(this);
    checkBoxEnableLaser_ = new QCheckBox("Enable Laser", this);
    vlayout_laser->addWidget(checkBoxEnableLaser_);

    laserWidget_ = new LaserWidget(laserModel_);
    vlayout_laser->addWidget(laserWidget_);

    QVBoxLayout *vlayout_buttons = new QVBoxLayout(this);
    buttonStartMeasurement_ = new QPushButton("Start Measurement", this);
    buttonStartMeasurement_->setEnabled(false);
    vlayout_buttons->addWidget(buttonStartMeasurement_);
    
    buttonStopMeasurement_ = new QPushButton("Stop Measurement", this);
    buttonStopMeasurement_->setEnabled(false);
    vlayout_buttons->addWidget(buttonStopMeasurement_);
    
    buttonStoreMeasurement_ = new QPushButton("Store Results Measurement", this);
    buttonStoreMeasurement_->setEnabled(false);
    vlayout_buttons->addWidget(buttonStoreMeasurement_);
    
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addLayout(vlayout_laser);
    vlayout->addLayout(vlayout_buttons);

    layout->addLayout(vlayout);
    
    connect(checkBoxEnableLaser_, SIGNAL(toggled(bool)),
	laserModel_, SLOT(setDeviceEnabled(bool)));
    
    connect(averageMeasCheckBox_, SIGNAL(toggled(bool)),
            this, SLOT(setAverageMeasEnabled(bool)));
    
    connect(buttonGeneratePos_, SIGNAL(clicked()),
            this, SLOT(generatePositions()));
    
    connect(buttonStartMeasurement_, SIGNAL(clicked()),
	this, SLOT(performScan()));
    
    connect(buttonStopMeasurement_, SIGNAL(clicked()),
            this, SLOT(stopMeasurement()));
    
    connect(buttonStoreMeasurement_, SIGNAL(clicked()), this, SLOT(storeResults()));
    
    connect(laserModel_, SIGNAL(deviceStateChanged(State)),
	this, SLOT(laserStateChanged(State)));

    connect(zigzagCheckBox_, SIGNAL(toggled(bool)), this, SLOT(setZigZag(bool)));

    connect(model_, SIGNAL(motionFinished()),
	this, SLOT(takeMeasurement()));
    
    //test
    connect(this, SIGNAL(FakeMotionFinished()),
	this, SLOT(takeMeasurement()));

    connect(this, SIGNAL(nextScanStep()),
	this, SLOT(doNextScanStep()));

    laserStateChanged(laserModel_->getDeviceState());

    generateCirclePositions();

    spyAverageMeasCheckBox_ = new QSignalSpy(averageMeasCheckBox_, SIGNAL(toggled(bool)));
    spyButtonGeneratePos_ = new QSignalSpy(buttonGeneratePos_, SIGNAL(clicked()));
    spyButtonStartMeasurement_ = new QSignalSpy(buttonStartMeasurement_, SIGNAL(clicked()));
    spyButtonStopMeasurement_ = new QSignalSpy(buttonStopMeasurement_, SIGNAL(clicked()));
    spyButtonStoreMeasurement_ = new QSignalSpy(buttonStoreMeasurement_, SIGNAL(clicked()));
    spyCheckBoxEnableLaser_ = new QSignalSpy(checkBoxEnableLaser_, SIGNAL(toggled(bool)));
    spyZigzagCheckBox_ = new QSignalSpy(zigzagCheckBox_, SIGNAL(toggled(bool)));
    spyNextScanStep_ = new QSignalSpy(this, SIGNAL(nextScanStep()));

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

    connect(this, SIGNAL(nextScanStep()), this, SLOT(doNextScanStep()));
}
    
LStepExpressMeasurementWidget::~LStepExpressMeasurementWidget()
{
    if(spyAverageMeasCheckBox_){delete spyAverageMeasCheckBox_; spyAverageMeasCheckBox_ = NULL;}
    if(spyButtonGeneratePos_){delete spyButtonGeneratePos_; spyButtonGeneratePos_ = NULL;}
    if(spyButtonStartMeasurement_){delete spyButtonStartMeasurement_; spyButtonStartMeasurement_ = NULL;}
    if(spyButtonStopMeasurement_){delete spyButtonStopMeasurement_; spyButtonStopMeasurement_ = NULL;}
    if(spyButtonStoreMeasurement_){delete spyButtonStoreMeasurement_; spyButtonStoreMeasurement_ = NULL;}
    if(spyCheckBoxEnableLaser_){delete spyCheckBoxEnableLaser_; spyCheckBoxEnableLaser_ = NULL;}
    if(spyZigzagCheckBox_){delete spyZigzagCheckBox_; spyZigzagCheckBox_ = NULL;}
    if(spyNextScanStep_){delete spyNextScanStep_; spyNextScanStep_ = NULL;}
    if(averageMeasCheckBox_){delete averageMeasCheckBox_; averageMeasCheckBox_ = NULL;}
    if(buttonGeneratePos_){delete buttonGeneratePos_; buttonGeneratePos_ = NULL;}
    if(buttonStartMeasurement_){delete buttonStartMeasurement_; buttonStartMeasurement_ = NULL;}
    if(buttonStopMeasurement_){delete buttonStopMeasurement_; buttonStopMeasurement_ = NULL;}
    if(buttonStoreMeasurement_){delete buttonStoreMeasurement_; buttonStoreMeasurement_ = NULL;}
    if(checkBoxEnableLaser_){delete checkBoxEnableLaser_; checkBoxEnableLaser_ = NULL;}
    if(zigzagCheckBox_){delete zigzagCheckBox_; zigzagCheckBox_ = NULL;}
}

void LStepExpressMeasurementWidget::printSpyInformation()
{
    for(int i = 0; i < spyAverageMeasCheckBox_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget", NQLog::Debug)<< "averageMeasCheckBox_, signal toggled("<<(spyAverageMeasCheckBox_->value(i))[0].toBool()<<")"  ;
    }
    spyAverageMeasCheckBox_->clear();
    for(int i = 0; i < spyButtonGeneratePos_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget", NQLog::Debug)<< "buttonGeneratePos_, signal clicked()";
    }
    spyButtonGeneratePos_->clear();
    for(int i = 0; i < spyButtonStartMeasurement_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget", NQLog::Debug)<< "buttonStartMeasurement_, signal clicked()"  ;
    }
    spyButtonStartMeasurement_->clear();
    for(int i = 0; i < spyButtonStopMeasurement_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget", NQLog::Debug)<< "buttonStopMeasurement_, signal clicked()"  ;
    }
    spyButtonStopMeasurement_->clear();
    for(int i = 0; i < spyButtonStoreMeasurement_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget", NQLog::Debug)<< "buttonStoreMeasurement_, signal clicked()"  ;
    }
    spyButtonStoreMeasurement_->clear();
    for(int i = 0; i < spyCheckBoxEnableLaser_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget", NQLog::Debug)<< "checkBoxEnableLaser_, signal toggled("<<(spyCheckBoxEnableLaser_->value(i))[0].toBool()<<")"  ;
    }
    spyCheckBoxEnableLaser_->clear();
    for(int i = 0; i < spyZigzagCheckBox_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget", NQLog::Debug)<< "zigzagCheckBox_, signal toggled("<<(spyZigzagCheckBox_->value(i))[0].toBool()<<")"  ;
    }
    spyZigzagCheckBox_->clear();
    for(int i = 0; i < spyNextScanStep_->size(); i++){
        NQLog("SPY LStepExpressMeasurementWidget", NQLog::Debug)<< "this, signal nextScanStep()";
    }
    spyNextScanStep_->clear();
}

void LStepExpressMeasurementWidget::FakeMotion()
{
    NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "FakeMotion";
    usleep(100000);
    emit FakeMotionFinished();
}

void LStepExpressMeasurementWidget::setZigZag(bool zigzag)
{
    isZigZag_ = zigzag;
}

void LStepExpressMeasurementWidget::laserStateChanged(State newState)
{
    NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "laserStateChanged(State newState) " << newState    ;
    checkBoxEnableLaser_->setChecked(newState == READY || newState == INITIALIZING);
}

void LStepExpressMeasurementWidget::setAverageMeasEnabled(bool enabled)
{
    if (enabled) {
      averageMeasEnabled_ = true;
    } else {
      averageMeasEnabled_ = false;
    }
}

//generate the x and y positions for the measurement
void LStepExpressMeasurementWidget::generatePositions()
{
    NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "in generatePositions "    ;

    //read in MIN/MAX positions
    z_init = model_->getPosition(2);
    x_min = (x_min_->text().isEmpty() == true) ? -150 : (x_min_->text().toDouble());
    y_min = (y_min_->text().isEmpty() == true) ? -150 : (y_min_->text().toDouble());
    x_max = (x_max_->text().isEmpty() == true) ? -150 : (x_max_->text().toDouble());
    y_max = (y_max_->text().isEmpty() == true) ? -150 : (y_max_->text().toDouble());

    //read in the stepsize
    x_stepsize = (x_stepsize_->text().isEmpty() == true) ? 10 : (x_stepsize_->text().toDouble());
    y_stepsize = (y_stepsize_->text().isEmpty() == true) ? 10 : (y_stepsize_->text().toDouble());

    //read in number of steps, if empty take default 10 steps
    //    nstepsx = (nstepsx_->text().isEmpty() == true) ? 10 : (nstepsx_->text().toInt());
    //nstepsy = (nstepsy_->text().isEmpty() == true) ? 10 : (nstepsy_->text().toInt());

    nstepsx = (int) ((x_max - x_min)/x_stepsize);
    nstepsy = (int) ((y_max - y_min)/y_stepsize);


  //  NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "nstepsx = " << nstepsx << " nstepsy = " << nstepsy << " x_init = " << x_init << " y_init = " << y_init << " rangex = " << rangex << " rangey = "<< rangey    ;

  std::vector<float> xpos;
  std::vector<float> ypos;
  std::vector<float> zpos;
  std::vector<float> pos_number;
  std::vector<float> meas;
  
  int z = 0;
  
  if(isZigZag_){
      //generate the positions in zigzag pattern
      for(int i = 0; i < nstepsx + 1; i++){
          for(int j = 0; j < nstepsy + 1; j++){
	  xpos.push_back(x_min + i*x_stepsize);
	  ypos.push_back(y_min + j*y_stepsize);
	  pos_number.push_back(z);
	  zpos.push_back(z_init);
	  meas.push_back(0.0);
	  z++;
          }
      }
  }else{  
      //generate the positions in meander pattern
      float y_prev = y_min;
      float y_pres = y_min;
      for(int i = 0; i < nstepsx + 1; i++){
          for(int j = 0; j < nstepsy + 1; j++){
	  xpos.push_back(x_min + i*x_stepsize);
	  if(j != 0){y_pres = y_prev + pow(-1,i)*y_stepsize;}
	  ypos.push_back(y_pres);
	  y_prev = y_pres;
	  pos_number.push_back(z);
	  zpos.push_back(z_init);
	  meas.push_back(0.0);
	  z++;
          }
      }
  }

  //  NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "size pos_number = "<<pos_number.size()<<" size xpos = "<<xpos.size()    ;
  //store the positions in the table_model as columns
  table_model->insertData(0, pos_number);
  table_model->insertData(1, xpos);
  table_model->insertData(2, ypos);
  table_model->insertData(3, zpos);
  table_model->insertData(4, meas);
  table_model->update();

  //  NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "filled positions "    ;

  //  NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "filled table "    ;

  buttonStartMeasurement_->setEnabled(true);
  buttonStopMeasurement_->setEnabled(true);
  clearedForMotion_ = true;
  buttonStoreMeasurement_->setEnabled(false);
}

//FIX ME! fill in
void LStepExpressMeasurementWidget::generateCirclePositions()
{
  //R^2 = x^2 + y^2
  double R = 0.5; //circle of diameter 1mm
  double x = 0.;
  for(int i = 0; i < 100; i++)
    {
      x = -0.5 + i*0.01;
      circle_x.push_back(x);
      circle_y.push_back(sqrt(R*R - x*x));
    }
  for(int i = 0; i < 100; i++)
    {
      x = +0.5 - i*0.01;
      circle_x.push_back(x);
      circle_y.push_back(-sqrt(R*R - x*x));
    }

}

void LStepExpressMeasurementWidget::stopMeasurement()
{
  NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "stop measurement"    ;  
  QMutexLocker locker(&mutex_);
  //model_->emergencyStop(); //for test
  clearedForMotion_ = false;
  currentIndex_ = table_model->rowCount();
}

void LStepExpressMeasurementWidget::takeMeasurement()
{
    NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "takeMeasurement"    ;  
    //QMutexLocker locker(&mutex_);
    double value = 0;
    //laserModel_->getMeasurement(value);
    value = currentIndex_; //test
    table_model->insertData(4, currentIndex_, value);
    table_model->update();
    currentIndex_++;

    usleep(100000);
    emit nextScanStep();
}

void LStepExpressMeasurementWidget::doNextScanStep()
{
    NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "doNextScanStep(), currentIndex = "<<currentIndex_ << " cleared for motion = "<<clearedForMotion_   ;  

    double x_pos;
    double y_pos;
    double z_pos;

    if(currentIndex_ < table_model->rowCount() && clearedForMotion_){
        x_pos = table_model->data(table_model->index(currentIndex_,1), Qt::DisplayRole).toDouble();                                                                                                   
        y_pos = table_model->data(table_model->index(currentIndex_,2), Qt::DisplayRole).toDouble();                                                                                                   
        z_pos = table_model->data(table_model->index(currentIndex_,3), Qt::DisplayRole).toDouble();                                                                                                   
        //manager_->moveAbsolute(x_pos, y_pos, z_pos, 0.0);
        usleep(100000);
        this->FakeMotion();
    }else{
        NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "scan finished"    ;
        buttonStoreMeasurement_->setEnabled(true);
    }
}

//FIX ME! needs to be tested in the lab
void LStepExpressMeasurementWidget::performScan()
{
    NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "starting scan"    ;
    
    clearedForMotion_ = true;
    currentIndex_ = 0;

    emit nextScanStep();

    /*
  double x_pos = 0;
  double y_pos = 0;
  double z_pos = 0;
  std::vector<float> meas;
  //go to all positions, do the measurement, store the measurement data
  int r = 0;
  while(clearedForMotion_ && r < table_model->rowCount())
      {
          currentIndex_ = r;
          //retrieve position from the model
          x_pos = table_model->data(table_model->index(r,1), Qt::DisplayRole).toInt();
          y_pos = table_model->data(table_model->index(r,2), Qt::DisplayRole).toInt();
          z_pos = table_model->data(table_model->index(r,3), Qt::DisplayRole).toInt();
          
          //go to position
          manager_->moveAbsolute(x_pos, y_pos, z_pos, 0.0);
          
          //if requested average measurement, perform circle at position
          double meas_atpos = 0.;
          NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "averageMeasEnabled_ = " << averageMeasEnabled_    ;
          if(averageMeasEnabled_)
	  {
	      NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "in average meas, circle_x size = " << circle_x.size()    ;
	      for(unsigned int c = 0; c < circle_x.size(); c++)
	          {
		  //manager_->moveAbsolute(x_pos+(circle_x)[c], y_pos+(circle_y)[c], z_pos, 0.0);
		  //make laser measurement
	          }
	      meas_atpos = 1.;
	  }else{
	  NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "in standard meas "    ;
	  //make laser measurement
	  //laserModel_->getMeasurement(meas_atpos);
          }
          
          
          NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "after meas "    ;
          //FIX ME! how to ensure the movement has stopped before making measurement with laser?
          
          //store dummy measurement result
          //meas.push_back(meas_atpos);
          r++;
	  NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "during scan, cleared for motion = " << clearedForMotion_    ;
      }

  table_model->insertData(4,meas);
  table_model->update();
  
  NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "scan finished"    ;

  buttonStoreMeasurement_->setEnabled(true);
    */
}

//FIX ME! store results in which format? ROOT file, text file, mathematica file?
//for now use simple text file
void LStepExpressMeasurementWidget::storeResults()
{
  QString filename = QFileDialog::getSaveFileName(this, "Write Settings", "~/", "*.lstep");
  if (filename.isNull()) return;

  QFile data(filename);
  QString dateString;
  if (data.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream out(&data);

    out << "average measurement was performed: " << averageMeasEnabled_ << "\n";
    out << "Index \t Timestamp \t X_pos \t Y_pos \t Z_pos \t R_pos \t LaserVal\n";
    for (int r = 0; r < table_model->rowCount(); r++)
      {
          dateString = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
          out << table_model->data(table_model->index(r,0), Qt::DisplayRole).toInt() << "\t" << dateString << "\t"  << table_model->data(table_model->index(r,1), Qt::DisplayRole).toInt() 
	    << "\t" << table_model->data(table_model->index(r,2), Qt::DisplayRole).toInt() << "\t" << table_model->data(table_model->index(r,3), Qt::DisplayRole).toInt() 
	  << "\t 0.000"
	    << "\t" << table_model->data(table_model->index(r,4), Qt::DisplayRole).toInt() << "\n";
      }
    }

}

