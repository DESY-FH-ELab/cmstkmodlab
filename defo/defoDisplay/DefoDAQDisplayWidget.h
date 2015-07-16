#ifndef DEFODAQDISPLAYWIDGET_H
#define DEFODAQDISPLAYWIDGET_H

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

#include "DefoDAQValueVector.h"

class LegendListView;
class QStandardItem;
class QModelIndex;
class QwtPlotItem;

class DefoDAQInternalLegend : public QwtPlotLegendItem
{
public:
    DefoDAQInternalLegend() {
        setRenderHint(QwtPlotItem::RenderAntialiased);
        QColor color(Qt::white);
        setTextPen(color);
        setBorderPen(color);
        QColor c(Qt::lightGray);
        c.setAlpha(200);
        setBackgroundBrush(c);
    }
};

class DefoDAQLegend : public QwtAbstractLegend
{
    Q_OBJECT

public:
    explicit DefoDAQLegend(QWidget *parent = NULL );
    virtual ~DefoDAQLegend();

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

class DefoDAQDateScaleDraw : public QwtDateScaleDraw
{
public:
    DefoDAQDateScaleDraw(Qt::TimeSpec timeSpec);
};

class DefoDAQTemperaturePicker : public QwtPlotPicker
{
public:
    DefoDAQTemperaturePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
    Qt::TimeSpec timeSpec_;
};

class DefoDAQDisplayWidget : public QwtPlot
{
  Q_OBJECT
public:
  explicit DefoDAQDisplayWidget(QWidget *parent = 0);

    void exportPlot();

protected slots:
  void showItem(QwtPlotItem* item, bool on);

protected:
  QwtPlotZoomer* zoomer_;
  DefoDAQInternalLegend* internalLegend_;
};

class DefoDAQTemperatureDisplayWidget : public DefoDAQDisplayWidget
{
  Q_OBJECT
public:
  explicit DefoDAQTemperatureDisplayWidget(QWidget *parent = 0);
};

class DefoDAQDisplayPlotItem : public QwtPlotCurve
{
public:
    DefoDAQDisplayPlotItem(const QwtText &title, const DefoDAQValueVector<double>* dataSource) :
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
    const DefoDAQValueVector<double>* dataSource() const { return dataSource_; }
    void attachToPlot(DefoDAQDisplayWidget *plot) {
        QwtPlotCurve::attach(plot);
    }
    void detachFromPlot(DefoDAQDisplayWidget *plot) {
        QwtPlotCurve::detach();
    }

protected:
    const DefoDAQValueVector<double>* dataSource_;
    bool enabled_;
};

#endif // DEFODAQDISPLAYWIDGET_H
