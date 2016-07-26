#include <nqlogger.h>
#include <QPalette>
#include <QColor>

#include "LaserWidget.h"

LaserWidget::LaserWidget(QWidget *parent)
    : QLCDNumber(parent)
{
    NQLog("LaserWidget") << "[LaserWidget]";
}

LaserWidget::~LaserWidget()
{
}

void LaserWidget::updateDisplay(double value)
{
    NQLog("LaserWidget") << "[updateDisplay]";
    this->display(value);
    this->repaint();
    emit finished();
}

void LaserWidget::backgroundColorUpdate(bool isOutOfRange)
{
    NQLog("LaserWidget") << "[backgroundColorUpdate] : " << isOutOfRange;
    QPalette *lcdpalette = new QPalette;
    if(isOutOfRange){
        lcdpalette->setColor(QPalette::WindowText, QColor(255, 0, 0));
    }else{
        lcdpalette->setColor(QPalette::WindowText, QColor(76, 76, 76));
    }
    this->setPalette(*lcdpalette);
    
    emit updateDisplay(999);
    delete lcdpalette;
}
