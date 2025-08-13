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

#include <iostream>
#include <nqlogger.h>
#include <QPalette>
#include <QColor>

#include "LaserWidget.h"

LaserWidget::LaserWidget(LaserModel* laserModel, QWidget *parent)
    : QLCDNumber(parent),
      laserModel_(laserModel)
{
    setDigitCount(5);

    //    NQLog("LaserWidget ", NQLog::Debug) << "[LaserWidget]";

    connect(laserModel_, SIGNAL(measurementChanged(double)),
	this, SLOT(updateDisplay(double)));

    connect(laserModel_, SIGNAL(inRangeStateChanged(bool)),
	this, SLOT(backgroundColorUpdate(bool)));
}

LaserWidget::~LaserWidget()
{
}

void LaserWidget::updateDisplay(double value)
{
  //    NQLog("LaserWidget ", NQLog::Debug) << "[updateDisplay]";
    this->display(value);
    this->repaint();
    emit finished();
}

void LaserWidget::backgroundColorUpdate(bool isInRange)
{
  //    NQLog("LaserWidget ", NQLog::Debug) << "[backgroundColorUpdate] : " << isInRange;
    QPalette *lcdpalette = new QPalette;
    if(!isInRange){
        lcdpalette->setColor(QPalette::WindowText, QColor(255, 0, 0));
    }else{
        lcdpalette->setColor(QPalette::WindowText, QColor(76, 76, 76));
    }
    this->setPalette(*lcdpalette);
    
    //    emit updateDisplay(999);
    delete lcdpalette;
}
