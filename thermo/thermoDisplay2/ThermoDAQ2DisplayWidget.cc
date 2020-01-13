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

#include <iostream>
#include <numeric>
#include <limits>

#include <QGroupBox>
#include <QVBoxLayout>
#include <QVector>
#include <QListView>

#include <qwt_date_scale_draw.h>
#include <qwt_date_scale_engine.h>
#include <qwt_scale_engine.h>
#include <qwt_legend.h>
#include <qwt_date.h>
#include <qwt_painter.h>
#include <qwt_legend_label.h>
#include <qwt_plot_renderer.h>

#include <qwt_legend_data.h>
#include <qwt_text.h>
#include <qwt_plot_item.h>
#include <qtreeview.h>
#include <qlayout.h>
#include <qstyle.h>
#include <qstandarditemmodel.h>
#include <qitemdelegate.h>
#include <qpainter.h>

#include "ThermoDAQ2DisplayWidget.h"

ThermoDAQ2PressureScaleDraw::ThermoDAQ2PressureScaleDraw()
  : QwtScaleDraw()
{

}

ThermoDAQ2PressureScaleDraw::~ThermoDAQ2PressureScaleDraw()
{

}

QwtText ThermoDAQ2PressureScaleDraw::label(double value) const
{
  if ( qFuzzyCompare( value + 1.0, 1.0 ) )
    value = 0.0;

  if (value<1) {
    return QLocale().toString(value, 'e', 1);
  } else if (value<10) {
    return QLocale().toString(value, 'f', 1);
  } else {
    return QLocale().toString((int)value);
  }
}

ThermoDAQ2PressureScaleEngine::ThermoDAQ2PressureScaleEngine(uint base)
  : QwtLogScaleEngine(base)
{

}

ThermoDAQ2PressureScaleEngine::~ThermoDAQ2PressureScaleEngine()
{

}

void ThermoDAQ2PressureScaleEngine::autoScale(int maxNumSteps,
                                              double &x1, double &x2,
                                              double &stepSize) const
{
  QwtLogScaleEngine::autoScale(maxNumSteps, x1, x2, stepSize);
  if (x2>2000) x2 = 2000;
}

static void qwtRenderBackground(QPainter *painter,
                                const QRectF &rect,
                                const QWidget *widget )
{
  if (widget->testAttribute(Qt::WA_StyledBackground)) {
    QStyleOption opt;
    opt.initFrom( widget );
    opt.rect = rect.toAlignedRect();

    widget->style()->drawPrimitive(QStyle::PE_Widget,
                                   &opt, painter, widget);
  } else {
    const QBrush brush = widget->palette().brush(widget->backgroundRole());
    painter->fillRect( rect, brush );
  }
}

class LegendListView: public QListView
{
public:
  LegendListView(ThermoDAQ2Legend *);

  QList<QStandardItem *> itemList(const QwtPlotItem *);

  virtual QSize sizeHint() const;
  virtual QSize minimumSizeHint() const;
};

LegendListView::LegendListView(ThermoDAQ2Legend *legend)
  : QListView(legend)
{
  setFrameStyle(NoFrame);
  viewport()->setBackgroundRole(QPalette::Window);
  viewport()->setAutoFillBackground(false);

  QStandardItemModel *model = new QStandardItemModel();

  setModel(model);

  setItemDelegate(new QItemDelegate(this));
}

QList<QStandardItem *> LegendListView::itemList(const QwtPlotItem *plotItem)
{
  QList<QStandardItem*> itemList;

  QStandardItemModel *mdl = qobject_cast<QStandardItemModel*>(model());
  for (int row = 0;row<mdl->rowCount();++row) {
    QStandardItem *item = mdl->item(row);
    const QVariant key = item->data();
    if (key.canConvert<qlonglong>()) {
      const qlonglong ptr = key.value<qlonglong>();
      if (ptr==qlonglong(plotItem)) itemList += item;
    }
  }

  return itemList;
}

QSize LegendListView::minimumSizeHint() const
{
  return QSize(-1, -1);
}

QSize LegendListView::sizeHint() const
{
  QStyleOptionViewItem styleOption;
  styleOption.initFrom( this );

  const QAbstractItemDelegate *delegate = itemDelegate();

  const QStandardItemModel *mdl =
      qobject_cast<const QStandardItemModel *>( model() );

  int w = 0;
  int h = 0;

  for ( int row = 0; row < mdl->rowCount(); row++ )
  {
    const QStandardItem *rootItem = mdl->item( row );

    int wRow = 0;
    for ( int i = 0; i < rootItem->rowCount(); i++ )
    {
      const QSize hint = delegate->sizeHint( styleOption,
                                             rootItem->child( i )->index() );

      wRow = qMax( wRow, hint.width() );
      h += hint.height();
    }

    const QSize rootHint = delegate->sizeHint(
        styleOption, rootItem->index() );

    wRow = qMax( wRow , rootHint.width() );
    if ( wRow > w )
      w = wRow;

    h += rootHint.height();
  }

  int left, right, top, bottom;
  getContentsMargins( &left, &top, &right, &bottom );

  w += left + right;
  h += top + bottom;

  return QSize( w, h );
}

ThermoDAQ2Legend::ThermoDAQ2Legend(QWidget *parent)
  : QwtAbstractLegend(parent)
{
  d_listView = new LegendListView(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(d_listView);

  connect(d_listView, SIGNAL(clicked(const QModelIndex &)),
          this, SLOT(handleClick(const QModelIndex &)));
}

ThermoDAQ2Legend::~ThermoDAQ2Legend()
{

}

void ThermoDAQ2Legend::renderLegend( QPainter *painter,
                                     const QRectF &rect,
                                     bool fillBackground ) const
{
  if (fillBackground) {
    if (autoFillBackground() || testAttribute( Qt::WA_StyledBackground)) {
      qwtRenderBackground(painter, rect, d_listView);
    }
  }

  QStyleOptionViewItem styleOption;
  styleOption.initFrom(this);
  styleOption.decorationAlignment = Qt::AlignCenter;

  const QAbstractItemDelegate *delegate = d_listView->itemDelegate();

  const QStandardItemModel *mdl =
      qobject_cast<const QStandardItemModel*>(d_listView->model());

  painter->save();
  painter->translate(rect.topLeft());

  for (int row = 0; row < mdl->rowCount(); row++) {
    const QStandardItem *item = mdl->item(row);

    styleOption.rect = d_listView->visualRect( item->index() );
    if (!styleOption.rect.isEmpty()) {
      delegate->paint(painter, styleOption, item->index());
    }
  }

  painter->restore();
}

int ThermoDAQ2Legend::scrollExtent( Qt::Orientation orientation ) const
{
  Q_UNUSED( orientation );

  return style()->pixelMetric( QStyle::PM_ScrollBarExtent );
}

void ThermoDAQ2Legend::updateLegend(const QVariant &itemInfo,
                                    const QList<QwtLegendData> &data)
{
  QStandardItemModel *mdl = qobject_cast<QStandardItemModel*>(d_listView->model());

  QwtPlotItem *plotItem = qvariant_cast<QwtPlotItem *>(itemInfo);

  QList<QStandardItem*> itemList = d_listView->itemList(plotItem);

  while (itemList.size()>data.size()) {
    QStandardItem *item = itemList.takeLast();
    mdl->removeRow(item->row());
  }

  if (!data.isEmpty()) {

    while (itemList.size() < data.size()) {
      QStandardItem *item = new QStandardItem();
      item->setEditable(false);
      item->setData(qlonglong(plotItem));
      ThermoDAQ2DisplayPlotItem* titem = (ThermoDAQ2DisplayPlotItem*)plotItem;
      item->setCheckable(titem->isEnabled());
      item->setCheckState(plotItem->isVisible() ? Qt::Checked : Qt::Unchecked );

      itemList += item;
      mdl->appendRow(item);
    }

    for (int i = 0;i<itemList.size();i++)
      updateItem(itemList[i], data[i]);
  }

  d_listView->updateGeometry();
}

void ThermoDAQ2Legend::updateItem(QStandardItem *item, const QwtLegendData &data)
{
  const QVariant key = item->data();
  const qlonglong ptr = key.value<qlonglong>();
  ThermoDAQ2DisplayPlotItem* titem = (ThermoDAQ2DisplayPlotItem*)ptr;

  item->setCheckable(titem->isEnabled());
  if (titem->isEnabled()==false) {
    item->setCheckState(Qt::Unchecked);
  }

  const QVariant titleValue = data.value( QwtLegendData::TitleRole );

  QwtText title;
  if ( titleValue.canConvert<QwtText>() )
  {
    item->setText( title.text() );
    title = titleValue.value<QwtText>();
  }
  else if ( titleValue.canConvert<QString>() )
  {
    title.setText( titleValue.value<QString>() );
  }
  item->setText( title.text() );

  const QVariant iconValue = data.value(QwtLegendData::IconRole);

  QPixmap pm;
  if (iconValue.canConvert<QPixmap>()) {
    pm = iconValue.value<QPixmap>();
  }

  item->setData(pm, Qt::DecorationRole);
}

void ThermoDAQ2Legend::handleClick( const QModelIndex &index )
{
  const QStandardItemModel *model =
      qobject_cast<QStandardItemModel *>( d_listView->model() );

  const QStandardItem *item = model->itemFromIndex( index );
  if (item->isCheckable()) {
    const qlonglong ptr = item->data().value<qlonglong>();

    emit checked((QwtPlotItem*)ptr,
                 item->checkState() == Qt::Checked, 0);
  }
}

bool ThermoDAQ2Legend::isEmpty() const
{
  return d_listView->model()->rowCount() == 0;
}

ThermoDAQ2DateScaleDraw::ThermoDAQ2DateScaleDraw(Qt::TimeSpec timeSpec)
  : QwtDateScaleDraw(timeSpec)
{
  setDateFormat(QwtDate::Second, "hh:mm:ss");
  setDateFormat(QwtDate::Minute, "hh:mm:ss");
  setDateFormat(QwtDate::Hour, "hh:mm:ss");
  setDateFormat(QwtDate::Day, "hh:mm:ss\ndd/MM/yyyy");
  setDateFormat(QwtDate::Week, "hh:mm:ss\ndd/MM/yyyy");
  setDateFormat(QwtDate::Month, "hh:mm:ss\ndd/MM/yyyy");
}

ThermoDAQ2VoltagePicker::ThermoDAQ2VoltagePicker(QWidget *parent, Qt::TimeSpec spec)
  : QwtPlotPicker(parent),
    timeSpec_(spec)
{
  setRubberBandPen(QColor(Qt::darkGreen));
  setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText ThermoDAQ2VoltagePicker::trackerTextF(const QPointF &pos) const
{
  const QDateTime dt = QwtDate::toDateTime(pos.x(), timeSpec_);

  QString s;
  s += dt.toString(" hh:mm:ss ");
  s += QString::number(pos.y(), 'f', 2);
  s += QString::fromUtf8(" V ");

  QwtText text(s);
  text.setColor(Qt::white);

  QColor c = rubberBandPen().color();
  text.setBorderPen(QPen(c));
  text.setBorderRadius(0);
  c.setAlpha(200);
  text.setBackgroundBrush(c);

  return text;
}

ThermoDAQ2CurrentPicker::ThermoDAQ2CurrentPicker(QWidget *parent, Qt::TimeSpec spec)
  : QwtPlotPicker(parent),
    timeSpec_(spec)
{
  setRubberBandPen(QColor(Qt::darkGreen));
  setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText ThermoDAQ2CurrentPicker::trackerTextF(const QPointF &pos) const
{
  const QDateTime dt = QwtDate::toDateTime(pos.x(), timeSpec_);

  QString s;
  s += dt.toString(" hh:mm:ss ");
  s += QString::number(pos.y(), 'f', 3);
  s += QString::fromUtf8(" A ");

  QwtText text(s);
  text.setColor(Qt::white);

  QColor c = rubberBandPen().color();
  text.setBorderPen(QPen(c));
  text.setBorderRadius(0);
  c.setAlpha(200);
  text.setBackgroundBrush(c);

  return text;
}

ThermoDAQ2TemperaturePicker::ThermoDAQ2TemperaturePicker(QWidget *parent, Qt::TimeSpec spec)
  : QwtPlotPicker(parent),
    timeSpec_(spec)
{
  setRubberBandPen(QColor(Qt::darkGreen));
  setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText ThermoDAQ2TemperaturePicker::trackerTextF(const QPointF &pos) const
{
  const QDateTime dt = QwtDate::toDateTime(pos.x(), timeSpec_);

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

ThermoDAQ2PressurePicker::ThermoDAQ2PressurePicker(QWidget *parent, Qt::TimeSpec spec)
  : QwtPlotPicker(parent),
    timeSpec_(spec)
{
  setRubberBandPen(QColor(Qt::darkGreen));
  setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText ThermoDAQ2PressurePicker::trackerTextF(const QPointF &pos) const
{
  const QDateTime dt = QwtDate::toDateTime(pos.x(), timeSpec_);

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

ThermoDAQ2OverPressurePicker::ThermoDAQ2OverPressurePicker(QWidget *parent, Qt::TimeSpec spec)
  : QwtPlotPicker(parent),
    timeSpec_(spec)
{
  setRubberBandPen(QColor(Qt::darkGreen));
  setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText ThermoDAQ2OverPressurePicker::trackerTextF(const QPointF &pos) const
{
  const QDateTime dt = QwtDate::toDateTime(pos.x(), timeSpec_);

  QString s;
  s += dt.toString(" hh:mm:ss ");
  if (pos.y()<1) {
    s += QString::number(pos.y(), 'f', 2);
  } else {
    s += QString::number((int)pos.y());
  }
  s += " bar ";

  QwtText text(s);
  text.setColor(Qt::white);

  QColor c = rubberBandPen().color();
  text.setBorderPen(QPen(c));
  text.setBorderRadius(0);
  c.setAlpha(200);
  text.setBackgroundBrush(c);

  return text;
}

ThermoDAQ2DisplayWidget::ThermoDAQ2DisplayWidget(QWidget *parent)
  : QwtPlot(parent),
    zoomer_(0)
{
  setCanvasBackground(QBrush(QColor(Qt::black)));
}

void ThermoDAQ2DisplayWidget::exportPlot()
{
  QwtPlotRenderer renderer;
  renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, true);
  renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground, true);
  renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasFrame, true);
  renderer.setDiscardFlag(QwtPlotRenderer::DiscardLegend, true);
  renderer.exportTo(this, "thermo2Display", QSizeF(300, 200), 300);
}

void ThermoDAQ2DisplayWidget::showItem(QwtPlotItem* item, bool on)
{
  ThermoDAQ2DisplayPlotItem* titem = static_cast<ThermoDAQ2DisplayPlotItem*>(item);
  if (titem->isEnabled()) {
    titem->setVisible(on);
    replot();
  }
}

ThermoDAQ2VoltageDisplayWidget::ThermoDAQ2VoltageDisplayWidget(QWidget *parent)
  : ThermoDAQ2DisplayWidget(parent)
{
  ThermoDAQ2DateScaleDraw* scaleDraw = new ThermoDAQ2DateScaleDraw(Qt::LocalTime);
  QwtDateScaleEngine* scaleEngine = new QwtDateScaleEngine(Qt::LocalTime);

  this->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
  this->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
  this->setAxisLabelRotation(QwtPlot::xBottom, -45.0);
  this->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

  QwtLinearScaleEngine * yscaleEngine = new QwtLinearScaleEngine();
  yscaleEngine->setMargins(0.5, 1.5);
  this->setAxisScaleEngine(QwtPlot::yLeft, yscaleEngine);
  this->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("Voltage [V]"));

  /*ThermoDAQ2VoltagePicker * picker = */new ThermoDAQ2VoltagePicker(this->canvas(),
                                                                 Qt::LocalTime);

  internalLegend_ = new ThermoDAQ2InternalLegend();
  internalLegend_->setMaxColumns(1);
  internalLegend_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  internalLegend_->attach(this);

  ThermoDAQ2Legend *legend = new ThermoDAQ2Legend(this);
  this->insertLegend(legend, QwtPlot::RightLegend);
  connect(legend, SIGNAL(checked(QwtPlotItem*,bool,int)),
          this, SLOT(showItem(QwtPlotItem*,bool)));
}

ThermoDAQ2CurrentDisplayWidget::ThermoDAQ2CurrentDisplayWidget(QWidget *parent)
  : ThermoDAQ2DisplayWidget(parent)
{
  ThermoDAQ2DateScaleDraw* scaleDraw = new ThermoDAQ2DateScaleDraw(Qt::LocalTime);
  QwtDateScaleEngine* scaleEngine = new QwtDateScaleEngine(Qt::LocalTime);

  this->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
  this->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
  this->setAxisLabelRotation(QwtPlot::xBottom, -45.0);
  this->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

  QwtLinearScaleEngine * yscaleEngine = new QwtLinearScaleEngine();
  yscaleEngine->setMargins(0.5, 1.5);
  this->setAxisScaleEngine(QwtPlot::yLeft, yscaleEngine);
  this->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("Current [A]"));

  /*ThermoDAQ2CurrentPicker * picker = */new ThermoDAQ2CurrentPicker(this->canvas(),
                                                                 Qt::LocalTime);

  internalLegend_ = new ThermoDAQ2InternalLegend();
  internalLegend_->setMaxColumns(1);
  internalLegend_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  internalLegend_->attach(this);

  ThermoDAQ2Legend *legend = new ThermoDAQ2Legend(this);
  this->insertLegend(legend, QwtPlot::RightLegend);
  connect(legend, SIGNAL(checked(QwtPlotItem*,bool,int)),
          this, SLOT(showItem(QwtPlotItem*,bool)));
}

ThermoDAQ2TemperatureDisplayWidget::ThermoDAQ2TemperatureDisplayWidget(QWidget *parent)
  : ThermoDAQ2DisplayWidget(parent)
{
  ThermoDAQ2DateScaleDraw* scaleDraw = new ThermoDAQ2DateScaleDraw(Qt::LocalTime);
  QwtDateScaleEngine* scaleEngine = new QwtDateScaleEngine(Qt::LocalTime);

  this->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
  this->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
  this->setAxisLabelRotation(QwtPlot::xBottom, -45.0);
  this->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

  QwtLinearScaleEngine * yscaleEngine = new QwtLinearScaleEngine();
  yscaleEngine->setMargins(1.5, 1.5);
  this->setAxisScaleEngine(QwtPlot::yLeft, yscaleEngine);
  this->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("T [°C]"));

  /*ThermoDAQ2TemperaturePicker * picker = */new ThermoDAQ2TemperaturePicker(this->canvas(),
                                                                         Qt::LocalTime);

  internalLegend_ = new ThermoDAQ2InternalLegend();
  internalLegend_->setMaxColumns(2);
  internalLegend_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  internalLegend_->attach(this);

  ThermoDAQ2Legend *legend = new ThermoDAQ2Legend(this);
  this->insertLegend(legend, QwtPlot::RightLegend);
  connect(legend, SIGNAL(checked(QwtPlotItem*,bool,int)),
          this, SLOT(showItem(QwtPlotItem*,bool)));
}

ThermoDAQ2PressureDisplayWidget::ThermoDAQ2PressureDisplayWidget(QWidget *parent)
  : ThermoDAQ2DisplayWidget(parent)
{
  ThermoDAQ2DateScaleDraw* scaleDraw = new ThermoDAQ2DateScaleDraw(Qt::LocalTime);
  QwtDateScaleEngine* scaleEngine = new QwtDateScaleEngine(Qt::LocalTime);

  this->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
  this->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
  this->setAxisLabelRotation(QwtPlot::xBottom, -45.0);
  this->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

  QwtLogScaleEngine * yscaleEngine = new ThermoDAQ2PressureScaleEngine();
  yscaleEngine->setMargins(1, 0);
  this->setAxisScaleEngine(QwtPlot::yLeft, yscaleEngine);
  this->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("p [mbar]"));

  ThermoDAQ2PressureScaleDraw* pscaleDraw = new ThermoDAQ2PressureScaleDraw();
  this->setAxisScaleDraw(QwtPlot::yLeft, pscaleDraw);

  /*ThermoDAQ2PressurePicker * picker = */new ThermoDAQ2PressurePicker(this->canvas(),
                                                                   Qt::LocalTime);

  internalLegend_ = new ThermoDAQ2InternalLegend();
  internalLegend_->setMaxColumns(1);
  internalLegend_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  internalLegend_->attach(this);

  ThermoDAQ2Legend *legend = new ThermoDAQ2Legend(this);
  this->insertLegend(legend, QwtPlot::RightLegend);
  connect(legend, SIGNAL(checked(QwtPlotItem*,bool,int)),
          this, SLOT(showItem(QwtPlotItem*,bool)));
}
