#include <QBoxLayout>
#include <QPushButton>

#include "DefoReconstructionWidget.h"

DefoReconstructionWidget::DefoReconstructionWidget(
     DefoReconstructionModel* reconstructionModel
   , QWidget *parent
) :
    QWidget(parent)
  , reconstructionModel_(reconstructionModel)
{
  //setMinimumSize(QSize(200, 300));

  QBoxLayout *layout = new QHBoxLayout();
  //setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
  setLayout(layout);

  QPushButton * reconstructButton = new QPushButton("&Reconstruct", this);
  connect(reconstructButton, SIGNAL(clicked()),
          this, SLOT(reconstructButtonClicked()));
  layout->addWidget(reconstructButton);

  progressBar_ = new QProgressBar(this);
  progressBar_->setMinimum(0);
  progressBar_->setMaximum(12);
  layout->addWidget(progressBar_);

  connect(
          reconstructionModel_
        , SIGNAL(setupChanged())
        , this
        , SLOT(setupChanged())
  	  );

  connect(
          reconstructionModel_
        , SIGNAL(recoProgressChanged(int))
        , this
        , SLOT(recoProgressChanged(int))
          );

  connect(
          reconstructionModel_
        , SIGNAL(incrementProgress())
        , this
        , SLOT(incrementProgress())
          );
}

void DefoReconstructionWidget::setupChanged() {

}

void DefoReconstructionWidget::reconstructButtonClicked() {

  reconstructionModel_->reconstruct();
}

void DefoReconstructionWidget::recoProgressChanged(int progress) {

  std::cout << progress << std::endl;
  progressBar_->setValue(progress);
}

void DefoReconstructionWidget::incrementProgress() {

  recoProgressChanged(progressBar_->value() + 1);
}
