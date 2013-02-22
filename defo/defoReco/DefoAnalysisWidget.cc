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
  //setMinimumSize(QSize(300, 300));

  QBoxLayout *layout = new QHBoxLayout();
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  setLayout(layout);

  plot3D_ = new DefoSurfacePlot(this);
  plot3D_->setMinimumSize(QSize(400, 400));
  plot3D_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

//  plot3D_->setRotation(30,0,15);
//  plot3D_->setScale(1,1,1);
//  plot3D_->setShift(0.15,0,0);
//  plot3D_->setZoom(0.9);

//  plot3D_->setCoordinateStyle(Qwt3D::BOX);
//  plot3D_->updateData();
//  plot3D_->updateGL();

  layout->addWidget(plot3D_);

  //connect(selectionModel_, SIGNAL(selectionChanged(DefoMeasurementPair*)),
  //        this, SLOT(selectionChanged(DefoMeasurementPair*)));
}

void DefoAnalysisWidget::selectionChanged(DefoMeasurementPair* selection) {

  std::cout << "selectionChanged" << std::endl;

  const DefoSurface& surface = selection->getSurface();
  std::cout << "selectionChanged" << std::endl;
  plot3D_->setData(surface);
  std::cout << "selectionChanged" << std::endl;
  plot3D_->draw();

  std::cout << "selectionChanged" << std::endl;
}
