#ifndef DEFOCONRADACTION_H
#define DEFOCONRADACTION_H

#include <QCheckBox>
#include "DefoConradModel.h"


/// Abstract action class for use with DefoConradModel
class DefoConradCheckbox : public QCheckBox
{
    Q_OBJECT
public:
  explicit DefoConradCheckbox(
      DefoConradModel* model
    , const QString& text
    , QWidget *parent = 0
  );

protected:
  DefoConradModel* model_;

public slots:
  virtual void deviceStateChanged(State newState) = 0;
  virtual void controlStateChanged(bool) = 0;
};



/// Action class for controlling the Conrad relais switch
class DefoConradDeviceCheckbox : public DefoConradCheckbox
{
    Q_OBJECT
public:
  explicit DefoConradDeviceCheckbox(
      DefoConradModel* model
    , const QString& text
    , QWidget *parent = 0
  );

public slots:
  virtual void deviceStateChanged(State newState);
  virtual void controlStateChanged(bool);
//  virtual void setChecked( bool checked );

};



/**
  Action class for controlling the devices connected to the Conrad relais
  switch.
  */
class DefoConradSwitchCheckbox : public DefoConradCheckbox
{
    Q_OBJECT
public:
  explicit DefoConradSwitchCheckbox(
      DefoConradModel* model
    , const DefoConradModel::DeviceSwitch& device
    , const QString& text
    , QWidget *parent = 0
  );

protected:
  DefoConradModel::DeviceSwitch device_;

public slots:
  virtual void deviceStateChanged(State newState);
  virtual void controlStateChanged(bool);
  void switchStateChanged(
      DefoConradModel::DeviceSwitch device
    , State newState
  );

protected slots:
  virtual void buttonToggled( bool checked );

};

#endif // DEFOCONRADACTION_H
