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
            , QString("Power panel ") + QString::number(i+1)
            , this
        )
    );

  layout->addWidget(
      new DefoConradSwitchCheckbox(
            model
          , DefoConradModel::CALIBRATION_LEDS
          , "Power calibration leds"
          , this
      )
  );

  layout->addWidget(
      new DefoConradSwitchCheckbox(
            model
          , DefoConradModel::CAMERA
          , "Power camera"
          , this
      )
  );

  setLayout(layout);

}
