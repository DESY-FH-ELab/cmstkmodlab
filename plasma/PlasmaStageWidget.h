/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef PLASMASTAGEWIDGET_H
#define PLASMASTAGEWIDGET_H

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
#include "NanotecSMCI36LinearStageWidget.h"

class PlasmaStageOutputStateWidget : public QLabel
{
  Q_OBJECT
public:
  explicit PlasmaStageOutputStateWidget(NanotecSMCI36LinearStageModel* model,
                                                     int pin,
                                                     QWidget *parent = 0);

protected:

  NanotecSMCI36LinearStageModel * model_;
  int pin_;

  QPixmap buttonRed_;
  QPixmap buttonGreen_;
  QPixmap buttonGrey_;

  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

protected slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();

signals:

  void setOutputPinState(int, bool);
};

class PlasmaStageIOWidget : public QWidget
{
    Q_OBJECT
public:
  explicit PlasmaStageIOWidget(NanotecSMCI36LinearStageModel* model, QWidget *parent = 0);

protected:
  NanotecSMCI36LinearStageModel* model_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class PlasmaStageWidget : public QWidget
{
    Q_OBJECT
public:
  explicit PlasmaStageWidget(NanotecSMCI36LinearStageModel* model, QWidget *parent = 0);

protected:

  NanotecSMCI36LinearStageModel* model_;

  NanotecSMCI36LinearStageStatusWidget* stageStatus_;
  NanotecSMCI36LinearStageMovementWidget* stageMovement_;
  PlasmaStageIOWidget* stageIO_;

public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // PLASMASTAGEWIDGET_H
