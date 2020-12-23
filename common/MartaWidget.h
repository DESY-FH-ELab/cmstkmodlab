/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef MARTAWIDGET_H
#define MARTAWIDGET_H

#include <QCheckBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <QSvgWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDialog>
#include <QTextEdit>

#include "MartaModel.h"
#include "DeviceState.h"

class MartaSVGWidget : public QSvgWidget
{
  Q_OBJECT
 public:

  MartaSVGWidget(QWidget* parent = 0);
  virtual int heightForWidth(int) const;
  virtual int widthForHeight(int) const;
  
 protected:

  virtual void resizeEvent(QResizeEvent *event);
  virtual void mouseDoubleClickEvent(QMouseEvent *event);

 signals:

  void alarmStatusDoubleClicked();

 private:

  QRect buttonAlarmStatus_;
};

class MartaAlarmDialog : public QDialog
{
  Q_OBJECT
 public:

  MartaAlarmDialog(MartaModel* model,
		   QWidget* parent = 0);
 protected:

  MartaModel* model_;
  
  QTextEdit* alarmView_;
  
 public slots:

  void updateInfo();
};

class MartaWidget : public QWidget
{
  Q_OBJECT
 public:

  explicit MartaWidget(MartaModel* model,
		       QWidget *parent = 0);

 protected:

  const static int LCD_SIZE = 8;

  MartaModel* model_;

  QCheckBox* martaCheckBox_;

  MartaSVGWidget* svgWidget_;
  
  QWidget* operationPanel_;

  QDoubleSpinBox* temperatureSetPointSpinner_;
  QDoubleSpinBox* speedSetPointSpinner_;
  QDoubleSpinBox* flowSetPointSpinner_;

  QCheckBox* chillerCheckBox_;
  QCheckBox* co2CheckBox_;
  QRadioButton* fixedSpeedButton_;
  QRadioButton* fixedFlowButton_;

  MartaAlarmDialog* alarmDialog_;
  
 public slots:

  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();

 protected slots:

  void showAlarmDialog();
};

#endif // MARTAWIDGET_H
