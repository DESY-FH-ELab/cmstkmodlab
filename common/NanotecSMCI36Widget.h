#ifndef NANOTECSMCI36WIDGET_H
#define NANOTECSMCI36WIDGET_H

#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTabWidget>

#include "NanotecSMCI36Model.h"
#include "DeviceState.h"

class NanotecSMCI36StepModeWidget : public QComboBox
{
  Q_OBJECT
public:
  explicit NanotecSMCI36StepModeWidget(NanotecSMCI36Model* model,
                                       QWidget *parent = 0);

protected:

  NanotecSMCI36Model * model_;

protected slots:

  void indexChanged(int);
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36ErrorCorrectionModeWidget : public QComboBox
{
  Q_OBJECT
public:
  explicit NanotecSMCI36ErrorCorrectionModeWidget(NanotecSMCI36Model* model,
                                                  QWidget *parent = 0);

protected:

  NanotecSMCI36Model * model_;

protected slots:

  void indexChanged(int);
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36RampModeWidget : public QComboBox
{
  Q_OBJECT
public:
  explicit NanotecSMCI36RampModeWidget(NanotecSMCI36Model* model,
                                       QWidget *parent = 0);

protected:

  NanotecSMCI36Model * model_;

protected slots:

  void indexChanged(int);
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36PositioningModeWidget : public QComboBox
{
  Q_OBJECT
public:
  explicit NanotecSMCI36PositioningModeWidget(NanotecSMCI36Model* model,
                                              QWidget *parent = 0);

protected:

  NanotecSMCI36Model * model_;

protected slots:

  void indexChanged(int);
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36StatusWidget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36StatusWidget(NanotecSMCI36Model* model, QWidget *parent = 0);

protected:
  NanotecSMCI36Model* model_;

  QLabel* status_;
  QLabel* controllerSteps_;
  QLabel* encoderSteps_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36SettingsWidget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36SettingsWidget(NanotecSMCI36Model* model, QWidget *parent = 0);

protected:
  NanotecSMCI36Model* model_;

  QDoubleSpinBox* pitch_;
  QSpinBox* motorID_;

  NanotecSMCI36StepModeWidget* stepMode_;
  NanotecSMCI36ErrorCorrectionModeWidget* errorCorrectionMode_;
  QSpinBox* maxEncoderDeviation_;
  NanotecSMCI36RampModeWidget* rampMode_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36MovementWidget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36MovementWidget(NanotecSMCI36Model* model, QWidget *parent = 0);

protected:
  NanotecSMCI36Model* model_;

  QCheckBox* stageMode_;

  NanotecSMCI36PositioningModeWidget* positioningMode_;

  QTabWidget* modeTabs_;

  QCheckBox* direction_;
  QDoubleSpinBox* travelDistance_;
  QDoubleSpinBox* minFrequency_;
  QDoubleSpinBox* maxFrequency_;
  QDoubleSpinBox* maxFrequency2_;

  QDoubleSpinBox* travelDistanceInMM_;
  QDoubleSpinBox* minSpeed_;
  QDoubleSpinBox* maxSpeed_;
  QDoubleSpinBox* maxSpeed2_;

  QLabel* minPosition_;
  QLabel* maxPosition_;

  QPushButton* start_;
  QPushButton* stop_;
  QPushButton* quickstop_;
  QPushButton* resetPositionError_;

public slots:

  void stageModeChanged(bool);
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36Widget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36Widget(NanotecSMCI36Model* model, QWidget *parent = 0);

protected:
  NanotecSMCI36Model* model_;

  QCheckBox* smci36CheckBox_;

  NanotecSMCI36StatusWidget* scmi36Status_;
  NanotecSMCI36SettingsWidget* scmi36Settings_;
  NanotecSMCI36MovementWidget* scmi36Movement_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // NANOTECSMCI36WIDGET_H
