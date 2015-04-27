#include "DefoConradPanelWidget.h"
#include <QGridLayout>

#include  <sstream>


DefoConradPanelWidget::DefoConradPanelWidget(DefoConradModel* model,
					     QWidget *parent)
  : QWidget(parent)
{
  QGridLayout* layout = new QGridLayout(this);
  layout->setSpacing(1);

  DefoConradSwitchButton* button;

  layout->addWidget(button = new DefoConradSwitchButton(model,
							static_cast<DefoConradModel::DeviceSwitch>(2),
							QString("Panel 3"),
							this),
		    0, 0, 2, 1);
  button->setMinimumWidth(100);
  button->setMinimumHeight(200);
  
  layout->addWidget(button = new DefoConradSwitchButton(model,
							static_cast<DefoConradModel::DeviceSwitch>(0),
							QString("Panel 1"),
							this),
		    0, 1, 1, 1);
  button->setMinimumWidth(100);
  button->setMinimumHeight(100);

  layout->addWidget(button = new DefoConradSwitchButton(model,
							static_cast<DefoConradModel::DeviceSwitch>(1),
							QString("Panel 2"),
							this),
		    1, 1, 1, 1);
  button->setMinimumWidth(100);
  button->setMinimumHeight(100);

  layout->addWidget(button = new DefoConradSwitchButton(model,
							static_cast<DefoConradModel::DeviceSwitch>(3),
							QString("Panel 4"),
							this),
		    0, 2, 2, 1);
  button->setMinimumWidth(100);
  button->setMinimumHeight(200);

  layout->addWidget(button = new DefoConradSwitchButton(model,
							static_cast<DefoConradModel::DeviceSwitch>(4),
							QString("Panel 5"),
							this),
		    2, 0, 1, 3);
  button->setMinimumWidth(300);
  button->setMinimumHeight(100);

  setLayout(layout);
}
