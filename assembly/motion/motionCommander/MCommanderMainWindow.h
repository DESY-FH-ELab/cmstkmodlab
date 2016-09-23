#ifndef MCOMMANDERMAINWINDOW_H
#define MCOMMANDERMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>

#include "LStepExpressModel.h"
#include "LStepExpressSettings.h"
#include "LStepExpressMotionManager.h"
#include "LStepExpressMotionThread.h"

#include "LStepExpressSettingsWidget.h"
#include "LStepExpressWidget.h"
#include "LStepExpressJoystickWidget.h"
#include "LStepExpressMeasurementTable.h"
#include "LStepExpressMeasurement.h"
#include "LStepExpressMeasurementWidget.h"
#include "LStepExpressPositionWidget.h"
#include "LStepExpressStatusWindow.h"
#include "LaserControlsWidget.h"

#include "LaserModel.h"
#include "LaserThread.h"

class MCommanderMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MCommanderMainWindow(QWidget *parent = 0);

public slots:

    void quit();

signals:

protected slots:
    /*
    void testManager();
    */

protected:

    LStepExpressModel* lStepExpressModel_;
    LStepExpressSettings* lStepExpressSettings_;
    LStepExpressMotionManager* motionManager_;
    LStepExpressMotionThread* motionThread_;
    LaserModel* laserModel_;
    LaserThread* laserThread_;
    LStepExpressMeasurement* measurement_;
    LStepExpressMeasurementTable* measurementTable_;
    
    QTabWidget* tabWidget_;

signals:
    /*
    void moveAbsolute(double x, double y, double z, double a);
    void moveRelative(double x, double y, double z, double a);
    */
};

#endif // MCOMMANDERMAINWINDOW_H
