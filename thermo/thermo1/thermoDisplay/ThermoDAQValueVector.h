/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMODAQVALUEVECTOR_H
#define THERMODAQVALUEVECTOR_H

#include <QVector>
#include <qwt_date.h>

template <typename T> class ThermoDAQValueVector : public QVector<double>
{
public:
    ThermoDAQValueVector() {

    }

    void clearData() {
        QVector<double>::clear();
        values_.clear();
    }

    bool push(const QDateTime& time, T value) {
        bool ret = QVector<double>::size()>0 && lastValue()==value;
        ret = true;
        QVector<double>::append(QwtDate::toDouble(time));
        values_.append(value);
        return ret;
    }

    bool pushIfChanged(const QDateTime& time, T value) {
        if (QVector<double>::size()>0 && lastValue()==value) return false;
        QVector<double>::append(QwtDate::toDouble(time));
        values_.append(value);
        return true;
    }

    QDateTime lastTime() { return QwtDate::toDateTime(QVector<double>::last()); }
    const QDateTime& lastTime() const { return QwtDate::toDateTime(QVector<double>::last()); }
    const T& valueAt(int idx) const { return values_.at(idx); }
    const T& lastValue() const { return values_.last(); }

    const double* constTimes() const { return QVector<double>::constData(); }
    const T* constValues() const { return values_.constData(); }

protected:
    QVector<T> values_;
};

#endif // THERMODAQVALUEVECTOR_H
