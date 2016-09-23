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
