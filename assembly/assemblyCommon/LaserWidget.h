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

#ifndef LASERWIDGET_H
#define LASERWIDGET_H

#include <vector>

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>

#include "LaserModel.h"

class LaserWidget : public QLCDNumber
{
    Q_OBJECT

public:
    explicit LaserWidget(LaserModel* laserModel,  QWidget *parent = 0);
    ~LaserWidget();

signals:
        void finished();

protected:
        LaserModel* laserModel_;

public slots:
    void updateDisplay(double value);
    void backgroundColorUpdate(bool isInRange);
};

#endif // LASERWIDGET_H
