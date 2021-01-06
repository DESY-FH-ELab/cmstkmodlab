/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMODISPLAY2VALUEAXIS_H
#define THERMODISPLAY2VALUEAXIS_H

#include <QObject>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QDateTimeEdit>
#include <QButtonGroup>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class ThermoDisplay2ValueAxis : public QValueAxis
{
  Q_OBJECT
public:

	enum AxisMode {
		AxisModeUser = 0,
		AxisModeFull
	};

  explicit ThermoDisplay2ValueAxis(int precision = 2);

  void refresh(QList<QAbstractSeries*> series);
  void configure();

signals:

  void axisModeChanged();

protected:

  int precision_;
  AxisMode axisMode_;
  qreal min_, userMin_;
  qreal max_, userMax_;
};

class ThermoDisplay2ValueAxisDialog : public QDialog
{
  Q_OBJECT
 public:

  ThermoDisplay2ValueAxisDialog(int precision, QWidget* parent = 0);

  void setAxisMode(ThermoDisplay2ValueAxis::AxisMode axisMode);
  void setMinMaxRange(const qreal min, const qreal max);
  void setUserRange(const qreal min, const qreal max);

  ThermoDisplay2ValueAxis::AxisMode getAxisMode() const { return axisMode_; }
  void getUserRange(qreal& min, qreal& max);

 protected:

  QButtonGroup *buttonGroup_;
  QRadioButton *modeUser_;
  QRadioButton *modeFull_;

  int precision_;
  qreal absoluteMin_;
  QDoubleSpinBox *userMin_;
  qreal absoluteMax_;
  QDoubleSpinBox *userMax_;

  ThermoDisplay2ValueAxis::AxisMode axisMode_;

 public slots:

  void userMinChanged(double);
  void userMaxChanged(double);
  void buttonClicked(QAbstractButton *);
};

#endif // THERMODISPLAY2VALUEAXIS_H
