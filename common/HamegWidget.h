#ifndef HAMEGWIDGET_H
#define HAMEGWIDGET_H

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

#include "HamegModel.h"
#include "DeviceState.h"

class HamegChannelWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit HamegChannelWidget(HamegModel* model, int channel, QWidget *parent = 0);

protected:
    const static int LCD_SIZE = 8;

    HamegModel* model_;
    int channel_;

    QRadioButton* cvModeButton_;
    QRadioButton* ccModeButton_;

    QLCDNumber* voltageDisplay_;
    QLCDNumber* currentDisplay_;

    QDoubleSpinBox* voltageSpinner_;
    QDoubleSpinBox* currentSpinner_;

protected slots:

    void modeChanged(int button);
    void voltageSpinnerChanged(double);
    void currentSpinnerChanged(double);

    void updateDeviceState(State);
    void controlStateChanged(bool);
    void updateInfo();
};

class HamegWidget : public QWidget
{
    Q_OBJECT
public:
  explicit HamegWidget(HamegModel* model, QWidget *parent = 0);

protected:
  HamegModel* model_;

  QCheckBox* hamegCheckBox_;
  QCheckBox* hamegRemoteBox_;
  QCheckBox* hamegOutputBox_;

  QWidget* operationPanel_;

  HamegChannelWidget* channel1_;
  HamegChannelWidget* channel2_;

public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // HAMEGWIDGET_H
