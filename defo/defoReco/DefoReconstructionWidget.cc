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
}

void DefoReconstructionWidget::setupChanged() {

}
