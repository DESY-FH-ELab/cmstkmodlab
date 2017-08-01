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

#include <nqlogger.h>
#include <QString>
#include <QDateTime>
#include <iostream>
#include "LStepExpressMeasurement.h"

LStepExpressMeasurement::LStepExpressMeasurement(LStepExpressModel* model, LStepExpressMotionManager* manager, LaserModel* laserModel, LStepExpressMeasurementTable* table,
				 QObject * /*parent*/)
    : QObject(),
      model_(model),
      manager_(manager),
      laserModel_(laserModel),
      table_(table)
{
    averageMeasEnabled_ = false;
    isLaserEnabled_ = false;
    measurementInProgress_ = false;

    //initialise the x, y, z positions
    z_init = model_->getPosition(2); 
    x_min = -150.0; 
    y_min = -150.0; 
    x_max = -150.0; 
    y_max = -150.0; 
    x_stepsize = 10.0;
    y_stepsize = 10.0;
    isZigZag_ = false;
    currentIndex_ = -1;
    tableSize_ = 0;

    connect(model_, SIGNAL(emergencyStopSignal()),
	this, SLOT(stopMeasurement()));
    
    connect(model_, SIGNAL(motionFinished()),
	this, SLOT(takeMeasurement()));
    
    connect(this, SIGNAL(nextScanStep()),
	this, SLOT(doNextScanStep()));

    connect(laserModel_, SIGNAL(deviceStateChanged(State)),
            this,SLOT(setLaserEnabled(State)));

    generateCirclePositions();

}
    
LStepExpressMeasurement::~LStepExpressMeasurement()
{
}

void LStepExpressMeasurement::setZigZag(bool zigzag)
{
  //    NQLog("LStepExpressMeasurement ", NQLog::Debug) << "setZigZag";
    isZigZag_ = zigzag;
}

void LStepExpressMeasurement::setAverageMeasEnabled(bool enabled)
{
  //    NQLog("LStepExpressMeasurement ", NQLog::Debug) << "setAverageMeasEnabled";
    if (enabled) {
      averageMeasEnabled_ = true;
    } else {
      averageMeasEnabled_ = false;
    }
}

void LStepExpressMeasurement::setInit(double x_min_, double x_max_, double y_min_, double y_max_, double x_stepsize_, double y_stepsize_)
{
  //    NQLog("LStepExpressMeasurement ", NQLog::Debug) << "setInit";
    x_min = x_min_;
    x_max = x_max_;
    y_min = y_min_;
    y_max = y_max_;
    x_stepsize = x_stepsize_;
    y_stepsize = y_stepsize_;
}

//generate the x and y positions for the measurement
void LStepExpressMeasurement::generatePositions()
{
  //    NQLog("LStepExpressMeasurement ", NQLog::Debug) << "in generatePositions "    ;

    //read in MIN/MAX positions
    z_init = model_->getPosition(2);

    nstepsx = (int) ((x_max - x_min)/x_stepsize);
    nstepsy = (int) ((y_max - y_min)/y_stepsize);

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

  //  NQLog("LStepExpressMeasurement ", NQLog::Debug) << "size pos_number = "<<pos_number.size()<<" size xpos = "<<xpos.size()    ;
  //store the positions in the table_ as columns
  //  table_->setRowCount(xpos.size()); //test if this line helps prevent seg faults after high amount of iterations
  table_->insertData(0, pos_number);
  table_->insertData(1, xpos);
  table_->insertData(2, ypos);
  table_->insertData(3, zpos);
  table_->insertData(4, meas);
  table_->update();

  tableSize_ = table_->rowCount();
  
  //  NQLog("LStepExpressMeasurement ", NQLog::Debug) << "generate positions, row count = "<<table_->rowCount()    ;

  //  NQLog("LStepExpressMeasurement ", NQLog::Debug) << "filled table "    ;

  clearedForMotion_ = true;
}

//FIX ME! fill in
void LStepExpressMeasurement::generateCirclePositions()
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

void LStepExpressMeasurement::stopMeasurement()
{
  //  NQLog("LStepExpressMeasurement ", NQLog::Debug) << "stop measurement"    ;  
  QMutexLocker locker(&mutex_);
  clearedForMotion_ = false;
  currentIndex_ = tableSize_;
}

void LStepExpressMeasurement::setLaserEnabled(State newState)
{
  //    NQLog("LStepExpressMeasurement ", NQLog::Debug) << "setLaserEnabled";
    if(newState == READY){
        isLaserEnabled_ = true;
    }else{
        isLaserEnabled_ = false;
    }
}

void LStepExpressMeasurement::takeMeasurement()
{
  //    NQLog("LStepExpressMeasurement ", NQLog::Debug) << "takeMeasurement"    ;  
    if(!isLaserEnabled_){return;}
    if(!measurementInProgress_){return;}
    
    double value = 0;
    laserModel_->getMeasurement(value);
    table_->insertData(4, currentIndex_, value);
    table_->update();
    currentIndex_++;

    emit informationChanged();
    emit nextScanStep();
}

void LStepExpressMeasurement::doNextScanStep()
{

    double x_pos;
    double y_pos;
    double z_pos;

    if(currentIndex_ < tableSize_ && clearedForMotion_){
        x_pos = table_->data(table_->index(currentIndex_,1), Qt::DisplayRole).toDouble();                                                                                
        y_pos = table_->data(table_->index(currentIndex_,2), Qt::DisplayRole).toDouble();                                                                               
	z_pos = table_->data(table_->index(currentIndex_,3), Qt::DisplayRole).toDouble();                                                                               
	model_->moveAbsolute(x_pos, y_pos, z_pos, 0.0);
    }else{
	measurementInProgress_ = false;
    }
}

//FIX ME! needs to be tested in the lab
void LStepExpressMeasurement::performScan()
{
    if(!isLaserEnabled_){return;}

    //    NQLog("LStepExpressMeasurement ", NQLog::Debug) << "starting scan"    ;

    measurementInProgress_ = true;
    clearedForMotion_ = true;
    currentIndex_ = 0;

    emit nextScanStep();
}

