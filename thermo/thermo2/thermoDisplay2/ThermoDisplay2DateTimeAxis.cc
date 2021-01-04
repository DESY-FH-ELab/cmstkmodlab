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

#include <iostream>
#include <algorithm>

#include <QBoxLayout>

#include <nqlogger.h>

#include "ThermoDisplay2LineSeries.h"
#include "ThermoDisplay2DateTimeAxis.h"

ThermoDisplay2DateTimeAxis::ThermoDisplay2DateTimeAxis()
 : QDateTimeAxis(),
   axisMode_(AxisModeFull)
{

}

void ThermoDisplay2DateTimeAxis::refresh(QList<QAbstractSeries*> series)
{
  QDateTime dtMin, dtMax;
  int temp;

  if (axisMode_!=AxisModeUser) {

    qreal minX = std::numeric_limits<qreal>::max();
    qreal maxX = -std::numeric_limits<qreal>::max();

    bool hasValues = false;
    for (QList<QAbstractSeries*>::Iterator it = series.begin();
        it!=series.end();
        ++it) {
      ThermoDisplay2LineSeries* s = dynamic_cast<ThermoDisplay2LineSeries*>(*it);
      if (s && s->isInitialized()) {
        minX = std::min(minX, s->minX());
        maxX = std::max(maxX, s->maxX());
        hasValues = true;
      }
    }
    if (!hasValues) return;

    min_ = QDateTime::fromMSecsSinceEpoch(minX);
    max_ = QDateTime::fromMSecsSinceEpoch(maxX);

    dtMin = min_;
    temp = dtMin.time().msec();
    dtMin = dtMin.addMSecs(-temp);
    temp = dtMin.time().second();
    dtMin = dtMin.addSecs(-temp);

    dtMax = max_;
    temp = dtMin.time().msec();
    dtMax = dtMax.addMSecs(-temp);
    temp = dtMin.time().second();
    dtMax = dtMax.addSecs(60-temp);

    QDateTime dtTemp = dtMax;
    if (axisMode_==AxisMode1Hour) {
      dtTemp = dtTemp.addSecs(-1 * 60 * 60);
    } else if (axisMode_==AxisMode2Hour) {
      dtTemp = dtTemp.addSecs(-2 * 60 * 60);
    } else if (axisMode_==AxisMode6Hour) {
      dtTemp = dtTemp.addSecs(-6 * 60 * 60);
    } else if (axisMode_==AxisMode12Hour) {
      dtTemp = dtTemp.addSecs(-12 * 60 * 60);
    } else if (axisMode_==AxisMode24Hour) {
      dtTemp = dtTemp.addSecs(-24 * 60 * 60);
    }

    if (axisMode_!=AxisModeFull) {
      if (dtMin<dtTemp) dtMin = dtTemp;
    }
  } else {
    dtMin = userMin_;
    dtMax = userMax_;
  }

  qint64 deltaX = dtMin.secsTo(dtMax) / 60;
  deltaX = 0.1*deltaX + 1;

  temp = dtMax.time().second();
  dtMax = dtMax.addSecs(-temp+deltaX*60);

  setRange(dtMin, dtMax);
}

void ThermoDisplay2DateTimeAxis::configure()
{
	ThermoDisplay2DateTimeAxisDialog dialog;
	dialog.exec();
}

ThermoDisplay2DateTimeAxisDialog::ThermoDisplay2DateTimeAxisDialog(QWidget* parent)
 : QDialog(parent)
{
  setMinimumWidth(600);
  setMinimumHeight(400);

  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);
}
