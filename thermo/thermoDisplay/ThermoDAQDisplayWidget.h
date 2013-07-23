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

#include "ThermoDAQValueVector.h"

class LegendListView;
class QStandardItem;
class QModelIndex;
class QwtPlotItem;

class ThermoDAQInternalLegend : public QwtPlotLegendItem
{
public:
    ThermoDAQInternalLegend() {
        setRenderHint( QwtPlotItem::RenderAntialiased );
        QColor color( Qt::white );
        setTextPen( color );
        setBorderPen( color );
        QColor c( Qt::gray );
        c.setAlpha( 200 );
        setBackgroundBrush( c );
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
