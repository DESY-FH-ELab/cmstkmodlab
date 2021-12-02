#ifndef MICRODAQVALUEVECTOR_H
#define MICRODAQVALUEVECTOR_H

#include <QVector>
#include <qwt_date.h>

template <typename T> class MicroDAQValueVector : public QVector<double>
{
public:
    MicroDAQValueVector() {

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

#endif // MICRODAQVALUEVECTOR_H
