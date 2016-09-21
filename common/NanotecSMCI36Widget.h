#ifndef NANOTECSMCI36WIDGET_H
#define NANOTECSMCI36WIDGET_H

#include <QCheckBox>
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

#include "NanotecSMCI36Model.h"
#include "DeviceState.h"

class NanotecSMCI36Widget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36Widget(NanotecSMCI36Model* model, QWidget *parent = 0);

protected:
  NanotecSMCI36Model* model_;

  QCheckBox* smci36CheckBox_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // HAMEGWIDGET_H
