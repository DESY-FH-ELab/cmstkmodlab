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
#include <QSignalSpy>
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
