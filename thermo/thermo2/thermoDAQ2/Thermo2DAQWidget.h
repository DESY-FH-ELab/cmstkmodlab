/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMO2DAQWIDGET_H
#define THERMO2DAQWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QDoubleSpinBox>

#include "Thermo2DAQModel.h"

class Thermo2DAQWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit Thermo2DAQWidget(const QString& title,
                              Thermo2DAQModel* DAQModel,
                              QWidget *parent = 0);

protected:
    // Models
    Thermo2DAQModel* daqModel_;

    // Widgets
    QPushButton* startMeasurementButton_;
    QPushButton* stopMeasurementButton_;
    QPushButton* clearHistoryButton_;
    QWidget* buttons_;

    QPlainTextEdit* logEdit_;
    QPushButton* logButton_;

protected slots:
    void daqStateChanged(bool running);
    void logButtonClicked();
    void stopMeasurement();
};

#endif // THERMODAQWIDGET_H
