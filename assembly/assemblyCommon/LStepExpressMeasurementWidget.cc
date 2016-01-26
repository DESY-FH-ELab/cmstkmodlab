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
    // fix me!! z position should be obtained from the LStepExpressModel position
    z_init = model->getPosition(2); //is 2 z-axis?
    x_init = -150; //take right upper corner of table
    y_init = -150; //take right upper corner of table

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
  //FIX ME! probably bug here, as when nstepsx_ and nstepsy_ are filled, pushing the button makes the code crash
  nstepsx = 2;//nstepsx_->text().toInt();
  nstepsy = 2;//nstepsy_->text().toInt();

  std::vector<double> xpos;
  std::vector<double> ypos;

  QTableWidget *tab = new QTableWidget(4,nstepsx*nstepsy,this);
  QStringList tab_header; 
  tab_header << "#" << "x-pos"<<"y-pos"<<"meas";

  for(int i = 0; i < nstepsx; i++){
    xpos[i] = x_init + rangex/nstepsx;
  }
  for(int j = 0; j < nstepsy; j++){
    ypos[j] = y_init + rangey/nstepsy;
  }
  
  int z = 0;
  for(int i = 0; i < nstepsx; i++){
    for(int j = 0; j < nstepsy; j++)
      {
	tab->setItem(z,0, new QTableWidgetItem(QString::number(z)));
	tab->setItem(z,1, new QTableWidgetItem(QString::number(xpos[i])));
	tab->setItem(z,2, new QTableWidgetItem(QString::number(ypos[j])));
	z++;
      }
  }
  


  
  //open a new window which shows the generated positions and asks for confirmation to start the measurement
  //MeanderMeasurementDialog *dialog = new MeanderMeasurementDialog();
  //dialog->show();

  //  QMessageBox::StandardButton reply;
  //reply = QMessageBox::question(this, "Meander Measurement", "Proceed with measurement?", QMessageBox::Yes | QMessageBox::No);

  //  if(reply == QMessageBox::Yes){
  //  performMeasurement(xpos, ypos);
  //}

}

void LStepExpressMeasurementWidget::performMeasurement(std::vector<double> x, std::vector<double> y)
{
}


