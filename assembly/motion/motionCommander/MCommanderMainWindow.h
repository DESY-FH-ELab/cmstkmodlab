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
#include "LStepExpressMeasurementWidget.h"

class MCommanderMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MCommanderMainWindow(QWidget *parent = 0);

public slots:

    void quit();

signals:

protected slots:

    void testManager();

protected:

    LStepExpressModel* lStepExpressModel_;
    LStepExpressSettings* lStepExpressSettings_;
    LStepExpressMotionManager* motionManager_;
    LStepExpressMotionThread* motionThread_;

    QTabWidget* tabWidget_;

signals:

    void moveAbsolute(double x, double y, double z, double a);
    void moveRelative(double x, double y, double z, double a);
};

#endif // MCOMMANDERMAINWINDOW_H
