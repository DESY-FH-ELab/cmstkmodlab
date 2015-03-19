#include "DefoConradWidget.h"
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGridLayout>

#include  <sstream>


DefoConradWidget::DefoConradWidget(DefoConradModel* model,
				   QWidget *parent)
  : QWidget(parent)
{
  QVBoxLayout* layout = new QVBoxLayout(this);

  layout->addWidget(new DefoConradDeviceCheckbox(model, "Enable switch", this));

  QGridLayout* glayout = new QGridLayout();
  glayout->setSpacing(1);

  DefoConradSwitchButton* button;

  glayout->addWidget(button = new DefoConradSwitchButton(model,
							 static_cast<DefoConradModel::DeviceSwitch>(2),
							 QString("Panel 3"),
							 this),
		     0, 0, 2, 1);
  button->setMinimumWidth(100);
  button->setMinimumHeight(200);

  glayout->addWidget(button = new DefoConradSwitchButton(model,
							 static_cast<DefoConradModel::DeviceSwitch>(0),
							 QString("Panel 1"),
							 this),
		     0, 1, 1, 1);
  button->setMinimumWidth(100);
  button->setMinimumHeight(100);

  glayout->addWidget(button = new DefoConradSwitchButton(model,
							 static_cast<DefoConradModel::DeviceSwitch>(1),
							 QString("Panel 2"),
							 this),
		     1, 1, 1, 1);
  button->setMinimumWidth(100);
  button->setMinimumHeight(100);

  glayout->addWidget(button = new DefoConradSwitchButton(model,
							 static_cast<DefoConradModel::DeviceSwitch>(3),
							 QString("Panel 4"),
							 this),
		     0, 2, 2, 1);
  button->setMinimumWidth(100);
  button->setMinimumHeight(200);

  glayout->addWidget(button = new DefoConradSwitchButton(model,
							 static_cast<DefoConradModel::DeviceSwitch>(4),
							 QString("Panel 5"),
							 this),
		     2, 0, 1, 3);
  button->setMinimumWidth(300);
  button->setMinimumHeight(100);

  layout->addLayout(glayout);

  for ( int i = 0; i < 5; ++i)
    layout->addWidget(new DefoConradSwitchCheckbox(model,
						   static_cast<DefoConradModel::DeviceSwitch>(i),
						   QString("Power panel ") + QString::number(i+1),
						   this));

  layout->addWidget(new DefoConradSwitchCheckbox(model,
						 DefoConradModel::CALIBRATION_LEDS,
						 "Power calibration leds",
						 this));

  layout->addWidget(new DefoConradSwitchCheckbox(model,
						 DefoConradModel::CAMERA,
						 "Power camera",
						 this));

  setLayout(layout);
}
