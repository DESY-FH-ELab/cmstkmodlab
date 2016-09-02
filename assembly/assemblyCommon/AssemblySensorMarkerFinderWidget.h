#ifndef ASSEMBLYSENSORMARKERWIDGET_H
#define ASSEMBLYSENSORMARKERWIDGET_H

#include <QToolBox>

#include "AssemblySensorMarkerFinder.h"

class AssemblySensorMarkerFinderWidget : public QToolBox
{
    Q_OBJECT
public:
    explicit AssemblySensorMarkerFinderWidget(AssemblySensorMarkerFinder *finder,
                                              QWidget *parent = 0);

protected:

    AssemblySensorMarkerFinder* finder_;

public slots:

};

#endif // ASSEMBLYSENSORMARKERWIDGET_H
