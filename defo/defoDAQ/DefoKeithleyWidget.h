#ifndef DEFOKEITHLEYWIDGET_H
#define DEFOKEITHLEYWIDGET_H

#include <vector>

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QWidget>

#include "DefoKeithleyModel.h"
#include "DefoTemperatureHistoryModel.h"
#include "DefoTemperatureHistoryWidget.h"

class DefoKeithleyWidget : public QWidget
{
  Q_OBJECT

public:
  explicit DefoKeithleyWidget(DefoKeithleyModel* model, QWidget *parent = 0);

protected:
  DefoKeithleyModel* model_;
  QCheckBox* keithleyCheckBox_;
  QWidget* sensorControlWidget_;

public slots:
  void keithleyStateChanged(State newState);
  void controlStateChanged(bool);

};

class DefoKeithleyTemperatureWidget : public QWidget
{
  Q_OBJECT

public:
  explicit DefoKeithleyTemperatureWidget(
      DefoKeithleyModel* model_
    , unsigned int sensor
    , QWidget *parent = 0
  );
//  ~DefoKeithleyTemperatureWidget();

protected:
  DefoKeithleyModel* model_;
  unsigned int sensor_;

  static const QString LABEL_FORMAT;

  QCheckBox* enabledCheckBox_;

  DefoTemperatureHistoryModel* historyModel_;
  DefoTemperatureHistoryWidget* historyWidget_;

  void updateWidgets();

public slots:
  void keithleyStateChanged( State state );
  void controlStateChanged(bool);
  void sensorStateChagned( uint sensor, State state );
  void sensorTemperatureChanged( uint sensor, double temperature );

  void enabledCheckBoxToggled( bool enabled );

};

#endif // DEFOKEITHLEYWIDGET_H
