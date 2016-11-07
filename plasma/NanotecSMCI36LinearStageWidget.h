#ifndef NANOTECSMCI36LINEARSTAGEWIDGET_H
#define NANOTECSMCI36LINEARSTAGEWIDGET_H

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
#include <QPixmap>

#include "PlasmaStepDoubleSpinBox.h"
#include "NanotecSMCI36LinearStageModel.h"

class NanotecSMCI36LinearStageStatusWidget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36LinearStageStatusWidget(NanotecSMCI36LinearStageModel* model,
                                                QWidget *parent = 0);

protected:

  NanotecSMCI36LinearStageModel* model_;

  QLabel* status_;
  QLCDNumber* position_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36LinearStageMovementWidget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36LinearStageMovementWidget(NanotecSMCI36LinearStageModel* model,
                                                  QWidget *parent = 0);

protected:

  NanotecSMCI36LinearStageModel* model_;

  PlasmaStepDoubleSpinBox* position_;
  PlasmaStepDoubleSpinBox* speed_;
  QPushButton *moveButton_;

  QPushButton *refRunButton_;
  QPushButton *stopButton_;
  QPushButton *quickstopButton_;
  QPushButton *resetErrorButton_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
  void limitsChanged();

protected slots:

  void moveButtonClicked();

signals:

  void requestMove(double);
};

class NanotecSMCI36LinearStageInputStateWidget : public QLabel
{
  Q_OBJECT
public:
  explicit NanotecSMCI36LinearStageInputStateWidget(NanotecSMCI36LinearStageModel* model,
                                                    int pin,
                                                    QWidget *parent = 0);

protected:

  NanotecSMCI36LinearStageModel * model_;
  int pin_;

  QPixmap buttonRed_;
  QPixmap buttonGreen_;
  QPixmap buttonGrey_;

protected slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36LinearStageOutputStateWidget : public QLabel
{
  Q_OBJECT
public:
  explicit NanotecSMCI36LinearStageOutputStateWidget(NanotecSMCI36LinearStageModel* model,
                                                     int pin,
                                                     QWidget *parent = 0);

protected:

  NanotecSMCI36LinearStageModel * model_;
  int pin_;

  QPixmap buttonRed_;
  QPixmap buttonGreen_;
  QPixmap buttonGrey_;

  void mousePressEvent(QMouseEvent* event);

protected slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();

signals:

  void toggleOutputPin(int);
};

class NanotecSMCI36LinearStageIOWidget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36LinearStageIOWidget(NanotecSMCI36LinearStageModel* model, QWidget *parent = 0);

protected:
  NanotecSMCI36LinearStageModel* model_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class NanotecSMCI36LinearStageWidget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36LinearStageWidget(NanotecSMCI36LinearStageModel* model, QWidget *parent = 0);

protected:

  NanotecSMCI36LinearStageModel* model_;

  NanotecSMCI36LinearStageStatusWidget* stageStatus_;
  NanotecSMCI36LinearStageMovementWidget* stageMovement_;
  NanotecSMCI36LinearStageIOWidget* stageIO_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // NANOTECSMCI36LINEARSTAGEWIDGET_H
