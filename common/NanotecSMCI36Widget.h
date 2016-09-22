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

  QDoubleSpinBox* pitch_;
  QLabel* status_;
  QSpinBox* motorID_;
  QLabel* controllerSteps_;
  QLabel* encoderSteps_;

  NanotecSMCI36StepModeWidget* stepMode_;
  NanotecSMCI36ErrorCorrectionModeWidget* errorCorrectionMode_;
  QSpinBox* maxEncoderDeviation_;
  NanotecSMCI36RampModeWidget* rampMode_;
  NanotecSMCI36PositioningModeWidget* positioningMode_;

  QCheckBox* direction_;
  QDoubleSpinBox* travelDistance_;
  QDoubleSpinBox* minFrequency_;
  QDoubleSpinBox* maxFrequency_;
  QDoubleSpinBox* maxFrequency2_;

  QDoubleSpinBox* travelDistanceInMM_;
  QDoubleSpinBox* minSpeed_;
  QDoubleSpinBox* maxSpeed_;
  QDoubleSpinBox* maxSpeed2_;

  QPushButton* start_;
  QPushButton* stop_;
  QPushButton* quickstop_;
  QPushButton* resetPositionError_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // NANOTECSMCI36WIDGET_H
