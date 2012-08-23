#include <iostream>
#include <iomanip>
#include <ios>
#include <fstream>
#include <algorithm>

#include <QFileDialog>
#include <QTextStream>
#include <QDateTime>
#include <QColor>

#include <qwt_legend.h>
#include <qwt_symbol.h>

#ifdef USE_FAKEIO
#include "devices/Julabo/JulaboFP50Fake.h"
#include "devices/Hameg/Hameg8143Fake.h"
#else
#include "devices/Hameg/Hameg8143.h"
#include "devices/Julabo/JulaboFP50.h"
#endif

#include "labcontrol.h"
#include "ui_labcontrol.h"

const int updateTime = 5;
const int daqUpdateTime = 10;
// const int daqUpdateTime = 5;

LabControl::LabControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LabControl)
{
    readPreferences();

    ui->setupUi(this);
    ui->ExecutionTimeLabel->setText("");

    tempDisplay_[0] = ui->Channel0Display;
    tempDisplay_[1] = ui->Channel1Display;
    tempDisplay_[2] = ui->Channel2Display;
    tempDisplay_[3] = ui->Channel3Display;
    tempDisplay_[4] = ui->Channel4Display;
    tempDisplay_[5] = ui->Channel5Display;
    tempDisplay_[6] = ui->Channel6Display;
    tempDisplay_[7] = ui->Channel7Display;
    tempDisplay_[8] = ui->Channel8Display;
    tempDisplay_[9] = ui->Channel9Display;

    refreshTimer_ = new QTimer(this);
    connect(refreshTimer_, SIGNAL(timeout()), this, SLOT(refreshDisplays()));

    daqRunning_ = false;
    daqTimer_ = new QTimer(this);
    connect(daqTimer_, SIGNAL(timeout()), this, SLOT(collectData()));

    executionRunning_ = false;

#ifdef USE_FAKEIO
    julabo_ = new JulaboFP50Fake("/dev/ttyS0");
    hameg8143_ = new Hameg8143Fake("/dev/ttyUSB0");
#else
    julabo_ = new JulaboFP50("/dev/ttyS5");
    hameg8143_ = new Hameg8143("/dev/ttyUSB0");
#endif

    if (hameg8143_->DeviceAvailable()) {
        hameg8143_->SetRemoteMode(true);
        hameg8143_->SwitchOutputOff();
        hameg8143_->SetRemoteMode(false);
    }

    dataCollector_ = LabControlDataCollector(julabo_, hameg8143_);

    applyPreferences();

    refreshDisplays();
    refreshTimer_->start(1000*updateTime);

    on_ReadbathTempButton_clicked();
    on_ReadPIDButton_clicked();

    dataCollector_.collectStatusData();

    updateControls();

    ui->DisplayTempVsTime->setCanvasBackground(QColor(Qt::black));
}

LabControl::~LabControl()
{
    delete ui;
}

void LabControl::refreshDisplays()
{
    if (daqRunning_) return;

    dataCollector_.collectStatusData();

    ui->PumpPressureDisplay->display(static_cast<int>(dataCollector_.pumpPressure()));

    bool isCirculatorOn = dataCollector_.circulatorState();
    if (isCirculatorOn) {
        ui->CirculatorStatusDisplay->setText("ON");
        ui->CirculatorOnButton->setEnabled(false);
        ui->CirculatorOffButton->setEnabled(true);
    } else {
        ui->CirculatorStatusDisplay->setText("OFF");
        ui->CirculatorOnButton->setEnabled(true);
        ui->CirculatorOffButton->setEnabled(false);
    }

    ui->JulaboStatusDisplay->display(dataCollector_.julaboStatus().first);
    ui->JulaboStatusText->setPlainText(dataCollector_.julaboStatus().second.c_str());

    ui->JulaboHeatingPowerDisplay->display(static_cast<int>(dataCollector_.data().heatingPower()));

    char line[40];
    sprintf(line, "% .2f", dataCollector_.data().bathTemperature());
    ui->BathTempDisplay->display(line);
    sprintf(line, "% .2f", dataCollector_.data().safetySensorTemperature());
    ui->JulaboSafetyTempDisplay->display(line);

    if (hameg8143_->DeviceAvailable()) {
        ui->HamegU1Display->display(dataCollector_.hamegStatus().voltage(0));
        ui->HamegU1Control->setValue(dataCollector_.hamegStatus().setPointVoltage(0));
        ui->HamegI1Display->display(dataCollector_.hamegStatus().current(0));
        ui->HamegI1Control->setValue(dataCollector_.hamegStatus().currentLimit(0));

        ui->HamegU2Display->display(dataCollector_.hamegStatus().voltage(1));
        ui->HamegU2Control->setValue(dataCollector_.hamegStatus().setPointVoltage(1));
        ui->HamegI2Display->display(dataCollector_.hamegStatus().current(1));
        ui->HamegI2Control->setValue(dataCollector_.hamegStatus().currentLimit(1));

        if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmOP1) {
            ui->HamegOutputStatus->setText("Outputs Enabled");
        }
        if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmOP0) {
            ui->HamegOutputStatus->setText("Outputs Disabled");
        }

        if (dataCollector_.hamegStatus()!=dataCollector_.lastHamegStatus()) updateControls();
    }

    executeCommand();
}

void LabControl::collectData()
{
    if (!daqRunning_) return;

    dataCollector_.setDAQTime(daqTime_);

    bool lastCirculatorState = dataCollector_.circulatorState();

    dataCollector_.collectData();

    ui->PumpPressureDisplay->display(static_cast<int>(dataCollector_.pumpPressure()));

    bool isCirculatorOn = dataCollector_.circulatorState();
    if (isCirculatorOn) {
        ui->CirculatorStatusDisplay->setText("ON");
        ui->CirculatorOnButton->setEnabled(false);
        ui->CirculatorOffButton->setEnabled(true);
    } else {
        ui->CirculatorStatusDisplay->setText("OFF");
        ui->CirculatorOnButton->setEnabled(true);
        ui->CirculatorOffButton->setEnabled(false);
    }

    if (lastCirculatorState!=isCirculatorOn) {
        dataCollector_.writeStatus(*daqFile_);
    }

    ui->JulaboStatusDisplay->display(dataCollector_.julaboStatus().first);
    ui->JulaboStatusText->setPlainText(dataCollector_.julaboStatus().second.c_str());

    ui->JulaboHeatingPowerDisplay->display(static_cast<int>(dataCollector_.data().heatingPower()));

    char line[40];
    sprintf(line, "% .2f", dataCollector_.data().bathTemperature());
    ui->BathTempDisplay->display(line);
    sprintf(line, "% .2f", dataCollector_.data().safetySensorTemperature());
    ui->JulaboSafetyTempDisplay->display(line);

    if (hameg8143_->DeviceAvailable()) {
        ui->HamegU1Display->display(dataCollector_.hamegStatus().voltage(0));
        ui->HamegU1Control->setValue(dataCollector_.hamegStatus().setPointVoltage(0));
        ui->HamegI1Display->display(dataCollector_.hamegStatus().current(0));
        ui->HamegI1Control->setValue(dataCollector_.hamegStatus().currentLimit(0));

        ui->HamegU2Display->display(dataCollector_.hamegStatus().voltage(1));
        ui->HamegU2Control->setValue(dataCollector_.hamegStatus().setPointVoltage(1));
        ui->HamegI2Display->display(dataCollector_.hamegStatus().current(1));
        ui->HamegI2Control->setValue(dataCollector_.hamegStatus().currentLimit(1));

        if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmOP1) {
            ui->HamegOutputStatus->setText("Outputs Enabled");
        }
        if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmOP0) {
            ui->HamegOutputStatus->setText("Outputs Disabled");
        }

        if (dataCollector_.hamegStatus()!=dataCollector_.lastHamegStatus()) updateControls();
    }

    for (unsigned int i=0;i<10;i++) {
        if (dataCollector_.activeChannel(i)) {
            sprintf(line, "% .2f", dataCollector_.data().temperature(i));
            tempDisplay_[i]->display(line);
        }
    }
    if (dataCollector_.referenceActive()) {
        sprintf(line, "% .2f", dataCollector_.data().referenceTemperature());
        ui->RefDisplay->display(line);
    }

    ui->PlotStartSlider->setRange(0, daqTime_);
    ui->PlotStopSlider->setRange(0, daqTime_);

    if (ui->PlotStopSlider->value()!=0) {
        ui->PlotStopSlider->setSliderPosition(daqTime_-currentStopTime_);
    } else {
        currentStopTime_ = daqTime_;
    }

    QString label = QString::number(ui->PlotStartSlider->value());
    label += " s";
    ui->PlotStartLabel->setText(label);

    label = QString::number(daqTime_-ui->PlotStopSlider->value());
    label += " s";
    ui->PlotStopLabel->setText(label);

    ui->DAQTimeLCD->display((int)daqTime_);
    daqTime_ += daqUpdateTime;

    updatePlots();

    executeCommand();
}

void LabControl::updateControls()
{
    if (!hameg8143_->DeviceAvailable()) {
        ui->HamegU1Display->setEnabled(false);
        ui->HamegI1Display->setEnabled(false);
        ui->HamegU1Control->setEnabled(false);
        ui->HamegI1Control->setEnabled(false);
        ui->HamegU1Write->setEnabled(false);
        ui->HamegI1Write->setEnabled(false);

        ui->HamegU2Display->setEnabled(false);
        ui->HamegI2Display->setEnabled(false);
        ui->HamegU2Control->setEnabled(false);
        ui->HamegI2Control->setEnabled(false);
        ui->HamegU2Write->setEnabled(false);
        ui->HamegI2Write->setEnabled(false);

        ui->HamegRemoteModeCheck->setEnabled(false);
        ui->HamegMixedModeCheck->setEnabled(false);

        ui->HamegEnable->setEnabled(false);
        ui->HamegDisable->setEnabled(false);
    } else {
        if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmCV1) {
            ui->HamegCVMode1->setChecked(true);
            ui->HamegCVMode1->setEnabled(true);
            ui->HamegCCMode1->setChecked(false);
            ui->HamegCCMode1->setEnabled(false);
        }
        if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmCC1) {
            ui->HamegCVMode1->setChecked(false);
            ui->HamegCVMode1->setEnabled(false);
            ui->HamegCCMode1->setChecked(true);
            ui->HamegCCMode1->setEnabled(true);
        }
        if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmCV2) {
            ui->HamegCVMode2->setChecked(true);
            ui->HamegCVMode2->setEnabled(true);
            ui->HamegCCMode2->setChecked(false);
            ui->HamegCCMode2->setEnabled(false);
        }
        if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmCC2) {
            ui->HamegCVMode2->setChecked(false);
            ui->HamegCVMode2->setEnabled(false);
            ui->HamegCCMode2->setChecked(true);
            ui->HamegCCMode2->setEnabled(true);
        }
	
	if (executionRunning_) {
	
	  ui->HamegU1Control->setEnabled(false);
	  ui->HamegU1Write->setEnabled(false);
	  ui->HamegI1Control->setEnabled(false);
	  ui->HamegI1Write->setEnabled(false);
	  ui->HamegU2Control->setEnabled(false);
	  ui->HamegU2Write->setEnabled(false);
	  ui->HamegI2Control->setEnabled(false);
	  ui->HamegI2Write->setEnabled(false);
	  
	  ui->HamegRemoteModeCheck->setEnabled(false);
	  ui->HamegMixedModeCheck->setEnabled(false);
	  
	  ui->HamegEnable->setEnabled(false);
	  ui->HamegDisable->setEnabled(false);
	  
	} else {

	  ui->HamegU1Control->setEnabled(true);
	  ui->HamegU1Write->setEnabled(true);
	  ui->HamegI1Control->setEnabled(true);
	  ui->HamegI1Write->setEnabled(true);
	  ui->HamegU2Control->setEnabled(true);
	  ui->HamegU2Write->setEnabled(true);
	  ui->HamegI2Control->setEnabled(true);
	  ui->HamegI2Write->setEnabled(true);
	  
	  ui->HamegRemoteModeCheck->setEnabled(true);
	  ui->HamegMixedModeCheck->setEnabled(true);
	  
	  ui->HamegEnable->setEnabled(true);
	  ui->HamegDisable->setEnabled(true);

	}

	if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmRM0) {
	  ui->HamegRemoteModeCheck->setChecked(false);
	}
	if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmRM1) {
	  ui->HamegRemoteModeCheck->setChecked(true);
	}
	
	if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmRM0) {
	  ui->HamegEnable->setEnabled(false);
	  ui->HamegDisable->setEnabled(false);
	  
	  ui->HamegU1Control->setEnabled(false);
	  ui->HamegI1Control->setEnabled(false);
	  ui->HamegU1Write->setEnabled(false);
	  ui->HamegI1Write->setEnabled(false);
	  
	  ui->HamegU2Control->setEnabled(false);
	  ui->HamegI2Control->setEnabled(false);
	  ui->HamegU2Write->setEnabled(false);
	  ui->HamegI2Write->setEnabled(false);
	}
	if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmRM1) {
	  if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmOP0) {
	    ui->HamegEnable->setEnabled(true);
	    ui->HamegDisable->setEnabled(false);
	  }
	  if (dataCollector_.hamegStatus().statusBits()&VHameg8143::hmOP1) {
	    ui->HamegEnable->setEnabled(false);
	    ui->HamegDisable->setEnabled(true);
	  }
	  
	  ui->HamegU1Control->setEnabled(true);
	  ui->HamegI1Control->setEnabled(true);
	  ui->HamegU1Write->setEnabled(true);
	  ui->HamegI1Write->setEnabled(true);
	  
	  ui->HamegU2Control->setEnabled(true);
	  ui->HamegI2Control->setEnabled(true);
	  ui->HamegU2Write->setEnabled(true);
	  ui->HamegI2Write->setEnabled(true);
	}
     }

    if (!executionRunning_) {
        bool isCirculatorOn = dataCollector_.circulatorState();
        if (isCirculatorOn) {
            ui->CirculatorOnButton->setEnabled(false);
            ui->CirculatorOffButton->setEnabled(true);
        } else {
            ui->CirculatorOnButton->setEnabled(true);
            ui->CirculatorOffButton->setEnabled(false);
        }
    }

    if (daqRunning_) {
        ui->StartDAQButton->setEnabled(false);
        ui->StopDAQButton->setEnabled(true);
        ui->frequencySlider->setEnabled(false);
        ui->DAQActiveChannelsGrpBox->setEnabled(false);
        ui->logButton->setEnabled(true);
        ui->logEdit->setEnabled(true);
        ui->PlotStartSlider->setEnabled(true);
        ui->PlotStopSlider->setEnabled(true);
        ui->PlotActiveChannelsGrpBox->setEnabled(true);
        ui->PlotChannel0->setEnabled(dataCollector_.activeChannel(0));
        ui->PlotChannel1->setEnabled(dataCollector_.activeChannel(1));
        ui->PlotChannel2->setEnabled(dataCollector_.activeChannel(2));
        ui->PlotChannel3->setEnabled(dataCollector_.activeChannel(3));
        ui->PlotChannel4->setEnabled(dataCollector_.activeChannel(4));
        ui->PlotChannel5->setEnabled(dataCollector_.activeChannel(5));
        ui->PlotChannel6->setEnabled(dataCollector_.activeChannel(6));
        ui->PlotChannel7->setEnabled(dataCollector_.activeChannel(7));
        ui->PlotChannel8->setEnabled(dataCollector_.activeChannel(8));
        ui->PlotChannel9->setEnabled(dataCollector_.activeChannel(9));
        ui->PlotReference->setEnabled(dataCollector_.referenceActive());
    } else {
        ui->StartDAQButton->setEnabled(true);
        ui->StopDAQButton->setEnabled(false);
        ui->frequencySlider->setEnabled(true);
        ui->logButton->setEnabled(false);
        ui->logEdit->setEnabled(false);
        ui->DAQActiveChannelsGrpBox->setEnabled(true);
        ui->PlotStartSlider->setEnabled(false);
        ui->PlotStopSlider->setEnabled(false);
        ui->PlotActiveChannelsGrpBox->setEnabled(false);
    }

    for (unsigned int i=0;i<10;i++) {
        tempDisplay_[i]->display("-");
    }
    ui->RefDisplay->display("-");

    if (executionRunning_) {
        ui->BathTempSet->setEnabled(false);
        ui->WriteBathTempButton->setEnabled(false);
        ui->PIDXpSpinBox->setEnabled(false);
        ui->PIDTvSpinBox->setEnabled(false);
        ui->PIDTnSpinBox->setEnabled(false);
        ui->WritePIDButton->setEnabled(false);
        ui->LoadPIDButton->setEnabled(false);
        ui->SavePIDButton->setEnabled(false);
        ui->CirculatorOnButton->setEnabled(false);
        ui->CirculatorOffButton->setEnabled(false);
        ui->LoadCommandQueue->setEnabled(false);
        ui->SaveCommandQueue->setEnabled(false);
        ui->ExecuteCommandQueue->setEnabled(false);
        ui->StopExecution->setEnabled(true);
    } else {
        ui->BathTempSet->setEnabled(true);
        ui->WriteBathTempButton->setEnabled(true);
        ui->PIDXpSpinBox->setEnabled(true);
        ui->PIDTvSpinBox->setEnabled(true);
        ui->PIDTnSpinBox->setEnabled(true);
        ui->WritePIDButton->setEnabled(true);
        ui->LoadPIDButton->setEnabled(true);
        ui->SavePIDButton->setEnabled(true);
        ui->LoadCommandQueue->setEnabled(true);
        ui->SaveCommandQueue->setEnabled(true);
        ui->ExecuteCommandQueue->setEnabled(true);
        ui->StopExecution->setEnabled(false);
    }
}

void LabControl::initPlots()
{
    static QColor colors[] = {
        QColor(Qt::red), // 0
        QColor(Qt::green), // 1
        QColor(Qt::blue), // 2
        QColor(Qt::cyan), // 3
        QColor(Qt::magenta), // 4
        QColor(Qt::yellow), // 5
        QColor(Qt::darkCyan), // 6
        QColor(Qt::darkMagenta), // 7
        QColor(Qt::darkYellow), // 8
        QColor(Qt::darkGray)  // 9
    };

    static QSize symbolSize(7,7);
    static QwtSymbol symbols[] = {
        QwtSymbol(QwtSymbol::XCross, colors[0], colors[0], symbolSize), // 0
        QwtSymbol(QwtSymbol::XCross, colors[1], colors[1], symbolSize), // 1
        QwtSymbol(QwtSymbol::XCross, colors[2], colors[2], symbolSize), // 2
        QwtSymbol(QwtSymbol::XCross, colors[3], colors[3], symbolSize), // 3
        QwtSymbol(QwtSymbol::XCross, colors[4], colors[4], symbolSize), // 4
        QwtSymbol(QwtSymbol::XCross, colors[5], colors[5], symbolSize), // 5
        QwtSymbol(QwtSymbol::XCross, colors[6], colors[6], symbolSize), // 6
        QwtSymbol(QwtSymbol::XCross, colors[7], colors[7], symbolSize), // 7
        QwtSymbol(QwtSymbol::XCross, colors[8], colors[8], symbolSize), // 8
        QwtSymbol(QwtSymbol::XCross, colors[9], colors[9], symbolSize), // 9
    };

    LabControlPlot * plot;

    std::vector<unsigned int> channelsToPlot;
    if (ui->PlotChannel0->isChecked()) channelsToPlot.push_back(0);
    if (ui->PlotChannel1->isChecked()) channelsToPlot.push_back(1);
    if (ui->PlotChannel2->isChecked()) channelsToPlot.push_back(2);
    if (ui->PlotChannel3->isChecked()) channelsToPlot.push_back(3);
    if (ui->PlotChannel4->isChecked()) channelsToPlot.push_back(4);
    if (ui->PlotChannel5->isChecked()) channelsToPlot.push_back(5);
    if (ui->PlotChannel6->isChecked()) channelsToPlot.push_back(6);
    if (ui->PlotChannel7->isChecked()) channelsToPlot.push_back(7);
    if (ui->PlotChannel8->isChecked()) channelsToPlot.push_back(8);
    if (ui->PlotChannel9->isChecked()) channelsToPlot.push_back(9);

    for (std::vector<unsigned int>::const_iterator it = channelsToPlot.begin();
         it != channelsToPlot.end();
         ++it) {
        QString title = "ch ";
        title += QString::number(*it);
        plot = new LabControlPlot(title, colors[*it]);
        plot->setSymbol(symbols[*it]);
        plot->setStyle(QwtPlotCurve::Lines);
        plot->getChannelDataVector_ = &LabControlDataCollector::temperatureVector;
        plot->channel_ = *it;
        plot->attach(ui->DisplayTempVsTime);
        plots_.push_back(plot);
    }

    if (ui->PlotReference->isChecked()) {
        plot = new LabControlPlot("Ref", QColor(Qt::white));
        plot->setStyle(QwtPlotCurve::Lines);
        plot->setSymbol(QwtSymbol(QwtSymbol::XCross, QColor(Qt::white), QColor(Qt::white), symbolSize));
        plot->getDataVector_ = &LabControlDataCollector::referenceTemperatureVector;
        plot->attach(ui->DisplayTempVsTime);
        plots_.push_back(plot);
    }

    if (ui->PlotBath->isChecked()) {
        plot = new LabControlPlot("Bath", QColor(Qt::darkGreen));
        plot->setStyle(QwtPlotCurve::Lines);
        plot->setSymbol(QwtSymbol(QwtSymbol::Rect, QColor(Qt::darkGreen), QColor(Qt::darkGreen), symbolSize));
        plot->getDataVector_ = &LabControlDataCollector::bathTemperatureVector;
        plot->attach(ui->DisplayTempVsTime);
        plots_.push_back(plot);
    }

    if (ui->PlotSafetySensor->isChecked()) {
        plot = new LabControlPlot("Safety", QColor(Qt::darkBlue));
        plot->setStyle(QwtPlotCurve::Lines);
        plot->setSymbol(QwtSymbol(QwtSymbol::Rect, QColor(Qt::darkBlue), QColor(Qt::darkBlue), symbolSize));
        plot->getDataVector_ = &LabControlDataCollector::safetySensorTemperatureVector;
        plot->attach(ui->DisplayTempVsTime);
        plots_.push_back(plot);
    }

    if (ui->PlotHeatingPower->isChecked()) {
        plot = new LabControlPlot("Power", QColor(Qt::darkRed));
        plot->setStyle(QwtPlotCurve::Lines);
        plot->setSymbol(QwtSymbol(QwtSymbol::Rect, QColor(Qt::darkRed), QColor(Qt::darkRed), symbolSize));
        plot->getDataVector_ = &LabControlDataCollector::heatingPowerVector;
        plot->attach(ui->DisplayTempVsTime);
        plots_.push_back(plot);
    }

    ui->DisplayTempVsTime->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
}

void LabControl::clearPlots()
{
    LabControlPlot * plot;
    for (std::vector<LabControlPlot*>::iterator it = plots_.begin();
         it != plots_.end();
         ++it) {
         plot = *it;
         plot->detach();
         delete plot;
    }
    plots_.clear();
}

void LabControl::updatePlots()
{
    unsigned int startTime = ui->PlotStartSlider->value();
    unsigned int stopTime = currentStopTime_;

    QwtArray<double> daqTimeVec;
    dataCollector_.daqTimeVector(startTime, stopTime, daqTimeVec);

    LabControlPlot * plot;
    for (std::vector<LabControlPlot*>::iterator it = plots_.begin();
         it != plots_.end();
         ++it) {

         plot = *it;
         void (LabControlDataCollector::*getDataVector)(unsigned int, unsigned int,
                                                        QwtArray<double> &) = plot->getDataVector_;
         void (LabControlDataCollector::*getChannelDataVector)(unsigned int,
                                                               unsigned int, unsigned int,
                                                               QwtArray<double> &) = plot->getChannelDataVector_;
         QwtArray<double> dataVec;
         if (plot->getDataVector_)
             (dataCollector_.*getDataVector)(startTime, stopTime, dataVec);
         if (plot->getChannelDataVector_)
             (dataCollector_.*getChannelDataVector)(plot->channel_, startTime, stopTime, dataVec);

         plot->setData(daqTimeVec, dataVec);
    }

    ui->DisplayTempVsTime->replot();
}

void LabControl::readPreferences()
{
    preferences_.read();
}

void LabControl::applyPreferences()
{
    // ui->PIDXpSpinBox->setValue(preferences_.pidPx());
    // ui->PIDTnSpinBox->setValue(preferences_.pidTn());
    // ui->PIDTvSpinBox->setValue(preferences_.pidTv());
    // on_WritePIDButton_clicked();

    for (unsigned int i=0;i<10;i++) {
        dataCollector_.calibration().setCalibration(i, preferences_.calibration(i));
    }
}

void LabControl::on_PlotStartSlider_valueChanged(int value)
{
    QString label = QString::number(value);
    label += " s";
    ui->PlotStartLabel->setText(label);

    updatePlots();
}

void LabControl::on_PlotStopSlider_valueChanged(int value)
{
    QString label = QString::number(daqTime_-value);
    label += " s";
    ui->PlotStopLabel->setText(label);
    currentStopTime_ = daqTime_-value;

    updatePlots();
}

void LabControl::on_StartDAQButton_clicked()
{
    if (daqRunning_) return;
    daqRunning_ = true;
    refreshTimer_->stop();

    clearPlots();
    ui->DisplayTempVsTime->replot();

    dataCollector_.setActiveChannel(0, ui->DAQChannel0->isChecked());
    dataCollector_.setActiveChannel(1, ui->DAQChannel1->isChecked());
    dataCollector_.setActiveChannel(2, ui->DAQChannel2->isChecked());
    dataCollector_.setActiveChannel(3, ui->DAQChannel3->isChecked());
    dataCollector_.setActiveChannel(4, ui->DAQChannel4->isChecked());
    dataCollector_.setActiveChannel(5, ui->DAQChannel5->isChecked());
    dataCollector_.setActiveChannel(6, ui->DAQChannel6->isChecked());
    dataCollector_.setActiveChannel(7, ui->DAQChannel7->isChecked());
    dataCollector_.setActiveChannel(8, ui->DAQChannel8->isChecked());
    dataCollector_.setActiveChannel(9, ui->DAQChannel9->isChecked());
    dataCollector_.setReferenceActive(ui->DAQReference->isChecked());

    ui->PlotChannel0->setChecked(0);
    ui->PlotChannel1->setChecked(0);
    ui->PlotChannel2->setChecked(0);
    ui->PlotChannel3->setChecked(0);
    ui->PlotChannel4->setChecked(0);
    ui->PlotChannel5->setChecked(0);
    ui->PlotChannel6->setChecked(0);
    ui->PlotChannel7->setChecked(0);
    ui->PlotChannel8->setChecked(0);
    ui->PlotChannel9->setChecked(0);

    updateControls();

    dataCollector_.clear();

    daqTime_ = 0;
    dataCollector_.setDAQTime(daqTime_);

    QDateTime dt = QDateTime::currentDateTime();
    QString filename = preferences_.dataDirectory().c_str();
    filename += "/LabControl_";
    filename += dt.toString("yyyyMMdd_hhmmss");
    filename += ".dat";
    daqFile_ = new std::ofstream(filename.toStdString().c_str());
    dataCollector_.writeFileHeader(*daqFile_, dt);

    std::cout << " [LabControl::on_StartDAQButton_clicked] -- \n"
            << "  writing to file " << filename.toStdString() << std::endl;

    dataCollector_.startDAQ(daqFile_, ui->frequencySlider->value());

    dataCollector_.writeStatus(*daqFile_);
    dataCollector_.writeHameg(*daqFile_);

    initPlots();

    collectData();

    daqTimer_->start(1000*daqUpdateTime);
}

void LabControl::on_StopDAQButton_clicked()
{
    if (!daqRunning_) return;
    daqRunning_ = false;
    daqTimer_->stop();

    dataCollector_.stopDAQ();

    delete daqFile_;

    updateControls();

    refreshTimer_->start(1000*updateTime);
}

void LabControl::on_WriteBathTempButton_clicked()
{
    float temperature = ui->BathTempSet->value();
    julabo_->SetWorkingTemperature(temperature);

    bool needWrite = false;
    if (daqRunning_ && temperature!=dataCollector_.workPoint()) {
        needWrite = true;
    }

    dataCollector_.setWorkPoint(temperature);

    if (needWrite) dataCollector_.writeStatus(*daqFile_);
}

void LabControl::on_ReadbathTempButton_clicked()
{
    float temperature = julabo_->GetWorkingTemperature();
    ui->BathTempSet->setValue(temperature);

    dataCollector_.setWorkPoint(temperature);
}

void LabControl::on_WritePIDButton_clicked()
{
    float Xp = ui->PIDXpSpinBox->value();
    int Tn = ui->PIDTnSpinBox->value();
    int Tv = ui->PIDTvSpinBox->value();

    julabo_->SetControlParameters(Xp, Tn, Tv);

    bool needWrite = false;
    if (daqRunning_ &&
        (Xp!=dataCollector_.pidXp() ||
         Tn!=dataCollector_.pidTn() ||
         Tv!=dataCollector_.pidTv())) {
        needWrite = true;
    }

    dataCollector_.setPidXp(Xp);
    dataCollector_.setPidTn(Tn);
    dataCollector_.setPidTv(Tv);

    if (needWrite) dataCollector_.writeStatus(*daqFile_);

    on_ReadPIDButton_clicked();
}

void LabControl::on_ReadPIDButton_clicked()
{
    float Xp = julabo_->GetProportionalParameter();
    int Tn = julabo_->GetIntegralParameter();
    int Tv = julabo_->GetDifferentialParameter();

    ui->PIDXpSpinBox->setValue(Xp);
    ui->PIDTnSpinBox->setValue(Tn);
    ui->PIDTvSpinBox->setValue(Tv);

    dataCollector_.setPidXp(Xp);
    dataCollector_.setPidTn(Tn);
    dataCollector_.setPidTv(Tv);
}

void LabControl::on_LoadPIDButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Load PID parameters",
                                                    "~/software/LabControl/config",
                                                    "Julabo PID (*.pid);;All Files (*)");
    if (fileName.isEmpty()) return;

    julabo_->LoadControlParametersAndApply(fileName.toStdString());

    float Xp = julabo_->GetProportionalParameter();
    int Tn = julabo_->GetIntegralParameter();
    int Tv = julabo_->GetDifferentialParameter();

    ui->PIDXpSpinBox->setValue(Xp);
    ui->PIDTnSpinBox->setValue(Tn);
    ui->PIDTvSpinBox->setValue(Tv);

    bool needWrite = false;
    if (daqRunning_ &&
        (Xp!=dataCollector_.pidXp() ||
         Tn!=dataCollector_.pidTn() ||
         Tv!=dataCollector_.pidTv())) {
        needWrite = true;
    }

    dataCollector_.setPidXp(Xp);
    dataCollector_.setPidTn(Tn);
    dataCollector_.setPidTv(Tv);

    if (needWrite) dataCollector_.writeStatus(*daqFile_);
}

void LabControl::on_SavePIDButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save PID parameters",
                                                    "~/software/LabControl/config",
                                                    "Julabo PID (*.pid);;All Files (*)");
    if (fileName.isEmpty()) return;
    julabo_->SaveControlParameters(fileName.toStdString());
}

void LabControl::on_CirculatorOnButton_clicked()
{
    julabo_->SetCirculatorOn();
}

void LabControl::on_CirculatorOffButton_clicked()
{
    julabo_->SetCirculatorOff();
}

void LabControl::on_ChangePumpPressure_clicked()
{
    julabo_->SetPumpPressure(static_cast<unsigned int>(ui->SetPumpPressure->value()));
}

void LabControl::on_LoadCommandQueue_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Load Command Queue",
                                                    "~/software/LabControl/config",
                                                    "LabControl Command Queue (*.cmd);;All Files (*)");
    if (filename.isEmpty()) return;

    ui->CommandQueueEdit->clear();

    FILE * ifile = fopen(filename.toStdString().c_str(), "r");
    QTextStream is(ifile, QIODevice::ReadOnly);
    QString line;
    while (!is.atEnd()) {
        line = is.readLine();
        ui->CommandQueueEdit->appendPlainText(line);
    }
}

void LabControl::on_SaveCommandQueue_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Save Command Queue",
                                                    "~/software/LabControl/config",
                                                    "LabControl Command Queue (*.cmd);;All Files (*)");
    if (filename.isEmpty()) return;

    FILE * ofile = fopen(filename.toStdString().c_str(), "w");
    QTextStream os(ofile, QIODevice::WriteOnly);
    os << ui->CommandQueueEdit->toPlainText();
}

void LabControl::on_ExecuteCommandQueue_clicked()
{
    executionRunning_ = true;
    executionTime_ = 0;
    updateControls();

    commands_.clear();

    unsigned int executionTime = 0;

    QString cmdText = ui->CommandQueueEdit->toPlainText();
    QStringList cmdList = cmdText.split('\n');
    for (int i=0;i<cmdList.size();++i) {
        QStringList line = cmdList.at(i).split(QRegExp("\\s+"));
        if (line.size()<2) continue;

        bool ok;
        QString timeString = line.at(0);
        if (timeString.startsWith("@")) {
            timeString.remove("@");
            executionTime = 0;
        }
        int time;
        if (timeString.endsWith("s")) {
            timeString.remove("s");
            time = static_cast<int>(timeString.toFloat(&ok));
        } else if (timeString.endsWith("m")) {
            timeString.remove("m");
            time = static_cast<int>(60. * timeString.toFloat(&ok));
        } else if (timeString.endsWith("h")) {
            timeString.remove("h");
            time = static_cast<int>(60. * 60. * timeString.toFloat(&ok));
        } else {
            time = static_cast<int>(timeString.toFloat(&ok));
        }
        if (!ok) continue;

        executionTime += time;

        QString c = line.at(1);
        float parameter = 0.0;
        if (line.size()==3) {
            parameter = line.at(2).toFloat(&ok);
            if (!ok) parameter = 0.0;
        }

        if (c=="setTemp") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdSetTemp, parameter));
        } else if (c=="setPIDP") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdSetPIDP, parameter));
        } else if (c=="setPIDTn") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdSetPIDTn, parameter));
        } else if (c=="setPIDTv") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdSetPIDTv, parameter));
        } else if (c=="circulatorON") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdCirculatorON, parameter));
        } else if (c=="circulatorOFF") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdCirculatorOFF, parameter));
        } else if (c=="startDAQ") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdStartDAQ, parameter));
        } else if (c=="stopDAQ") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdStopDAQ, parameter));
        } else if (c=="isBathTempBelow") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::isBathTempBelow, parameter));
        } else if (c=="isBathTempAbove") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::isBathTempAbove, parameter));
        } else if (c=="setU1") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdSetU1, parameter));
        } else if (c=="setI1") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdSetI1, parameter));
        } else if (c=="setU2") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdSetU2, parameter));
        } else if (c=="setI2") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdSetI2, parameter));
        } else if (c=="voltageON") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdVoltageON, parameter));
        } else if (c=="voltageOFF") {
            commands_.push_back(LabControlCommand(executionTime, LabControlCommand::cmdVoltageOFF, parameter));
        } else {
            std::cout << " [LabControl::on_ExecuteCommandQueue_clicked] -- \n"
                << "  command " << c.toStdString() << " not known" << std::endl;
            continue;
        }

        std::cout << " [LabControl::on_ExecuteCommandQueue_clicked] -- \n"
            << "  added command " << c.toStdString() << " at " << executionTime << " seconds" << std::endl;
    }

    std::sort(commands_.begin(), commands_.end());
}

void LabControl::on_StopExecution_clicked()
{
    commands_.clear();
    executionRunning_ = false;
    ui->ExecutionTimeLabel->setText("");
    updateControls();
}

void LabControl::executeCommand()
{
    if (!executionRunning_) return;

    std::cout << " [LabControl::executeCommand] -- " << std::endl;

    if (commands_.empty()) {
        executionRunning_ = false;
        updateControls();
        ui->ExecutionTimeLabel->setText("");
        return;
    }

    if (daqRunning_) {
        executionTime_ += daqUpdateTime;
    } else {
        executionTime_ += updateTime;
    }

    unsigned int cmdTime = commands_.front().time_;
    if (cmdTime<=executionTime_) {
        int cmdType = commands_.front().type_;
        float cmdParameter = commands_.front().parameter_;

        std::cout << " [LabControl::executeCommand] -- \n"
            << "  executing " << cmdType << "  " << cmdParameter << std::endl;

        if (cmdType==LabControlCommand::cmdSetTemp) {
            ui->BathTempSet->setValue(cmdParameter);
            on_WriteBathTempButton_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdSetPIDP) {
            ui->PIDXpSpinBox->setValue(cmdParameter);
            on_WritePIDButton_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdSetPIDTn) {
            ui->PIDTnSpinBox->setValue(static_cast<int>(cmdParameter));
            on_WritePIDButton_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdSetPIDTv) {
            ui->PIDTvSpinBox->setValue(static_cast<int>(cmdParameter));
            on_WritePIDButton_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdCirculatorON) {
            on_CirculatorOnButton_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdCirculatorOFF) {
            on_CirculatorOffButton_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdStartDAQ) {
            on_StartDAQButton_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdStopDAQ) {
            on_StopDAQButton_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::isBathTempBelow) {
            if (dataCollector_.data().bathTemperature() < cmdParameter) {
                commands_.pop_front();
                // Update times of rest of command queue
                for (std::deque<LabControlCommand>::iterator it = commands_.begin();
                     it != commands_.end();
                     it++) {
                    unsigned int dt = (*it).time_ - cmdTime;
                    (*it).time_ = executionTime_ + dt;
                }
            }
        }
        if (cmdType==LabControlCommand::isBathTempAbove) {
            if (dataCollector_.data().bathTemperature() > cmdParameter) {
                commands_.pop_front();
                // Update times of rest of command queue
                for (std::deque<LabControlCommand>::iterator it = commands_.begin();
                     it != commands_.end();
                     it++) {
                    unsigned int dt = (*it).time_ - cmdTime;
                    (*it).time_ = executionTime_ + dt;
                }
            }
        }
        if (cmdType==LabControlCommand::cmdSetU1) {
	    ui->HamegU1Control->setValue(cmdParameter);
	    on_HamegU1Write_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdSetI1) {
	    ui->HamegI1Control->setValue(cmdParameter);
	    on_HamegI1Write_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdSetU2) {
	    ui->HamegU2Control->setValue(cmdParameter);
	    on_HamegU2Write_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdSetI2) {
	    ui->HamegI2Control->setValue(cmdParameter);
	    on_HamegI2Write_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdVoltageON) {
  	    on_HamegEnable_clicked();
            commands_.pop_front();
        }
        if (cmdType==LabControlCommand::cmdVoltageOFF) {
  	    on_HamegDisable_clicked();
            commands_.pop_front();
        }
    }

    QString label("Execution-Time: ");
    label += QString::number(executionTime_);
    label += " s";
    ui->ExecutionTimeLabel->setText(label);
}

void LabControl::on_PlotChannel0_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotChannel1_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotChannel2_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotChannel3_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotChannel4_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotChannel5_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotChannel6_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotChannel7_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotChannel8_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotChannel9_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotReference_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotBath_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotSafetySensor_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_PlotHeatingPower_stateChanged(int )
{
    clearPlots();
    initPlots();
    updatePlots();
}

void LabControl::on_logButton_clicked()
{
    std::cout << " [LabControl::on_logButton_clicked] -- new log entry\n"
              << ui->logEdit->toPlainText().toStdString()
              << std::endl;

    int daqTime = daqTime_-daqUpdateTime;
    QStringList sl = ui->logEdit->toPlainText().split('\n', QString::SkipEmptyParts);
    for (QStringList::iterator it = sl.begin();
         it!=sl.end();
         ++it) {
        *daqFile_ << "<LOG>  ";
        *daqFile_ << std::setw(12) << std::right << daqTime << " ";
        *daqFile_ <<(*it).toStdString() << std::endl;
    }

    ui->logEdit->setPlainText("");
}

void LabControl::on_frequencySlider_valueChanged(int value)
{
    int v = (value/10)*10;
    ui->frequencySlider->setValue(v);
}

void LabControl::on_HamegEnable_clicked()
{
    hameg8143_->SwitchOutputOn();
}

void LabControl::on_HamegDisable_clicked()
{
    hameg8143_->SwitchOutputOff();
}

void LabControl::on_HamegRemoteModeCheck_clicked()
{
    hameg8143_->SetRemoteMode(ui->HamegRemoteModeCheck->isChecked());
    ui->HamegMixedModeCheck->setChecked(false);
    if (!ui->HamegRemoteModeCheck->isChecked()) {
        ui->HamegEnable->setEnabled(false);
        ui->HamegDisable->setEnabled(false);

        ui->HamegU1Control->setEnabled(false);
        ui->HamegI1Control->setEnabled(false);
        ui->HamegU1Write->setEnabled(false);
        ui->HamegI1Write->setEnabled(false);

        ui->HamegU2Control->setEnabled(false);
        ui->HamegI2Control->setEnabled(false);
        ui->HamegU2Write->setEnabled(false);
        ui->HamegI2Write->setEnabled(false);
    }
}

void LabControl::on_HamegMixedModeCheck_clicked()
{
    hameg8143_->SetMixedMode(ui->HamegMixedModeCheck->isChecked());
}

void LabControl::on_HamegU1Write_clicked()
{
    hameg8143_->SetVoltage(1, ui->HamegU1Control->value());
}

void LabControl::on_HamegI1Write_clicked()
{
    hameg8143_->SetCurrent(1, ui->HamegI1Control->value());
}

void LabControl::on_HamegU2Write_clicked()
{
    hameg8143_->SetVoltage(2, ui->HamegU2Control->value());
}

void LabControl::on_HamegI2Write_clicked()
{
    hameg8143_->SetCurrent(2, ui->HamegI2Control->value());
}
