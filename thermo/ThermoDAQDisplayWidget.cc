#include <iostream>

#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>

#include <qwt_date_scale_draw.h>
#include <qwt_date_scale_engine.h>
#include <qwt_scale_engine.h>

#include "ThermoDAQDisplayWidget.h"

ThermoDAQDisplayWidget::ThermoDAQDisplayWidget(ThermoDAQModel* daqModel,
                                               QWidget *parent) :
    QWidget(parent),
    daqModel_(daqModel)
{
  // Layouts to put everything into place
  QVBoxLayout* layout = new QVBoxLayout();
  setLayout(layout);
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  temperatureDateScaleDraw_ = new QwtDateScaleDraw();
  temperatureDateScaleDraw_->setTimeSpec(Qt::LocalTime);

  temperaturePlot_ = new QwtPlot(QwtText("Temperatures"), this);
  temperaturePlot_->setAxisTitle(QwtPlot::xBottom, QwtText("Time"));
  temperaturePlot_->setAxisScaleDraw( QwtPlot::xBottom, temperatureDateScaleDraw_);
  temperaturePlot_->setAxisScaleEngine(QwtPlot::xBottom, new QwtDateScaleEngine());
  temperaturePlot_->setAxisTitle(QwtPlot::yLeft, QwtText("Temperature [C]"));
  temperaturePlot_->setMaximumHeight(300);
  layout->addWidget(temperaturePlot_);

  pressureDateScaleDraw_ = new QwtDateScaleDraw();
  pressureDateScaleDraw_->setTimeSpec(Qt::LocalTime);

  QwtLogScaleEngine * logScale = new QwtLogScaleEngine();

  pressurePlot_ = new QwtPlot(QwtText("Pressures"), this);
  pressurePlot_->setAxisTitle(QwtPlot::xBottom, QwtText("Time"));
  pressurePlot_->setAxisScaleDraw( QwtPlot::xBottom, pressureDateScaleDraw_);
  pressurePlot_->setAxisScaleEngine(QwtPlot::xBottom, new QwtDateScaleEngine());
  pressurePlot_->setAxisTitle(QwtPlot::yLeft, QwtText("Pressure [mbar]"));
  pressurePlot_->setAxisScaleEngine(QwtPlot::yLeft, logScale);
  pressurePlot_->setMaximumHeight(300);
  layout->addWidget(pressurePlot_);

  updateGeometry();

  QString nameTemplate("sensor %1");
  for (int sensor=0;sensor<10;++sensor) {
      keithleyItem_[sensor] = new ThermoDAQDisplayPlotItem<double>(QwtText(nameTemplate.arg(sensor)),
                                                                   daqModel_->getKeithleyTemperature(sensor));
  }

  pfeifferItem1_ = new ThermoDAQDisplayPlotItem<double>(QwtText("p1"),
                                                        daqModel_->getPressure1());
  pfeifferItem2_ = new ThermoDAQDisplayPlotItem<double>(QwtText("p2"),
                                                        daqModel_->getPressure2());

  connect(daqModel_, SIGNAL(newDataAvailable()),
          this, SLOT(refresh()));
}

void ThermoDAQDisplayWidget::refresh()
{
    std::cout << "ThermoDAQDisplayWidget::refresh()" << std::endl;

    for (int i=0;i<10;++i) {
        keithleyItem_[i]->refresh();
        if (keithleyItem_[i]->dataSize()>2) {
            keithleyItem_[i]->attach(temperaturePlot_);
        } else {
            keithleyItem_[i]->detach();
        }
    }

    pfeifferItem1_->refresh();
    if (pfeifferItem1_->dataSize()>2) {
        pfeifferItem1_->attach(pressurePlot_);
    } else {
        pfeifferItem1_->detach();
    }

    pfeifferItem2_->refresh();
    if (pfeifferItem2_->dataSize()>2) {
        pfeifferItem2_->attach(pressurePlot_);
    } else {
        pfeifferItem2_->detach();
    }

    temperaturePlot_->replot();
    pressurePlot_->replot();
}
