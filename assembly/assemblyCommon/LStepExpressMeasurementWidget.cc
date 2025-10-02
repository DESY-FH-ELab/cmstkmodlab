/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
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
#include "LStepExpressMeasurementWidget.h"

LStepExpressMeasurementWidget::LStepExpressMeasurementWidget(LStepExpressModel* model, LaserModel* laserModel, LStepExpressMeasurement* measurement, LStepExpressMeasurementTable* table,
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
    //checkBoxEnableLaser_ = new QCheckBox("Enable Laser", this);
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
    //vlayout_laser->addWidget(checkBoxEnableLaser_);

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
    //connect(checkBoxEnableLaser_, SIGNAL(toggled(bool)),
    //	laserModel_, SLOT(setDeviceEnabled(bool)));

    //   connect(checkBoxEnableLaser_, SIGNAL(toggled(bool)),
    //	measurement_model_, SLOT(setLaserEnabled(bool)));
    
    connect(averageMeasCheckBox_, SIGNAL(toggled(bool)),
            measurement_model_, SLOT(setAverageMeasEnabled(bool)));
    
    connect(buttonGeneratePos_, SIGNAL(clicked()),
            measurement_model_, SLOT(generatePositions()));
    
    connect(buttonStartMeasurement_, SIGNAL(clicked()),
	measurement_model_, SLOT(performScan()));
    
    connect(buttonStopMeasurement_, SIGNAL(clicked()),
	model_, SLOT(emergencyStop()));

    connect(buttonStoreMeasurement_, SIGNAL(clicked()), this, SLOT(storeResults()));
    
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

    laserStateChanged(laserModel_->getDeviceState());
    lstepStateChanged(model_->getDeviceState());
}
    
LStepExpressMeasurementWidget::~LStepExpressMeasurementWidget()
{
    if(averageMeasCheckBox_){delete averageMeasCheckBox_; averageMeasCheckBox_ = nullptr;}
    if(buttonGeneratePos_){delete buttonGeneratePos_; buttonGeneratePos_ = nullptr;}
    if(buttonStartMeasurement_){delete buttonStartMeasurement_; buttonStartMeasurement_ = nullptr;}
    if(buttonStopMeasurement_){delete buttonStopMeasurement_; buttonStopMeasurement_ = nullptr;}
    if(buttonStoreMeasurement_){delete buttonStoreMeasurement_; buttonStoreMeasurement_ = nullptr;}
    //if(checkBoxEnableLaser_){delete checkBoxEnableLaser_; checkBoxEnableLaser_ = nullptr;}
    if(zigzagCheckBox_){delete zigzagCheckBox_; zigzagCheckBox_ = nullptr;}
}

void LStepExpressMeasurementWidget::laserStateChanged(State /* newState */)
{
  //    NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "laserStateChanged(State newState) " << newState    ;
  //  checkBoxEnableLaser_->setChecked(newState == READY || newState == INITIALIZING);
}

void LStepExpressMeasurementWidget::lstepStateChanged(State newState)
{
  //    NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "lstepStateChanged(State newState) " << newState    ;
    buttonStartMeasurement_->setEnabled(newState == READY || newState == INITIALIZING);
    buttonStopMeasurement_->setEnabled(newState == READY || newState == INITIALIZING);
    buttonStoreMeasurement_->setEnabled(newState == READY || newState == INITIALIZING);
}

void LStepExpressMeasurementWidget::setInit()
{
  //    NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "setInit()";
    measurement_model_->setInit(x_min_->text().toDouble(),x_max_->text().toDouble(),y_min_->text().toDouble(),y_max_->text().toDouble(),x_stepsize_->text().toDouble(),y_stepsize_->text().toDouble());
}

void LStepExpressMeasurementWidget::updateWidget()
{
  //  NQLog("LStepExpressMeasurementWidget ", NQLog::Debug) << "updateWidget()";
    table_view->update();
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

    out << "Index \t Timestamp \t X_pos \t Y_pos \t Z_pos \t R_pos \t LaserVal\n";
    for (int r = 0; r < table_->rowCount(); r++)
      {
          dateString = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
          out << table_->data(table_->index(r,0), Qt::DisplayRole).toInt() << "\t" << dateString << "\t"  << table_->data(table_->index(r,1), Qt::DisplayRole).toFloat() 
	    << "\t" << table_->data(table_->index(r,2), Qt::DisplayRole).toFloat() << "\t" << table_->data(table_->index(r,3), Qt::DisplayRole).toFloat() 
	  << "\t 0.000"
	    << "\t" << table_->data(table_->index(r,4), Qt::DisplayRole).toFloat() << "\n";
      }
    }

}

