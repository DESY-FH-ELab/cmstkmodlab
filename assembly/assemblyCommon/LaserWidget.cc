#include <nqlogger.h>

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
