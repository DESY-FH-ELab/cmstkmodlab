#ifndef THERMODAQDISPLAYWIDGET_H
#define THERMODAQDISPLAYWIDGET_H

#include <iostream>
#include <vector>

#include <QWidget>
#include <QVBoxLayout>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_date_scale_draw.h>
#include <qwt_plot_zoomer.h>

#include "ThermoDAQValueVector.h"

class ThermoDAQDateScaleDraw : public QwtDateScaleDraw
{
public:
    ThermoDAQDateScaleDraw(Qt::TimeSpec timeSpec);
};

class ThermoDAQTemperaturePicker : public QwtPlotPicker
{
public:
    ThermoDAQTemperaturePicker(QWidget *parent);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
};

class ThermoDAQPressurePicker : public QwtPlotPicker
{
public:
    ThermoDAQPressurePicker(QWidget *parent);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
};

class ThermoDAQTemperatureZoomer : public QwtPlotZoomer
{
public:
    ThermoDAQTemperatureZoomer(QWidget *parent);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
};

class ThermoDAQPressureZoomer : public QwtPlotZoomer
{
public:
    ThermoDAQPressureZoomer(QWidget *parent);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
};

class ThermoDAQDisplayWidget : public QwtPlot
{
  Q_OBJECT
public:
  explicit ThermoDAQDisplayWidget(QWidget *parent = 0);
  virtual void updateZoomBase();
  void addCurve(QwtPlotCurve* curve) { curves_.push_back(curve); }
  void removeCurve(QwtPlotCurve* curve);

protected slots:
  void showItem(const QVariant & item, bool on);

protected:
  QwtPlotZoomer* zoomer_;
  QVector<QwtPlotCurve*> curves_;
};

class ThermoDAQTemperatureDisplayWidget : public ThermoDAQDisplayWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQTemperatureDisplayWidget(QWidget *parent = 0);
    void updateZoomBase();
};

class ThermoDAQPressureDisplayWidget : public ThermoDAQDisplayWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQPressureDisplayWidget(QWidget *parent = 0);
};

template <typename T> class ThermoDAQDisplayPlotItem : public QwtPlotCurve
{
public:
    ThermoDAQDisplayPlotItem(const QwtText &title, const ThermoDAQValueVector<T>* dataSource) :
        QwtPlotCurve(title),
        dataSource_(dataSource),
        isAttached_(false)
    {
        setVisible(false);
    }
    bool isAttached() const { return isAttached_; }
    void refresh() {
        QwtPlotCurve::setRawSamples(dataSource_->constTimes(), dataSource_->constValues(),
                                    dataSource_->size());
    }
    const ThermoDAQValueVector<T>* dataSource() const { return dataSource_; }
    void attachToPlot(ThermoDAQDisplayWidget *plot) {
        QwtPlotCurve::attach(plot);
        plot->addCurve(this);
        isAttached_ = true;
    }
    void detachFromPlot(ThermoDAQDisplayWidget *plot) {
        QwtPlotCurve::detach();
        plot->removeCurve(this);
        isAttached_ = false;
    }

protected:
    const ThermoDAQValueVector<T>* dataSource_;
    bool isAttached_;
};

#endif // THERMODAQDISPLAYWIDGET_H
