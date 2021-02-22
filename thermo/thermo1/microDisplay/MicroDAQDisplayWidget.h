#ifndef MICRODAQDISPLAYWIDGET_H
#define MICRODAQDISPLAYWIDGET_H

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

#include "MicroDAQValueVector.h"

class LegendListView;
class QStandardItem;
class QModelIndex;
class QwtPlotItem;

class MicroDAQPressureScaleDraw : public QwtScaleDraw
{
public:
    MicroDAQPressureScaleDraw();
    virtual ~MicroDAQPressureScaleDraw();
    virtual QwtText label (double) const;
};

class MicroDAQPressureScaleEngine : public QwtLogScaleEngine
{
public:
    MicroDAQPressureScaleEngine(uint base = 10);
    virtual ~MicroDAQPressureScaleEngine();
    virtual void autoScale(int maxSteps,
                           double &x1, double &x2,
                           double &stepSize) const;
};

class MicroDAQInternalLegend : public QwtPlotLegendItem
{
public:
    MicroDAQInternalLegend() {
        setRenderHint(QwtPlotItem::RenderAntialiased);
        QColor color(Qt::white);
        setTextPen(color);
        setBorderPen(color);
        QColor c(Qt::lightGray);
        c.setAlpha(200);
        setBackgroundBrush(c);
    }
};

class MicroDAQLegend : public QwtAbstractLegend
{
    Q_OBJECT

public:
    explicit MicroDAQLegend(QWidget *parent = NULL );
    virtual ~MicroDAQLegend();

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

class MicroDAQDateScaleDraw : public QwtDateScaleDraw
{
public:
    MicroDAQDateScaleDraw(Qt::TimeSpec timeSpec);
};

class MicroDAQTemperaturePicker : public QwtPlotPicker
{
public:
    MicroDAQTemperaturePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
    Qt::TimeSpec timeSpec_;
};

class MicroDAQPressurePicker : public QwtPlotPicker
{
public:
    MicroDAQPressurePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
    Qt::TimeSpec timeSpec_;
};

class MicroDAQOverPressurePicker : public QwtPlotPicker
{
public:
    MicroDAQOverPressurePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
    Qt::TimeSpec timeSpec_;
};

class MicroDAQDisplayWidget : public QwtPlot
{
  Q_OBJECT
public:
  explicit MicroDAQDisplayWidget(QWidget *parent = 0);

    void exportPlot();

protected slots:
  void showItem(QwtPlotItem* item, bool on);

protected:
  QwtPlotZoomer* zoomer_;
  MicroDAQInternalLegend* internalLegend_;
};

class MicroDAQTemperatureDisplayWidget : public MicroDAQDisplayWidget
{
  Q_OBJECT
public:
  explicit MicroDAQTemperatureDisplayWidget(QWidget *parent = 0);
};

class MicroDAQPressureDisplayWidget : public MicroDAQDisplayWidget
{
  Q_OBJECT
public:
  explicit MicroDAQPressureDisplayWidget(QWidget *parent = 0);
};

class MicroDAQMicroPressureDisplayWidget : public MicroDAQDisplayWidget
{
  Q_OBJECT
public:
  explicit MicroDAQMicroPressureDisplayWidget(QWidget *parent = 0);
};

class MicroDAQDisplayPlotItem : public QwtPlotCurve
{
public:
    MicroDAQDisplayPlotItem(const QwtText &title, const MicroDAQValueVector<double>* dataSource) :
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
    const MicroDAQValueVector<double>* dataSource() const { return dataSource_; }
    void attachToPlot(MicroDAQDisplayWidget *plot) {
        QwtPlotCurve::attach(plot);
    }
    void detachFromPlot(MicroDAQDisplayWidget *plot) {
        QwtPlotCurve::detach();
    }

protected:
    const MicroDAQValueVector<double>* dataSource_;
    bool enabled_;
};

#endif // MICRODAQDISPLAYWIDGET_H
