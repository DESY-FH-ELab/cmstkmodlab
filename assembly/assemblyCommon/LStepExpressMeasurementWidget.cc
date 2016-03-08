#include <nqlogger.h>

#include "LStepExpressMeasurementWidget.h"

LStepExpressMeasurementWidget::LStepExpressMeasurementWidget(LStepExpressModel* model, LStepExpressMotionManager* manager,
                                       QWidget *parent)
    : QWidget(parent),
      model_(model),
      manager_(manager)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);

    QVBoxLayout* vlayout = new QVBoxLayout(this);

    averageMeasCheckBox_ = new QCheckBox("Average measurement", this);
    vlayout->addWidget(averageMeasCheckBox_);

    buttonGeneratePos_ = new QPushButton("Generate positions", this);
    vlayout->addWidget(buttonGeneratePos_);

    connect(averageMeasCheckBox_, SIGNAL(toggled(bool)),
            this, SLOT(setAverageMeasEnabled(bool)));

    connect(buttonGeneratePos_, SIGNAL(clicked()),
            this, SLOT(generatePositions()));

    averageMeasEnabled_ = false;

    QLabel* nstepsxLabel = new QLabel("steps x-direction");
    QLabel* nstepsyLabel = new QLabel("steps y-direction");

    nstepsx_ = new QLineEdit();
    nstepsy_ = new QLineEdit();

    QHBoxLayout *hlayout_x = new QHBoxLayout(this);
    hlayout_x->addWidget(nstepsxLabel);
    hlayout_x->addWidget(nstepsx_);
    QHBoxLayout *hlayout_y = new QHBoxLayout(this);
    hlayout_y->addWidget(nstepsyLabel);
    hlayout_y->addWidget(nstepsy_);

    QVBoxLayout *layout_xy = new QVBoxLayout(this);
    layout_xy->addLayout(hlayout_x);
    layout_xy->addLayout(hlayout_y);

    layout->addLayout(layout_xy);
    layout->addLayout(vlayout);

    //initialise the x, y, z positions
    z_init = model_->getPosition(2); //FIX ME! is 2 z-axis?
    x_init = -150; //FIX ME! take right upper corner of table
    y_init = -150; //FIX ME! take right upper corner of table


    //table model/view
    table_model = new LStepExpressMeasurementTable();
    table_view = new QTableView(this);
    table_view->setModel(table_model);
    layout->addWidget(table_view);

    QVBoxLayout *vlayout_buttons = new QVBoxLayout(this);
    buttonStartMeasurement_ = new QPushButton("Start Measurement", this);
    buttonStartMeasurement_->setEnabled(false);
    vlayout_buttons->addWidget(buttonStartMeasurement_);

    connect(buttonStartMeasurement_, SIGNAL(clicked()),
	    this, SLOT(performMeasurement()));


    buttonStopMeasurement_ = new QPushButton("Stop Measurement", this);
    buttonStopMeasurement_->setEnabled(false);
    vlayout_buttons->addWidget(buttonStopMeasurement_);

    connect(buttonStopMeasurement_, SIGNAL(clicked()),
            this, SLOT(stopMeasurement()));


    buttonStoreMeasurement_ = new QPushButton("Store Results Measurement", this);
    buttonStoreMeasurement_->setEnabled(false);
    vlayout_buttons->addWidget(buttonStoreMeasurement_);

    layout->addLayout(vlayout_buttons);

    connect(buttonStoreMeasurement_, SIGNAL(clicked()), this, SLOT(storeResults()));

    generateCirclePositions();
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
  //  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "in generatePositions ";

  //read in number of steps, if empty take default 10 steps
  nstepsx = (nstepsx_->text().isEmpty() == true) ? 10 : (nstepsx_->text().toInt());
  nstepsy = (nstepsy_->text().isEmpty() == true) ? 10 : (nstepsy_->text().toInt());


  //  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "nstepsx = " << nstepsx << " nstepsy = " << nstepsy << " x_init = " << x_init << " y_init = " << y_init << " rangex = " << rangex << " rangey = "<< rangey;

  std::vector<float> xpos;
  std::vector<float> ypos;
  std::vector<float> zpos;
  std::vector<float> pos_number;
  std::vector<float> meas;
  
  int z = 0;
  /*
  //generate the positions in zigzag pattern
  for(int i = 0; i < nstepsx + 1; i++){
    for(int j = 0; j < nstepsy + 1; j++){
      xpos.push_back(x_init + i*rangex/nstepsx);
      ypos.push_back(y_init + j*rangey/nstepsy);
      pos_number.push_back(z);
      zpos.push_back(z_init);
      meas.push_back(0.0);
      z++;
    }
  }
  */

  //generate the positions in meander pattern
  float y_prev = y_init;
  float y_pres = y_init;
  for(int i = 0; i < nstepsx + 1; i++){
    for(int j = 0; j < nstepsy + 1; j++){
      xpos.push_back(x_init + i*rangex/nstepsx);
      if(j != 0){y_pres = y_prev + pow(-1,i)*rangey/nstepsy;}
      ypos.push_back(y_pres);
      y_prev = y_pres;
      pos_number.push_back(z);
      zpos.push_back(z_init);
      meas.push_back(0.0);
      z++;
    }
  }

  //  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "size pos_number = "<<pos_number.size()<<" size xpos = "<<xpos.size();
  //store the positions in the table_model as columns
  table_model->insertData(0, pos_number);
  table_model->insertData(1, xpos);
  table_model->insertData(2, ypos);
  table_model->insertData(3, zpos);
  table_model->insertData(4, meas);
  table_model->update();

  //  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "filled positions ";

  //  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "filled table ";

  buttonStartMeasurement_->setEnabled(true);
  buttonStopMeasurement_->setEnabled(true);
  isActive_ = true;
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
  isActive_ = false;
}

//FIX ME! needs to be tested in the lab
void LStepExpressMeasurementWidget::performMeasurement()
{
  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "starting scan";

  double x_pos = 0;
  double y_pos = 0;
  double z_pos = 0;
  std::vector<float> meas;
  //go to all positions, do the measurement, store the measurement data
  int r = 0;
  while(isActive_ && r < table_model->rowCount())
    //  for(int r = 0; r < table_model->rowCount(); r++)
    {
      //retrieve position from the model
      x_pos = table_model->data(table_model->index(r,1), Qt::DisplayRole).toInt();
      y_pos = table_model->data(table_model->index(r,2), Qt::DisplayRole).toInt();
      z_pos = table_model->data(table_model->index(r,3), Qt::DisplayRole).toInt();

      //go to position
      //manager_->moveAbsolute(x_pos, y_pos, z_pos, 0.0);

      //if requested average measurement, perform circle at position
      double meas_atpos = 0.;
      NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "averageMeasEnabled_ = " << averageMeasEnabled_;
      if(averageMeasEnabled_)
	{
	  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "in average meas, circle_x size = " << circle_x.size();
	  for(unsigned int c = 0; c < circle_x.size(); c++)
	    {
	      //manager_->moveAbsolute(x_pos+(circle_x)[c], y_pos+(circle_y)[c], z_pos, 0.0);
	      //make laser measurement
	    }
	  meas_atpos = 1.;
	}else{
	NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "in standard meas ";
	//make laser measurement
	meas_atpos = 1.;
      }
      

      NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "after meas ";
      //FIX ME! how to ensure the movement has stopped before making measurement with laser?

      //store dummy measurement result
      meas.push_back(meas_atpos);
      r++;
    }
  table_model->insertData(4,meas);
  table_model->update();

  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "scan finished";

  buttonStoreMeasurement_->setEnabled(true);
}

//FIX ME! store results in which format? ROOT file, text file, mathematica file?
//for now use simple text file
void LStepExpressMeasurementWidget::storeResults()
{
  QString filename = QFileDialog::getSaveFileName(this, "Write Settings", "~/", "*.lstep");
  if (filename.isNull()) return;

  QFile data(filename);
  if (data.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream out(&data);

    out << "average measurement was performed: " << averageMeasEnabled_ << "\n";
    out << "# \t x-pos \t y-pos \t z-pos \t measurement\n";
    for (int r = 0; r < table_model->rowCount(); r++)
      {
	out << table_model->data(table_model->index(r,0), Qt::DisplayRole).toInt() << "\t" << table_model->data(table_model->index(r,1), Qt::DisplayRole).toInt() 
	    << "\t" << table_model->data(table_model->index(r,2), Qt::DisplayRole).toInt() << "\t" << table_model->data(table_model->index(r,3), Qt::DisplayRole).toInt()
	    << "\t" << table_model->data(table_model->index(r,4), Qt::DisplayRole).toInt() << "\n";
      }
    }

}

