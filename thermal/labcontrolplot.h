#ifndef LABCONTROLPLOT_H
#define LABCONTROLPLOT_H

#include <QVector>

#include <qwt_plot_curve.h>

#include "labcontroldatacollector.h"

class LabControlPlot : public QwtPlotCurve
{
 public:

    LabControlPlot(const QString &title, const QPen & pen);

    void (LabControlDataCollector::*getDataVector_)(unsigned int, unsigned int,
                                                    QVector<double> &);
    void (LabControlDataCollector::*getChannelDataVector_)(unsigned int,
                                                           unsigned int, unsigned int,
                                                           QVector<double> &);
    unsigned int channel_;
};

#endif // LABCONTROLPLOT_H
