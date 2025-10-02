/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef LASERCONTROLSWIDGET_H
#define LASERCONTROLSWIDGET_H

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
#include <QTimer>

#include "LaserModel.h"

class LaserControlsWidget : public QWidget
{
    Q_OBJECT

 public:
    explicit LaserControlsWidget(LaserModel* model, QWidget *parent = 0);

 protected:
    LaserModel* laserModel_;
    QComboBox* samplingBox_;
    QComboBox* averagingBox_;
    QComboBox* materialBox_;
    QComboBox* diffuseBox_;
    QCheckBox* checkBoxEnableLaser_;

    public slots:
        void laserStateChanged(State newState);
};

#endif // LASERCONTROLSWIDGET_H
