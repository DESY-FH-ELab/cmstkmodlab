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

#ifndef THERMODAQWIDGET_H
#define THERMODAQWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QDoubleSpinBox>

#include "ThermoDAQModel.h"

class ThermoDAQWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ThermoDAQWidget(ThermoDAQModel* DAQModel, QWidget *parent = 0);

protected:
    // Models
    ThermoDAQModel* daqModel_;

    // Widgets
    QPushButton* startMeasurementButton_;
    QPushButton* stopMeasurementButton_;
    QPushButton* clearHistoryButton_;
    QWidget* buttons_;

    QDoubleSpinBox* sampleThickness_;
    QDoubleSpinBox* sampleArea_;
    QWidget* metadata_;

    QPlainTextEdit* logEdit_;
    QPushButton* logButton_;

protected slots:
    void daqStateChanged(bool running);
    void logButtonClicked();
    void stopMeasurement();
    void sampleThicknessChanged(double value);
    void sampleAreaChanged(double value);
};

#endif // THERMODAQWIDGET_H
