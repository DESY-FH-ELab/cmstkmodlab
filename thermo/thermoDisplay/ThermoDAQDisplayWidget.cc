#include <iostream>
#include <numeric>
#include <limits>

#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>

#include <qwt_date_scale_draw.h>
#include <qwt_date_scale_engine.h>
#include <qwt_scale_engine.h>
#include <qwt_legend.h>
#include <qwt_date.h>

#include "ThermoDAQDisplayWidget.h"

ThermoDAQDateScaleDraw::ThermoDAQDateScaleDraw(Qt::TimeSpec timeSpec) :
    QwtDateScaleDraw(timeSpec)
{
    setDateFormat(QwtDate::Second, "hh:mm:ss");
    setDateFormat(QwtDate::Minute, "hh:mm:ss");
    setDateFormat(QwtDate::Hour, "hh:mm:ss");
    setDateFormat(QwtDate::Day, "hh:mm:ss\ndd/MM/yyyy");
    setDateFormat(QwtDate::Week, "hh:mm:ss\ndd/MM/yyyy");
    setDateFormat(QwtDate::Month, "hh:mm:ss\ndd/MM/yyyy");
}

ThermoDAQTemperaturePicker::ThermoDAQTemperaturePicker(QWidget *parent) :
    QwtPlotPicker(parent)
{
    setRubberBandPen(QColor(Qt::darkGreen));
    setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText ThermoDAQTemperaturePicker::trackerTextF(const QPointF &pos) const
{
    const QDateTime dt = QwtDate::toDateTime(pos.x());

    QString s;
    s += dt.toString(" hh:mm:ss ");
    s += QString::number(pos.y(), 'f', 2);
    s += QString::fromUtf8(" °C ");

    QwtText text(s);
    text.setColor(Qt::white);

    QColor c = rubberBandPen().color();
    text.setBorderPen(QPen(c));
    text.setBorderRadius(0);
    c.setAlpha(200);
    text.setBackgroundBrush(c);

    return text;
}

ThermoDAQPressurePicker::ThermoDAQPressurePicker(QWidget *parent) :
    QwtPlotPicker(parent)
{
    setRubberBandPen(QColor(Qt::darkGreen));
    setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText ThermoDAQPressurePicker::trackerTextF(const QPointF &pos) const
{
    const QDateTime dt = QwtDate::toDateTime(pos.x());

    QString s;
    s += dt.toString(" hh:mm:ss ");
    if (pos.y()<1) {
        s += QString::number(pos.y(), 'e', 2);
    } else {
        s += QString::number((int)pos.y());
    }
    s += " mbar ";

    QwtText text(s);
    text.setColor(Qt::white);

    QColor c = rubberBandPen().color();
    text.setBorderPen(QPen(c));
    text.setBorderRadius(0);
    c.setAlpha(200);
    text.setBackgroundBrush(c);

    return text;
}

ThermoDAQTemperatureZoomer::ThermoDAQTemperatureZoomer(QWidget *parent) :
    QwtPlotZoomer(parent)
{
    setRubberBandPen(QColor(Qt::darkGreen));
    setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText ThermoDAQTemperatureZoomer::trackerTextF(const QPointF &pos) const
{
    const QDateTime dt = QwtDate::toDateTime(pos.x());

    QString s;
    s += dt.toString(" hh:mm:ss ");
    s += QString::number(pos.y(), 'f', 2);
    s += QString::fromUtf8(" °C ");

    QwtText text(s);
    text.setColor(Qt::white);

    QColor c = rubberBandPen().color();
    text.setBorderPen(QPen(c));
    text.setBorderRadius(0);
    c.setAlpha(200);
    text.setBackgroundBrush(c);

    return text;
}

ThermoDAQPressureZoomer::ThermoDAQPressureZoomer(QWidget *parent) :
    QwtPlotZoomer(parent)
{
    setRubberBandPen(QColor(Qt::darkGreen));
    setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText ThermoDAQPressureZoomer::trackerTextF(const QPointF &pos) const
{
    const QDateTime dt = QwtDate::toDateTime(pos.x());

    QString s;
    s += dt.toString(" hh:mm:ss ");
    if (pos.y()<1) {
        s += QString::number(pos.y(), 'e', 2);
    } else {
        s += QString::number((int)pos.y());
    }
    s += " mbar ";

    QwtText text(s);
    text.setColor(Qt::white);

    QColor c = rubberBandPen().color();
    text.setBorderPen(QPen(c));
    text.setBorderRadius(0);
    c.setAlpha(200);
    text.setBackgroundBrush(c);

    return text;
}

ThermoDAQDisplayWidget::ThermoDAQDisplayWidget(QWidget *parent) :
    QwtPlot(parent),
    zoomer_(0)
{
    setCanvasBackground(QBrush(QColor(Qt::black)));
}

void ThermoDAQDisplayWidget::updateZoomBase()
{
    static const double limitMax = std::numeric_limits<double>::max();
    static const double limitMin = std::numeric_limits<double>::min();

//    std::cout << zoomer_->zoomBase().left() << std::endl;
//    std::cout << zoomer_->zoomBase().right() << std::endl;
//    std::cout << zoomer_->zoomBase().bottom() << std::endl;
//    std::cout << zoomer_->zoomBase().top() << std::endl;

    double minX = limitMax;
    double maxX = 0;
    double minY = limitMax;
    double maxY = limitMin;

    for (QVector<QwtPlotCurve*>::const_iterator it = curves_.begin();
         it!=curves_.end();
         ++it) {

        QRectF dr = (*it)->dataRect();
        minX = std::min(minX, dr.left());
        maxX = std::max(maxX, dr.right());
        minY = std::min(minY, dr.bottom());
        maxY = std::max(maxY, dr.top());
    }

    QRectF rect;
    rect.setLeft(minX);
    rect.setRight(maxX);
    rect.setBottom(minY);
    rect.setTop(maxY);

    zoomer_->setZoomBase(rect);
}

void ThermoDAQDisplayWidget::showItem(const QVariant & item, bool on)
{
    QwtPlotItem* plotItem = qvariant_cast<QwtPlotItem*>(item);
    plotItem->setVisible(on);
    replot();
}

void ThermoDAQDisplayWidget::removeCurve(QwtPlotCurve* curve)
{
    int index = curves_.indexOf(curve);
    curves_.remove(index);
}

ThermoDAQTemperatureDisplayWidget::ThermoDAQTemperatureDisplayWidget(QWidget *parent) :
    ThermoDAQDisplayWidget(parent)
{
    ThermoDAQDateScaleDraw* scaleDraw = new ThermoDAQDateScaleDraw(Qt::LocalTime);
    QwtDateScaleEngine* scaleEngine = new QwtDateScaleEngine(Qt::LocalTime);

    this->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
    this->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
    this->setAxisLabelRotation(QwtPlot::xBottom, -45.0);
    this->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    QwtLinearScaleEngine * yscaleEngine = new QwtLinearScaleEngine();
    yscaleEngine->setMargins(1.5, 1.5);
    this->setAxisScaleEngine(QwtPlot::yLeft, yscaleEngine);
    this->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("T [°C]"));

    ThermoDAQTemperaturePicker * picker = new ThermoDAQTemperaturePicker(this->canvas());
//    zoomer_ = new ThermoDAQTemperatureZoomer(this->canvas());
//    zoomer_->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
//    zoomer_->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
//    zoomer_->setKeyPattern(QwtEventPattern::KeyHome, Qt::Key_Home);
//    zoomer_->setMaxStackDepth(0);

    QwtLegend *legend = new QwtLegend(this);
    legend->setDefaultItemMode(QwtLegendData::Checkable);
    this->insertLegend(legend, QwtPlot::RightLegend);

    connect(legend, SIGNAL(checked(QVariant,bool,int)),
            this, SLOT(showItem(QVariant,bool)));
}

void ThermoDAQTemperatureDisplayWidget::updateZoomBase()
{
    static const double limitMax = std::numeric_limits<double>::max();
    static const double limitMin = std::numeric_limits<double>::min();

//    std::cout << zoomer_->zoomBase().left() << std::endl;
//    std::cout << zoomer_->zoomBase().right() << std::endl;
//    std::cout << zoomer_->zoomBase().bottom() << std::endl;
//    std::cout << zoomer_->zoomBase().top() << std::endl;

    double minX = limitMax;
    double maxX = 0;
    double minY = limitMax;
    double maxY = limitMin;

    for (QVector<QwtPlotCurve*>::const_iterator it = curves_.begin();
         it!=curves_.end();
         ++it) {

        QRectF dr = (*it)->dataRect();
        minX = std::min(minX, dr.left());
        maxX = std::max(maxX, dr.right());
        minY = std::min(minY, dr.bottom());
        maxY = std::max(maxY, dr.top());
    }

    QRectF rect;
    rect.setLeft(minX);
    rect.setRight(maxX);
    rect.setBottom(minY-2);
    rect.setTop(maxY+2);

    zoomer_->setZoomBase(rect);
}

ThermoDAQPressureDisplayWidget::ThermoDAQPressureDisplayWidget(QWidget *parent) :
    ThermoDAQDisplayWidget(parent)
{
    ThermoDAQDateScaleDraw* scaleDraw = new ThermoDAQDateScaleDraw(Qt::LocalTime);
    QwtDateScaleEngine* scaleEngine = new QwtDateScaleEngine(Qt::LocalTime);

    this->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
    this->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
    this->setAxisLabelRotation(QwtPlot::xBottom, -45.0);
    this->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    QwtLogScaleEngine * yscaleEngine = new QwtLogScaleEngine();
    //yscaleEngine->setMargins(1, 0);
    this->setAxisScaleEngine(QwtPlot::yLeft, yscaleEngine);
    this->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("p [mbar]"));

    ThermoDAQPressurePicker * picker = new ThermoDAQPressurePicker(this->canvas());
//    zoomer_ = new ThermoDAQPressureZoomer(this->canvas());
//    zoomer_->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
//    zoomer_->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
//    zoomer_->setKeyPattern(QwtEventPattern::KeyHome, Qt::Key_Home);

    QwtLegend *legend = new QwtLegend(this);
    legend->setDefaultItemMode(QwtLegendData::Checkable);
    this->insertLegend(legend, QwtPlot::RightLegend);

    connect(legend, SIGNAL(checked(QVariant,bool,int)),
            this, SLOT(showItem(QVariant,bool)));
}
