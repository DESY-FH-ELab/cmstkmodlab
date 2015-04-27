#ifndef DEFOCONRADBUTTON_H
#define DEFOCONRADBUTTON_H

#include <QPushButton>
#include "DefoConradModel.h"

/// Abstract action class for use with DefoConradModel
class DefoConradButton : public QPushButton
{
    Q_OBJECT

public:

  explicit DefoConradButton(DefoConradModel* model,
			    const QString& text,
			    QWidget *parent = 0);

public slots:

  virtual void deviceStateChanged(State newState) = 0;
  virtual void controlStateChanged(bool) = 0;

protected:

  DefoConradModel* model_;
};

/**
  Action class for controlling the devices connected to the Conrad relais
  switch.
  */
class DefoConradSwitchButton : public DefoConradButton
{
    Q_OBJECT

public:

  explicit DefoConradSwitchButton(DefoConradModel* model,
				  const DefoConradModel::DeviceSwitch& device,
				  const QString& text,
				  QWidget *parent = 0);

public slots:

  virtual void deviceStateChanged(State newState);
  virtual void controlStateChanged(bool);
  void switchStateChanged(DefoConradModel::DeviceSwitch device,
			  State newState);

protected:

  DefoConradModel::DeviceSwitch device_;

protected slots:

  virtual void buttonToggled( bool checked );
};

#endif // DEFOCONRADBUTTON_H
