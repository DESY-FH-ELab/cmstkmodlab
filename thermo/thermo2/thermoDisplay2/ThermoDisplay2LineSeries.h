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

#ifndef THERMODISPLAY2LINESERIES_H
#define THERMODISPLAY2LINESERIES_H

#include <QObject>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class ThermoDisplay2LineSeries : public QLineSeries
{
  Q_OBJECT
public:

  explicit ThermoDisplay2LineSeries();

  bool isInitialized() { return initialized_; }
  void resetInitialized() { initialized_ = false; }

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool enabled);

  void append(qreal x, qreal y);

  qreal minX() { return minX_; }
  qreal maxX() { return maxX_; }
  qreal minY() { return minY_; }
  qreal maxY() { return maxY_; }

protected slots:

signals:

protected:

  bool initialized_;
  bool enabled_;
  qreal minX_, maxX_;
  qreal minY_, maxY_;
  qreal lastX_, lastY_;
};

#endif // THERMODISPLAY2LINESERIES_H
