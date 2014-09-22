#ifndef MICRODAQMODEL_H
#define MICRODAQMODEL_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include <qwt_date.h>

#include "IotaModel.h"
#include "ArduinoPresModel.h"

template <typename value_type> class MicroDAQValueVector : public QVector<double>
{
public:
    MicroDAQValueVector() {

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

class MicroDAQModel : public QObject
{
    Q_OBJECT
public:
    explicit MicroDAQModel(IotaModel* iotaModel,
                           ArduinoPresModel* arduinoPresModel,
                           QObject *parent = 0);

    QDateTime& currentTime();

    void customDAQMessage(const QString & message);
    void createDAQStatusMessage(QString & buffer);

    void myMoveToThread(QThread *thread);

    bool daqState() const { return daqState_; }

public slots:

    void startMeasurement();
    void stopMeasurement();

protected slots:

    void iotaInfoChanged();
    void arduinoPresInfoChanged();

    void clearHistory();

protected:

    bool daqState_;

    IotaModel* iotaModel_;
    ArduinoPresModel* arduinoPresModel_;

    QMutex mutex_;

    QDateTime currentTime_;

    template <typename T> bool updateIfChanged(T &variable, T newValue) {
        if (variable==newValue) return false;
        variable = newValue;
        return true;
    }

    // IOTA DATA
    bool iotaPumpEnabled_;
    float iotaActPressure_;
    float iotaSetPressure_;
    float iotaActFlow_;
    float iotaSetFlow_;

    // ARDUINO PRES DATA
    float arduinoPressureA_;
    float arduinoPressureB_;

signals:

    void daqMessage(const QString & message);
    void daqStateChanged(bool running);
    void newDataAvailable();
};

#endif // MICRODAQMODEL_H
