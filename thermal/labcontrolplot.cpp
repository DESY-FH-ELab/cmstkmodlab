#include "labcontrolplot.h"

LabControlPlot::LabControlPlot(const QString &title, const QPen & pen)
        : QwtPlotCurve(title)
{
    setPen(pen);

    getDataVector_ = NULL;
    getChannelDataVector_ = NULL;

    channel_ = 0;
}
