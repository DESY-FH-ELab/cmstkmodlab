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
  setMinimumSize(QSize(300, 300));

  connect(
          reconstructionModel_
        , SIGNAL(setupChanged())
        , this
        , SLOT(setupChanged())
  	  );

  QBoxLayout *layout = new QHBoxLayout();
  QWidget * reconstructionWidget = new QWidget(this);
  reconstructionWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  reconstructionWidget->setLayout(layout);

  QBoxLayout *vbox = new QVBoxLayout();
  QWidget * reconstructionInfoWidget = new QWidget(reconstructionWidget);
  reconstructionInfoWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  reconstructionInfoWidget->setLayout(vbox);
  layout->addWidget(reconstructionInfoWidget);

  QPushButton * reconstructButton = new QPushButton("&Reconstruct", reconstructionInfoWidget);
  connect(reconstructButton, SIGNAL(clicked()),
          this, SLOT(reconstructButtonClicked()));
  vbox->addWidget(reconstructButton);
}

void DefoReconstructionWidget::setupChanged() {

}

void DefoReconstructionWidget::reconstructButtonClicked() {
  reconstructionModel_->reconstruct();
}
