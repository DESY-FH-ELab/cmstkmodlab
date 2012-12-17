#include <QBoxLayout>
#include <QPushButton>

#include "DefoAnalysisWidget.h"

DefoAnalysisWidget::DefoAnalysisWidget(
     DefoMeasurementPairListModel * listModel
   , DefoMeasurementPairSelectionModel * selectionModel
   , QWidget *parent
) :
    QWidget(parent)
  , listModel_(listModel)
  , selectionModel_(selectionModel)
{
  setMinimumSize(QSize(300, 300));

  QBoxLayout *layout = new QVBoxLayout();
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  setLayout(layout);

  plot3D_ = new DefoSurfacePlot(this);
  layout->addWidget(plot3D_);
}
