#ifndef THERMODISPLAYMAINWINDOW_H
#define THERMODISPLAYMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>

#include <qwt_date.h>

#include "ThermoDAQClient.h"
#include "ThermoDAQNetworkReader.h"

template <typename value_type> class ThermoDAQValueVector : public QVector<double>
{
public:
    ThermoDAQValueVector() {

    }

    void clear() {
        QVector<double>::clear();
        values_.clear();
    }

    bool push(const QDateTime& time, value_type value) {
        bool ret = QVector<double>::size()>0 && lastValue()==value;
        QVector<double>::append(QwtDate::toDouble(time));
        values_.append(value);
        return ret;
    }

    bool pushIfChanged(const QDateTime& time, value_type value) {
        if (QVector<double>::size()>0 && lastValue()==value) return false;
        QVector<double>::append(QwtDate::toDouble(time));
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

class ThermoDisplayMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit ThermoDisplayMainWindow(QWidget *parent = 0);

public slots:

    void requestData();
    void updateInfo();

protected:

    QTimer* timer_;

    ThermoDAQClient* client_;
    ThermoDAQNetworkReader* reader_;

    ThermoDAQValueVector<float> bath_;
};

#endif // THERMODISPLAYMAINWINDOW_H
