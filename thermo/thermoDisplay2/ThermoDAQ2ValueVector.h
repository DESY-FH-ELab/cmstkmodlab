/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMODAQ2VALUEVECTOR_H
#define THERMODAQ2VALUEVECTOR_H

#include <QVector>
#include <qwt_date.h>

template <typename value_type> class ThermoDAQ2ValueVector : public QVector<double>
{
public:
  ThermoDAQ2ValueVector() {

  }

  void clear() {
    QVector<double>::clear();
    values_.clear();
  }

  bool push(const QDateTime& time, value_type value) {
    bool ret = QVector<double>::size()>0 && lastValue()==value;
    QVector<double>::append(time.toMSecsSinceEpoch());
    values_.append(value);
    return ret;
  }

  bool pushIfChanged(const QDateTime& time, value_type value) {
    if (QVector<double>::size()>0 && lastValue()==value) return false;
    QVector<double>::append(time.toMSecsSinceEpoch());
    values_.append(value);
    return true;
  }

  double& lastTime() { return QVector<double>::last(); }
  const double& lastTime() const { return QVector<double>::last(); }
  value_type& lastValue() { return values_.last(); }

  const double* constTimes() const { return QVector<double>::constData(); }
  const value_type* constValues() const { return values_.constData(); }

protected:
  QVector<value_type> values_;
};

#endif // THERMODAQ2VALUEVECTOR_H
