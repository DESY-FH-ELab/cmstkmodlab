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

  QDoubleSpinBox* position_;
  QDoubleSpinBox* speed_;
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

class NanotecSMCI36LinearStageWidget : public QWidget
{
    Q_OBJECT
public:
  explicit NanotecSMCI36LinearStageWidget(NanotecSMCI36LinearStageModel* model, QWidget *parent = 0);

protected:

  NanotecSMCI36LinearStageModel* model_;

  NanotecSMCI36LinearStageStatusWidget* stageStatus_;
  NanotecSMCI36LinearStageMovementWidget* stageMovement_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // NANOTECSMCI36LINEARSTAGEWIDGET_H
