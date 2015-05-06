#ifndef KEITHLEYWIDGET_H
#define KEITHLEYWIDGET_H

#include <vector>

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>

#include "KeithleyModel.h"

class KeithleyUpdateIntervalBox : public QComboBox
{
  Q_OBJECT

public:
    explicit KeithleyUpdateIntervalBox(KeithleyModel* model, QWidget *parent = 0);

protected slots:
    void currentItemChanged(int);

protected:
    KeithleyModel* model_;

signals:
    void valueChanged(int);
};

class KeithleyWidget : public QWidget
{
  Q_OBJECT

public:
  explicit KeithleyWidget(KeithleyModel* model, QWidget *parent = 0);

protected:
  KeithleyModel* model_;
  QCheckBox* keithleyCheckBox_;
  KeithleyUpdateIntervalBox* updateIntervalBox_;
  QWidget* sensorControlWidget_;

public slots:
  void keithleyStateChanged(State newState);
  void controlStateChanged(bool);
};

class KeithleyTemperatureWidget : public QWidget
{
  Q_OBJECT

public:
  explicit KeithleyTemperatureWidget(KeithleyModel* model_,
                                     unsigned int sensor,
                                     QWidget *parent = 0);

protected:
  KeithleyModel* model_;
  unsigned int sensor_;

  static const unsigned int LCD_SIZE;
  static const QString LABEL_FORMAT;

  QFormLayout* layout_;
  QCheckBox* enabledCheckBox_;
  QLabel* currentTempLabel_;
  QLCDNumber* currentTempDisplay_;
  QLabel* currentGradientLabel_;
  QLCDNumber* currentGradientDisplay_;

  void updateWidgets();

public slots:
  void keithleyStateChanged( State state );
  void controlStateChanged(bool);
  void sensorStateChanged(uint sensor, State state );
  void sensorTemperatureChanged(uint sensor, double temperature);
  void sensorTemperatureGradientChanged(uint sensor, double temperature);

  void enabledCheckBoxToggled(bool enabled);
};

#endif // KEITHLEYWIDGET_H
