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
  setMinimumSize(QSize(200, 300));

  QBoxLayout *layout = new QVBoxLayout();
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  setLayout(layout);

  QPushButton * reconstructButton = new QPushButton("&Reconstruct", this);
  connect(reconstructButton, SIGNAL(clicked()),
          this, SLOT(reconstructButtonClicked()));
  layout->addWidget(reconstructButton);

  connect(
          reconstructionModel_
        , SIGNAL(setupChanged())
        , this
        , SLOT(setupChanged())
  	  );
}

void DefoReconstructionWidget::setupChanged() {

}

void DefoReconstructionWidget::reconstructButtonClicked() {
  reconstructionModel_->reconstruct();
}
