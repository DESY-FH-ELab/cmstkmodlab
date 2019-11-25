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

#ifndef THERMODAQ2DISPLAYWIDGET_H
#define THERMODAQ2DISPLAYWIDGET_H

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

#include "ThermoDAQ2ValueVector.h"

class LegendListView;
class QStandardItem;
class QModelIndex;
class QwtPlotItem;

class ThermoDAQ2PressureScaleDraw : public QwtScaleDraw
{
public:
  ThermoDAQ2PressureScaleDraw();
  virtual ~ThermoDAQ2PressureScaleDraw();
  virtual QwtText label (double) const;
};

class ThermoDAQ2PressureScaleEngine : public QwtLogScaleEngine
{
public:
  ThermoDAQ2PressureScaleEngine(uint base = 10);
  virtual ~ThermoDAQ2PressureScaleEngine();
  virtual void autoScale(int maxSteps,
                         double &x1, double &x2,
                         double &stepSize) const;
};

class ThermoDAQ2InternalLegend : public QwtPlotLegendItem
{
public:
  ThermoDAQ2InternalLegend() {
    setRenderHint(QwtPlotItem::RenderAntialiased);
    QColor color(Qt::white);
    setTextPen(color);
    setBorderPen(color);
    QColor c(Qt::lightGray);
    c.setAlpha(200);
    setBackgroundBrush(c);
  }
};

class ThermoDAQ2Legend : public QwtAbstractLegend
{
  Q_OBJECT

public:
  explicit ThermoDAQ2Legend(QWidget *parent = NULL );
  virtual ~ThermoDAQ2Legend();

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

class ThermoDAQ2DateScaleDraw : public QwtDateScaleDraw
{
public:
  ThermoDAQ2DateScaleDraw(Qt::TimeSpec timeSpec);
};

class ThermoDAQ2VoltagePicker : public QwtPlotPicker
{
public:
  ThermoDAQ2VoltagePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
  virtual QwtText trackerTextF(const QPointF &) const;
  Qt::TimeSpec timeSpec_;
};

class ThermoDAQ2CurrentPicker : public QwtPlotPicker
{
public:
  ThermoDAQ2CurrentPicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
  virtual QwtText trackerTextF(const QPointF &) const;
  Qt::TimeSpec timeSpec_;
};

class ThermoDAQ2TemperaturePicker : public QwtPlotPicker
{
public:
  ThermoDAQ2TemperaturePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
  virtual QwtText trackerTextF(const QPointF &) const;
  Qt::TimeSpec timeSpec_;
};

class ThermoDAQ2PressurePicker : public QwtPlotPicker
{
public:
  ThermoDAQ2PressurePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
  virtual QwtText trackerTextF(const QPointF &) const;
  Qt::TimeSpec timeSpec_;
};

class ThermoDAQ2OverPressurePicker : public QwtPlotPicker
{
public:
    ThermoDAQ2OverPressurePicker(QWidget *parent, Qt::TimeSpec spec = Qt::LocalTime);
protected:
    virtual QwtText trackerTextF(const QPointF &) const;
    Qt::TimeSpec timeSpec_;
};

class ThermoDAQ2DisplayWidget : public QwtPlot
{
  Q_OBJECT
public:
  explicit ThermoDAQ2DisplayWidget(QWidget *parent = 0);

  void exportPlot();

protected slots:
  void showItem(QwtPlotItem* item, bool on);

protected:
  QwtPlotZoomer* zoomer_;
  ThermoDAQ2InternalLegend* internalLegend_;
};

class ThermoDAQ2VoltageDisplayWidget : public ThermoDAQ2DisplayWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQ2VoltageDisplayWidget(QWidget *parent = 0);
};

class ThermoDAQ2CurrentDisplayWidget : public ThermoDAQ2DisplayWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQ2CurrentDisplayWidget(QWidget *parent = 0);
};

class ThermoDAQ2TemperatureDisplayWidget : public ThermoDAQ2DisplayWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQ2TemperatureDisplayWidget(QWidget *parent = 0);
};

class ThermoDAQ2PressureDisplayWidget : public ThermoDAQ2DisplayWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQ2PressureDisplayWidget(QWidget *parent = 0);
};

class ThermoDAQ2DisplayPlotItem : public QwtPlotCurve
{
public:
  ThermoDAQ2DisplayPlotItem(const QwtText &title, const ThermoDAQ2ValueVector<double>* dataSource)
    : QwtPlotCurve(title),
      dataSource_(dataSource),
      enabled_(true) {
    setVisible(true);
  }

  void setEnabled(bool on) { enabled_ = on; }
  bool isEnabled() const { return enabled_; }
  void refresh() {
    QwtPlotCurve::setRawSamples(dataSource_->constTimes(), dataSource_->constValues(),
                                dataSource_->size());
  }
  const ThermoDAQ2ValueVector<double>* dataSource() const { return dataSource_; }
  void attachToPlot(ThermoDAQ2DisplayWidget *plot) {
    QwtPlotCurve::attach(plot);
  }
  void detachFromPlot(ThermoDAQ2DisplayWidget *plot) {
    QwtPlotCurve::detach();
  }

protected:

  const ThermoDAQ2ValueVector<double>* dataSource_;
  bool enabled_;
};

#endif // THERMODAQ2DISPLAYWIDGET_H
