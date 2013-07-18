#ifndef THERMODAQDISPLAYWIDGET_H
#define THERMODAQDISPLAYWIDGET_H

#include <iostream>

#include <QWidget>
#include <QVBoxLayout>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_date_scale_draw.h>

#include "ThermoDAQModel.h"

template <typename value_type> class ThermoDAQDisplayPlotItem : public QwtPlotCurve
{
public:
    ThermoDAQDisplayPlotItem(const QwtText &title, const ThermoDAQValueVector<value_type>* dataSource) :
        QwtPlotCurve(title),
        dataSource_(dataSource) { }
    void refresh();
    const ThermoDAQValueVector<value_type>* dataSource() const { return dataSource_; }
protected:
    const ThermoDAQValueVector<value_type>* dataSource_;
};

template <typename value_type>
inline void ThermoDAQDisplayPlotItem<value_type>::refresh() {
    if (dataSource_->size()==dataSize()) return;
    QwtPlotCurve::setRawSamples(dataSource_->constTimes(), dataSource_->constValues(),
                                dataSource_->size());
    std::cout << title().text().toStdString() << std::endl;
}

class ThermoDAQDisplayWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQDisplayWidget(ThermoDAQModel* DAQModel, QWidget *parent = 0);

protected:
  // Models
  ThermoDAQModel* daqModel_;

  // Widgets
  QwtPlot* temperaturePlot_;
  QwtPlot* pressurePlot_;

  QwtDateScaleDraw* temperatureDateScaleDraw_;
  QwtDateScaleDraw* pressureDateScaleDraw_;

  ThermoDAQDisplayPlotItem<double>* keithleyItem_[10];

  ThermoDAQDisplayPlotItem<double>* pfeifferItem1_;
  ThermoDAQDisplayPlotItem<double>* pfeifferItem2_;

protected slots:
  void refresh();
};

#endif // THERMODAQDISPLAYWIDGET_H
