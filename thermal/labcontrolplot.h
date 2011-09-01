#ifndef LABCONTROLPLOT_H
#define LABCONTROLPLOT_H

#include <qwt_plot_curve.h>
#include <qwt_array.h>

#include "labcontroldatacollector.h"

class LabControlPlot : public QwtPlotCurve
{
 public:

    LabControlPlot(const QString &title, const QPen & pen);

    void (LabControlDataCollector::*getDataVector_)(unsigned int, unsigned int,
                                                    QwtArray<double> &);
    void (LabControlDataCollector::*getChannelDataVector_)(unsigned int,
                                                           unsigned int, unsigned int,
                                                           QwtArray<double> &);
    unsigned int channel_;
};

#endif // LABCONTROLPLOT_H
