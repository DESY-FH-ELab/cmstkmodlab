#include "DefoConradWidget.h"
#include <QCheckBox>
#include <QVBoxLayout>

#include  <sstream>


DefoConradWidget::DefoConradWidget(DefoConradModel* model, QWidget *parent) :
    QWidget(parent)
{

  QLayout* layout = new QVBoxLayout(this);

  layout->addWidget(new DefoConradDeviceCheckbox(model, "Enable switch", this));

  for ( int i = 0; i < 5; ++i)
    layout->addWidget(
        new DefoConradSwitchCheckbox(
              model
            , static_cast<DefoConradModel::DeviceSwitch>(i)
            , QString("Enable panel ") + QString::number(i+1)
            , this
        )
    );

  layout->addWidget(
      new DefoConradSwitchCheckbox(
            model
          , DefoConradModel::CALIBRATION_LEDS
          , "Enable calibration leds"
          , this
      )
  );

  layout->addWidget(
      new DefoConradSwitchCheckbox(
            model
          , DefoConradModel::CAMERA
          , "Enable camera"
          , this
      )
  );

  setLayout(layout);

}
