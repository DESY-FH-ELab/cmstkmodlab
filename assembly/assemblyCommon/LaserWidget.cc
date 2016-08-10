#include <iostream>
#include <nqlogger.h>
#include <QPalette>
#include <QColor>

#include "LaserWidget.h"

LaserWidget::LaserWidget(QWidget *parent)
    : QLCDNumber(parent)
{

    std::cout<<"LaserWidget " << "[LaserWidget]"    <<std::endl;
}

LaserWidget::~LaserWidget()
{
}

void LaserWidget::updateDisplay(double value)
{
    std::cout<<"LaserWidget " << "[updateDisplay]"    <<std::endl;
    this->display(value);
    this->repaint();
    emit finished();
}

void LaserWidget::backgroundColorUpdate(bool isInRange)
{
    std::cout<<"LaserWidget " << "[backgroundColorUpdate] : " << isInRange    <<std::endl;
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
