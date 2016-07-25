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

class LaserWidget : public QLCDNumber
{
    Q_OBJECT

public:
    LaserWidget(QWidget *parent = 0);
    ~LaserWidget();

signals:
        void finished();

protected:

public slots:
    void updateDisplay(double value);

};

#endif // LASERWIDGET_H
