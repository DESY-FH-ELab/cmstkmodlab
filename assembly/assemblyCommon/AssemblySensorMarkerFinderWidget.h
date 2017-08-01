/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
