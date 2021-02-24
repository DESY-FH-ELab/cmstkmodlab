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

#ifndef THERMODISPLAY2DATETIMEAXIS_H
#define THERMODISPLAY2DATETIMEAXIS_H

#include <QObject>
#include <QDialog>
#include <QDateTime>
#include <QRadioButton>
#include <QDateTimeEdit>
#include <QButtonGroup>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>

QT_CHARTS_USE_NAMESPACE

class ThermoDisplay2DateTimeAxis : public QDateTimeAxis
{
  Q_OBJECT
public:

  typedef enum axismode {
    AxisMode15Minutes = 0,
    AxisMode30Minutes,
    AxisMode1Hour,
    AxisMode2Hour,
    AxisMode6Hour,
    AxisMode12Hour,
    AxisMode24Hour,
    AxisModeUser,
    AxisModeFull
  } AxisMode;

  explicit ThermoDisplay2DateTimeAxis();

  void refresh(QList<QAbstractSeries*> series);
  void configure();

signals:

  void axisModeChanged();

protected:

  AxisMode axisMode_;
  QDateTime min_, userMin_;
  QDateTime max_, userMax_;
};

class ThermoDisplay2DateTimeAxisDialog : public QDialog
{
  Q_OBJECT
 public:

  ThermoDisplay2DateTimeAxisDialog(QWidget* parent = 0);

  void setAxisMode(ThermoDisplay2DateTimeAxis::AxisMode axisMode);
  void setMinMaxRange(const QDateTime& dtMin, const QDateTime& dtMax);
  void setUserRange(const QDateTime& dtMin, const QDateTime& dtMax);

  ThermoDisplay2DateTimeAxis::AxisMode getAxisMode() const { return axisMode_; }
  void getUserRange(QDateTime& dtMin, QDateTime& dtMax);

 protected:

  QButtonGroup *buttonGroup_;
  QRadioButton *mode15Minutes_;
  QRadioButton *mode30Minutes_;
  QRadioButton *mode1Hour_;
  QRadioButton *mode2Hour_;
  QRadioButton *mode6Hour_;
  QRadioButton *mode12Hour_;
  QRadioButton *mode24Hour_;
  QRadioButton *modeUser_;
  QRadioButton *modeFull_;

  QDateTimeEdit *userMin_;
  QDateTimeEdit *userMax_;

  ThermoDisplay2DateTimeAxis::AxisMode axisMode_;

 public slots:

  void buttonClicked(QAbstractButton *);
};

#endif // THERMODISPLAY2DATETIMEAXIS_H
