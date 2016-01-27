#include <nqlogger.h>

#include "LStepExpressMeasurementWidget.h"

LStepExpressMeasurementWidget::LStepExpressMeasurementWidget(LStepExpressModel* model,
                                       QWidget *parent)
    : QWidget(parent),
      model_(model)
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
    z_init = model->getPosition(2); //FIX ME! is 2 z-axis?
    x_init = -150; //FIX ME! take right upper corner of table
    y_init = -150; //FIX ME! take right upper corner of table

    //    QTableWidget *tab = new QTableWidget(5,2*2,this);
    //QStringList tab_header;
    //tab_header << "#" << "x-pos"<<"y-pos"<<"z-pos"<<"meas";
    //tab->setHorizontalHeaderLabels(tab_header);

    //test table model/view
    table_model = new LStepExpressMeasurementTable();
    // table_model->insertColumns(0,4); //one column already present
    table_view = new QTableView(this);
    table_view->setModel(table_model);

    layout->addWidget(table_view);
}

void LStepExpressMeasurementWidget::setAverageMeasEnabled(bool enabled)
{
    if (enabled) {
      averageMeasEnabled_ = true;
    } else {
      averageMeasCheckBox_->setEnabled(false);
      averageMeasEnabled_ = false;
    }
}

//generate the x and y positions for the measurement
void LStepExpressMeasurementWidget::generatePositions()
{
  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "in generatePositions ";

  //read in number of steps, if empty take default 10 steps
  nstepsx = (nstepsx_->text().isEmpty() == true) ? 10 : nstepsx_->text().toInt();
  nstepsy = (nstepsy_->text().isEmpty() == true) ? 10 : nstepsy_->text().toInt();


  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "nstepsx = " << nstepsx << " nstepsy = " << nstepsy << " x_init = " << x_init << " y_init = " << y_init << " rangex = " << rangex << " rangey = "<< rangey;

  std::vector<float> xpos;
  std::vector<float> ypos;
  std::vector<float> zpos;
  std::vector<float> pos_number;
  std::vector<float> meas;
  
  int z = 0;
  for(int i = 0; i < nstepsx; i++){
    for(int j = 0; j < nstepsy; j++){
      xpos.push_back(x_init + i*rangex/nstepsx);
      ypos.push_back(y_init + j*rangey/nstepsy);
      pos_number.push_back(z);
      zpos.push_back(z_init);
      meas.push_back(0.0);
      z++;
    }
  }

  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "size pos_number = "<<pos_number.size()<<" size xpos = "<<xpos.size();
  
  table_model->insertData(0, pos_number);
  table_model->insertData(1, xpos);
  table_model->insertData(2, ypos);
  table_model->insertData(3, zpos);
  table_model->insertData(4, meas);
  table_model->update();

  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "filled positions ";

  NQLog("LStepExpressMeasurementWidget", NQLog::Debug) << "filled table ";

  table_view->update();
  table_view->show();
  //  buttonStartMeasurement_ = new QPushButton("Start Measurement", this);
  //layout()->addWidget(buttonStartMeasurement_);

  // connect(buttonStartMeasurement_, SIGNAL(clicked()),
  //	  this, SLOT(performMeasurement()));

}

void LStepExpressMeasurementWidget::performMeasurement(std::vector<double> x, std::vector<double> y)
{
}


