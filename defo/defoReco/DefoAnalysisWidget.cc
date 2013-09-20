#include <QBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>

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
  plot3D_->setMinimumSize(QSize(800, 600));
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

  QGroupBox * gb = new QGroupBox(this);
  QVBoxLayout *vbox = new QVBoxLayout;

  QPushButton * push = new QPushButton("+");
  connect(push, SIGNAL(clicked()),
          plot3D_, SLOT(increaseZScale()));
  vbox->addWidget(push);

  push = new QPushButton("-");
  connect(push, SIGNAL(clicked()),
          plot3D_, SLOT(decreaseZScale()));
  vbox->addWidget(push);

  QSpinBox * spin = new QSpinBox();
  spin->setRange(5, 20);
  spin->setValue(10);
  connect(spin, SIGNAL(valueChanged(int)),
          plot3D_, SLOT(setNIsolines(int)));
  vbox->addWidget(spin);

  QCheckBox* checkbox = new QCheckBox("3D");
  checkbox->setChecked(true);
  connect(checkbox, SIGNAL(toggled(bool)),
          plot3D_, SLOT(toggleView(bool)));
  vbox->addWidget(checkbox);

  checkbox = new QCheckBox("shaded");
  checkbox->setChecked(true);
  connect(checkbox, SIGNAL(toggled(bool)),
          plot3D_, SLOT(setIsShade(bool)));
  vbox->addWidget(checkbox);

  checkbox = new QCheckBox("mesh");
  checkbox->setChecked(true);
  connect(checkbox, SIGNAL(toggled(bool)),
          plot3D_, SLOT(setIsMesh(bool)));
  vbox->addWidget(checkbox);

  checkbox = new QCheckBox("axes");
  checkbox->setChecked(true);
  connect(checkbox, SIGNAL(toggled(bool)),
          plot3D_, SLOT(setIsAxes(bool)));
  vbox->addWidget(checkbox);

  checkbox = new QCheckBox("isolines");
  checkbox->setChecked(true);
  connect(checkbox, SIGNAL(toggled(bool)),
          plot3D_, SLOT(setIsIsolines(bool)));
  vbox->addWidget(checkbox);

  checkbox = new QCheckBox("legend");
  checkbox->setChecked(true);
  connect(checkbox, SIGNAL(toggled(bool)),
          plot3D_, SLOT(setIsLegend(bool)));
  vbox->addWidget(checkbox);

  vbox->addStretch(1);
  gb->setLayout(vbox);

  layout->addWidget(gb);

  connect(selectionModel_, SIGNAL(selectionChanged(DefoMeasurementPair*)),
          this, SLOT(selectionChanged(DefoMeasurementPair*)));
}

void DefoAnalysisWidget::selectionChanged(DefoMeasurementPair* selection)
{
  if (!selection) return;

  DefoSurfacePlotViewSettings *vs;

  std::map<DefoMeasurementPair*,DefoSurfacePlotViewSettings*>::iterator it = viewSettings_.find(selection);
  if (it!=viewSettings_.end()) {
      vs = it->second;
  } else {
      vs = new DefoSurfacePlotViewSettings();
      viewSettings_[selection] = vs;
  }

  const DefoSurface& surface = selection->getSurface();

  plot3D_->setViewSettings(vs);
  plot3D_->setData(surface);
  plot3D_->draw();
}
