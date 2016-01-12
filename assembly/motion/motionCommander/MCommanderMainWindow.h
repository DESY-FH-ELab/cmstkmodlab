#ifndef MCOMMANDERMAINWINDOW_H
#define MCOMMANDERMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>

#include "LStepExpressModel.h"
#include "LStepExpressMotionManager.h"
#include "LStepExpressMotionThread.h"

#include "LStepExpressWidget.h"
#include "LStepExpressJoystickWidget.h"

class MCommanderMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MCommanderMainWindow(QWidget *parent = 0);

public slots:

  void quit();

signals:

protected slots:

protected:

    LStepExpressModel* lStepExpressModel_;
    LStepExpressMotionManager* motionManager_;
    LStepExpressMotionThread* motionThread_;

    QTabWidget* tabWidget_;
};

#endif // MCOMMANDERMAINWINDOW_H
