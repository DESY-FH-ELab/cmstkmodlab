/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMODAQDISPLAYWIDGET_H
#define THERMODAQDISPLAYWIDGET_H

#include <iostream>
#include <vector>

#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_date_scale_draw.h>
#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>
#include <qwt_plot_legenditem.h>
#include <qwt_abstract_legend.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_draw.h>

#include "ThermoDAQValueVector.h"

class LegendListView;
class QStandardItem;
class QModelIndex;
class QwtPlotItem;

class ThermoDAQPressureScaleDraw : public QwtScaleDraw
{
public:
    ThermoDAQPressureScaleDraw();
    virtual ~ThermoDAQPressureScaleDraw();
    virtual QwtText label (double) const;
};

class ThermoDAQPressureScaleEngine : public QwtLogScaleEngine
{
public:
    ThermoDAQPressureScaleEngine(uint base = 10);
    virtual ~ThermoDAQPressureScaleEngine();
    virtual void autoScale(int maxSteps,
                           double &x1, double &x2,
                           double &stepSize) const;
};

class ThermoDAQInternalLegend : public QwtPlotLegendItem
{
public:
    ThermoDAQInternalLegend() {
        setRenderHint(QwtPlotItem::RenderAntialiased);
        QColor color(Qt::white);
        setTextPen(color);
        setBorderPen(color);
        QColor c(Qt::lightGray);
        c.setAlpha(200);
        setBackgroundBrush(c);
    }
};

class ThermoDAQLegend : public QwtAbstractLegend
{
    Q_OBJECT

public:
    explicit ThermoDAQLegend(QWidget *parent = NULL );
    virtual ~ThermoDAQLegend();

    virtual void renderLegend(QPainter *painter,
                              const QRectF &, bool fillBackground ) const;
    virtual bool isEmpty() const;
    virtual int scrollExtent( Qt::Orientation ) const;

signals:
    void checked(QwtPlotItem *plotItem, bool on, int index);

public slots:
    virtual void updateLegend(const QVariant &,
                              const QList<QwtLegendData> & );

private slots:
    void handleClick( const QModelIndex & );

private:
    void updateItem( QStandardItem *, const QwtLegendData & );

    LegendListView *d_listView;
};

class ThermoDAQDateScaleDraw : public QwtDateScaleDraw
{
public:
    ThermoDAQDateScaleDraw(Qt::TimeSpec timeSpec);
};

class ThermoDAQTemperaturePicker : public QwtPlotPicker
{
public:
    ThermoDAQTemperaturePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
    Qt::TimeSpec timeSpec_;
};

class ThermoDAQPressurePicker : public QwtPlotPicker
{
public:
    ThermoDAQPressurePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
    Qt::TimeSpec timeSpec_;
};

class ThermoDAQOverPressurePicker : public QwtPlotPicker
{
public:
    ThermoDAQOverPressurePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
    Qt::TimeSpec timeSpec_;
};

class ThermoDAQDisplayWidget : public QwtPlot
{
  Q_OBJECT
public:
  explicit ThermoDAQDisplayWidget(QWidget *parent = 0);

    void exportPlot();

protected slots:
  void showItem(QwtPlotItem* item, bool on);

protected:
  QwtPlotZoomer* zoomer_;
  ThermoDAQInternalLegend* internalLegend_;
};

class ThermoDAQTemperatureDisplayWidget : public ThermoDAQDisplayWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQTemperatureDisplayWidget(QWidget *parent = 0);
};

class ThermoDAQPressureDisplayWidget : public ThermoDAQDisplayWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQPressureDisplayWidget(QWidget *parent = 0);
};

class ThermoDAQMicroPressureDisplayWidget : public ThermoDAQDisplayWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQMicroPressureDisplayWidget(QWidget *parent = 0);
};

class ThermoDAQDisplayPlotItem : public QwtPlotCurve
{
public:
    ThermoDAQDisplayPlotItem(const QwtText &title, const ThermoDAQValueVector<double>* dataSource) :
        QwtPlotCurve(title),
        dataSource_(dataSource),
        enabled_(true)
    {
        setVisible(true);
    }
    void setEnabled(bool on) { enabled_ = on; }
    bool isEnabled() const { return enabled_; }
    void refresh() {
        QwtPlotCurve::setRawSamples(dataSource_->constTimes(), dataSource_->constValues(),
                                    dataSource_->size());
    }
    const ThermoDAQValueVector<double>* dataSource() const { return dataSource_; }
    void attachToPlot(ThermoDAQDisplayWidget *plot) {
        QwtPlotCurve::attach(plot);
    }
    void detachFromPlot(ThermoDAQDisplayWidget *plot) {
        QwtPlotCurve::detach();
    }

protected:
    const ThermoDAQValueVector<double>* dataSource_;
    bool enabled_;
};

#endif // THERMODAQDISPLAYWIDGET_H
