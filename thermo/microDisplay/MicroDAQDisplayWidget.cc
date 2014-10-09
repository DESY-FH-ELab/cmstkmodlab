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

#include "MicroDAQDisplayWidget.h"

MicroDAQPressureScaleDraw::MicroDAQPressureScaleDraw() :
    QwtScaleDraw()
{

}

MicroDAQPressureScaleDraw::~MicroDAQPressureScaleDraw()
{

}

QwtText MicroDAQPressureScaleDraw::label(double value) const
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

MicroDAQPressureScaleEngine::MicroDAQPressureScaleEngine(uint base) :
    QwtLogScaleEngine(base)
{

}

MicroDAQPressureScaleEngine::~MicroDAQPressureScaleEngine()
{

}

void MicroDAQPressureScaleEngine::autoScale(int maxNumSteps,
                                             double &x1, double &x2,
                                             double &stepSize) const
{
    QwtLogScaleEngine::autoScale(maxNumSteps, x1, x2, stepSize);
    if (x2>2000) x2 = 2000;
}

static void qwtRenderBackground(QPainter *painter,
                                const QRectF &rect, const QWidget *widget )
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
    LegendListView(MicroDAQLegend *);

    QList<QStandardItem *> itemList(const QwtPlotItem *);

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
};

LegendListView::LegendListView(MicroDAQLegend *legend) :
    QListView(legend)
{
    setFrameStyle(NoFrame);
    viewport()->setBackgroundRole(QPalette::Background);
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

MicroDAQLegend::MicroDAQLegend(QWidget *parent):
    QwtAbstractLegend(parent)
{
    d_listView = new LegendListView(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d_listView);

    connect(d_listView, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(handleClick(const QModelIndex &)));
}

MicroDAQLegend::~MicroDAQLegend()
{
}

void MicroDAQLegend::renderLegend( QPainter *painter,
    const QRectF &rect, bool fillBackground ) const
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

int MicroDAQLegend::scrollExtent( Qt::Orientation orientation ) const
{
    Q_UNUSED( orientation );

    return style()->pixelMetric( QStyle::PM_ScrollBarExtent );
}

void MicroDAQLegend::updateLegend(const QVariant &itemInfo,
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
            MicroDAQDisplayPlotItem* titem = (MicroDAQDisplayPlotItem*)plotItem;
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

void MicroDAQLegend::updateItem(QStandardItem *item, const QwtLegendData &data)
{
    const QVariant key = item->data();
    const qlonglong ptr = key.value<qlonglong>();
    MicroDAQDisplayPlotItem* titem = (MicroDAQDisplayPlotItem*)ptr;

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

void MicroDAQLegend::handleClick( const QModelIndex &index )
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

bool MicroDAQLegend::isEmpty() const
{
    return d_listView->model()->rowCount() == 0;
}


MicroDAQDateScaleDraw::MicroDAQDateScaleDraw(Qt::TimeSpec timeSpec) :
    QwtDateScaleDraw(timeSpec)
{
    setDateFormat(QwtDate::Second, "hh:mm:ss");
    setDateFormat(QwtDate::Minute, "hh:mm:ss");
    setDateFormat(QwtDate::Hour, "hh:mm:ss");
    setDateFormat(QwtDate::Day, "hh:mm:ss\ndd/MM/yyyy");
    setDateFormat(QwtDate::Week, "hh:mm:ss\ndd/MM/yyyy");
    setDateFormat(QwtDate::Month, "hh:mm:ss\ndd/MM/yyyy");
}

MicroDAQTemperaturePicker::MicroDAQTemperaturePicker(QWidget *parent, Qt::TimeSpec spec) :
    QwtPlotPicker(parent),
    timeSpec_(spec)
{
    setRubberBandPen(QColor(Qt::darkGreen));
    setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText MicroDAQTemperaturePicker::trackerTextF(const QPointF &pos) const
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

MicroDAQPressurePicker::MicroDAQPressurePicker(QWidget *parent, Qt::TimeSpec spec) :
    QwtPlotPicker(parent),
    timeSpec_(spec)
{
    setRubberBandPen(QColor(Qt::darkGreen));
    setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText MicroDAQPressurePicker::trackerTextF(const QPointF &pos) const
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

MicroDAQOverPressurePicker::MicroDAQOverPressurePicker(QWidget *parent, Qt::TimeSpec spec) :
    QwtPlotPicker(parent),
    timeSpec_(spec)
{
    setRubberBandPen(QColor(Qt::darkGreen));
    setTrackerMode(QwtPlotPicker::AlwaysOn);
}

QwtText MicroDAQOverPressurePicker::trackerTextF(const QPointF &pos) const
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

MicroDAQDisplayWidget::MicroDAQDisplayWidget(QWidget *parent) :
    QwtPlot(parent),
    zoomer_(0)
{
    setCanvasBackground(QBrush(QColor(Qt::black)));
}

void MicroDAQDisplayWidget::exportPlot()
{
    QwtPlotRenderer renderer;
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, true);
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground, true);
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasFrame, true);
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardLegend, true);
    renderer.exportTo(this, "MicroDisplay", QSizeF(300, 200), 300);
}

void MicroDAQDisplayWidget::showItem(QwtPlotItem* item, bool on)
{
    MicroDAQDisplayPlotItem* titem = static_cast<MicroDAQDisplayPlotItem*>(item);
    if (titem->isEnabled()) {
        titem->setVisible(on);
        replot();
    }
}

MicroDAQTemperatureDisplayWidget::MicroDAQTemperatureDisplayWidget(QWidget *parent) :
    MicroDAQDisplayWidget(parent)
{
    MicroDAQDateScaleDraw* scaleDraw = new MicroDAQDateScaleDraw(Qt::LocalTime);
    QwtDateScaleEngine* scaleEngine = new QwtDateScaleEngine(Qt::LocalTime);

    this->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
    this->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
    this->setAxisLabelRotation(QwtPlot::xBottom, -45.0);
    this->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    QwtLinearScaleEngine * yscaleEngine = new QwtLinearScaleEngine();
    yscaleEngine->setMargins(1.5, 1.5);
    this->setAxisScaleEngine(QwtPlot::yLeft, yscaleEngine);
    this->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("T [°C]"));

    MicroDAQTemperaturePicker * picker = new MicroDAQTemperaturePicker(this->canvas(),
                                                                         Qt::LocalTime);

    internalLegend_ = new MicroDAQInternalLegend();
    internalLegend_->setMaxColumns(1);
    internalLegend_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    internalLegend_->attach(this);

    MicroDAQLegend *legend = new MicroDAQLegend(this);
    this->insertLegend(legend, QwtPlot::RightLegend);
    connect(legend, SIGNAL(checked(QwtPlotItem*,bool,int)),
            this, SLOT(showItem(QwtPlotItem*,bool)));
}

MicroDAQPressureDisplayWidget::MicroDAQPressureDisplayWidget(QWidget *parent) :
    MicroDAQDisplayWidget(parent)
{
    MicroDAQDateScaleDraw* scaleDraw = new MicroDAQDateScaleDraw(Qt::LocalTime);
    QwtDateScaleEngine* scaleEngine = new QwtDateScaleEngine(Qt::LocalTime);

    this->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
    this->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
    this->setAxisLabelRotation(QwtPlot::xBottom, -45.0);
    this->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    QwtLogScaleEngine * yscaleEngine = new MicroDAQPressureScaleEngine();
    yscaleEngine->setMargins(1, 0);
    this->setAxisScaleEngine(QwtPlot::yLeft, yscaleEngine);
    this->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("p [mbar]"));

    MicroDAQPressureScaleDraw* pscaleDraw = new MicroDAQPressureScaleDraw();
    this->setAxisScaleDraw(QwtPlot::yLeft, pscaleDraw);

    MicroDAQPressurePicker * picker = new MicroDAQPressurePicker(this->canvas(),
                                                                   Qt::LocalTime);

    internalLegend_ = new MicroDAQInternalLegend();
    internalLegend_->setMaxColumns(1);
    internalLegend_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    internalLegend_->attach(this);

    MicroDAQLegend *legend = new MicroDAQLegend(this);
    this->insertLegend(legend, QwtPlot::RightLegend);
    connect(legend, SIGNAL(checked(QwtPlotItem*,bool,int)),
            this, SLOT(showItem(QwtPlotItem*,bool)));
}

MicroDAQMicroPressureDisplayWidget::MicroDAQMicroPressureDisplayWidget(QWidget *parent) :
    MicroDAQDisplayWidget(parent)
{
    MicroDAQDateScaleDraw* scaleDraw = new MicroDAQDateScaleDraw(Qt::LocalTime);
    QwtDateScaleEngine* scaleEngine = new QwtDateScaleEngine(Qt::LocalTime);

    this->setAxisScaleDraw(QwtPlot::xBottom, scaleDraw);
    this->setAxisScaleEngine(QwtPlot::xBottom, scaleEngine);
    this->setAxisLabelRotation(QwtPlot::xBottom, -45.0);
    this->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    QwtLinearScaleEngine * yscaleEngine = new QwtLinearScaleEngine();
    yscaleEngine->setMargins(1.5, 1.5);
    this->setAxisScaleEngine(QwtPlot::yLeft, yscaleEngine);
    this->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("p [bar]"));

    MicroDAQPressureScaleDraw* pscaleDraw = new MicroDAQPressureScaleDraw();
    this->setAxisScaleDraw(QwtPlot::yLeft, pscaleDraw);

    MicroDAQOverPressurePicker * picker = new MicroDAQOverPressurePicker(this->canvas(),
								       Qt::LocalTime);

    internalLegend_ = new MicroDAQInternalLegend();
    internalLegend_->setMaxColumns(1);
    internalLegend_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    internalLegend_->attach(this);

    MicroDAQLegend *legend = new MicroDAQLegend(this);
    this->insertLegend(legend, QwtPlot::RightLegend);
    connect(legend, SIGNAL(checked(QwtPlotItem*,bool,int)),
            this, SLOT(showItem(QwtPlotItem*,bool)));
}
