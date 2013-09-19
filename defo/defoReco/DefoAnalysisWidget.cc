#include <QBoxLayout>
#include <QPushButton>

#include <qwt3d_types.h>

#include "DefoAnalysisWidget.h"

DefoAnalysisWidget::DefoAnalysisWidget(DefoMeasurementPairListModel * listModel,
                                       DefoMeasurementPairSelectionModel * selectionModel,
                                       QWidget *parent) :
    QWidget(parent),
    listModel_(listModel),
    selectionModel_(selectionModel)
{
  //setMinimumSize(QSize(300, 300));

  QBoxLayout *layout = new QHBoxLayout();
  layout->setContentsMargins(1,1,1,1);
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  setLayout(layout);

  plot3D_ = new DefoSurfacePlot(this);
  plot3D_->setMinimumSize(QSize(900, 600));
  plot3D_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  plot3D_->setRotation(30,0,15);
  plot3D_->setScale(1,1,1);
  plot3D_->setShift(0.15,0,0);
  plot3D_->setZoom(0.9);

  plot3D_->setCoordinateStyle(Qwt3D::BOX);
  plot3D_->updateData();
  plot3D_->updateGL();

  layout->addWidget(plot3D_);

  plot3D_->updateData();
  plot3D_->updateGL();

  connect(selectionModel_, SIGNAL(selectionChanged(DefoMeasurementPair*)),
          this, SLOT(selectionChanged(DefoMeasurementPair*)));
}

void DefoAnalysisWidget::selectionChanged(DefoMeasurementPair* selection)
{
  if (!selection) return;

  std::cout << "selectionChanged " << selection << std::endl;

  const DefoSurface& surface = selection->getSurface();

  plot3D_->setData(surface);
  plot3D_->draw();
}
