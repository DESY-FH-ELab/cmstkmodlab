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
  QLabel* stepMode_;

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

  NanotecSMCI36PositioningModeWidget* positioningMode_;

  QCheckBox* direction_;
  QSpinBox* travelDistance_;
  QSpinBox* minFrequency_;
  QSpinBox* maxFrequency_;
  QSpinBox* maxFrequency2_;

  QPushButton* start_;
  QPushButton* stop_;
  QPushButton* quickstop_;
  QPushButton* resetPositionError_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36InputStateWidget : public QLabel
{
  Q_OBJECT
public:
  explicit NanotecSMCI36InputStateWidget(NanotecSMCI36Model* model,
                                         int pin,
                                         QWidget *parent = 0);

protected:

  NanotecSMCI36Model * model_;
  int pin_;

protected slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36OutputStateWidget : public QCheckBox
{
  Q_OBJECT
public:
  explicit NanotecSMCI36OutputStateWidget(NanotecSMCI36Model* model,
                                          int pin,
                                          QWidget *parent = 0);

protected:

  NanotecSMCI36Model * model_;
  int pin_;

protected slots:

  void outputChanged(int);
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36InputWidget : public QWidget
{
  Q_OBJECT
public:
  explicit NanotecSMCI36InputWidget(NanotecSMCI36Model* model,
                                    int pin,
                                    QWidget *parent = 0);

protected:

  NanotecSMCI36Model * model_;
  int pin_;

protected slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36OutputWidget : public QWidget
{
  Q_OBJECT
public:
  explicit NanotecSMCI36OutputWidget(NanotecSMCI36Model* model,
                                     int pin,
                                     QWidget *parent = 0);

protected:

  NanotecSMCI36Model * model_;
  int pin_;

protected slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36IOWidget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36IOWidget(NanotecSMCI36Model* model, QWidget *parent = 0);

protected:
  NanotecSMCI36Model* model_;

public slots:

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
  NanotecSMCI36IOWidget* smci36IO_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // NANOTECSMCI36WIDGET_H
