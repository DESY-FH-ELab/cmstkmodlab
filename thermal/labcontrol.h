#ifndef LABCONTROL_H
#define LABCONTROL_H

#include <deque>
#include <vector>
#include <fstream>

#include <QMainWindow>
#include <QTimer>
#include <QLCDNumber>

#include "devices/Julabo/VJulaboFP50.h"
#include "devices/Hameg/VHameg8143.h"

#include "labcontrolpreferences.h"
#include "labcontroldatacollector.h"
#include "labcontrolcommand.h"
#include "labcontrolplot.h"

namespace Ui {
    class LabControl;
}

class LabControl : public QMainWindow
{
    Q_OBJECT

public:

    explicit LabControl(QWidget *parent = 0);
    ~LabControl();

private:

    Ui::LabControl *ui;

    LabControlPreferences preferences_;

    QTimer *refreshTimer_;

    QTimer *daqTimer_;
    unsigned int daqTime_;
    unsigned int currentStopTime_;
    bool daqRunning_;
    std::ofstream *daqFile_;

    bool executionRunning_;
    unsigned int executionTime_;

    void updateControls();
    void initPlots();
    void clearPlots();
    void updatePlots();
    void readPreferences();
    void applyPreferences();
    void executeCommand();

    LabControlDataCollector dataCollector_;

    QLCDNumber* tempDisplay_[10];

    std::deque<LabControlCommand> commands_;

    std::vector<LabControlPlot*> plots_;

    VJulaboFP50* julabo_;
    VHameg8143* hameg8143_;

private slots:

    void on_HamegI2Write_clicked();
    void on_HamegU2Write_clicked();
    void on_HamegI1Write_clicked();
    void on_HamegU1Write_clicked();
    void on_HamegMixedModeCheck_clicked();
    void on_HamegRemoteModeCheck_clicked();
    void on_HamegDisable_clicked();
    void on_HamegEnable_clicked();
    void on_frequencySlider_valueChanged(int value);
    void on_logButton_clicked();
    void on_PlotReference_stateChanged(int );
    void on_PlotChannel9_stateChanged(int );
    void on_PlotChannel8_stateChanged(int );
    void on_PlotChannel7_stateChanged(int );
    void on_PlotChannel6_stateChanged(int );
    void on_PlotChannel5_stateChanged(int );
    void on_PlotChannel4_stateChanged(int );
    void on_PlotChannel3_stateChanged(int );
    void on_PlotChannel2_stateChanged(int );
    void on_PlotChannel1_stateChanged(int );
    void on_PlotChannel0_stateChanged(int );
    void on_PlotSafetySensor_stateChanged(int );
    void on_PlotHeatingPower_stateChanged(int );
    void on_PlotBath_stateChanged(int );
    void refreshDisplays();
    void collectData();

    void on_ChangePumpPressure_clicked();
    void on_CirculatorOffButton_clicked();
    void on_CirculatorOnButton_clicked();
    void on_SaveCommandQueue_clicked();
    void on_LoadCommandQueue_clicked();
    void on_SavePIDButton_clicked();
    void on_LoadPIDButton_clicked();
    void on_ReadPIDButton_clicked();
    void on_WritePIDButton_clicked();
    void on_ReadbathTempButton_clicked();
    void on_StopExecution_clicked();
    void on_ExecuteCommandQueue_clicked();
    void on_WriteBathTempButton_clicked();
    void on_StopDAQButton_clicked();
    void on_StartDAQButton_clicked();
    void on_PlotStopSlider_valueChanged(int value);
    void on_PlotStartSlider_valueChanged(int value);
};

#endif // LABCONTROL_H
