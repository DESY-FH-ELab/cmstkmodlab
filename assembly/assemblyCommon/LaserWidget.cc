#include <nqlogger.h>

#include "LaserWidget.h"

LaserWidget::LaserWidget(QWidget *parent)
    : QLCDNumber(parent)
{
}

void LaserWidget::updateDisplay(double value)
{
    this->display(value);
    this->repaint();
    emit finished();
}
