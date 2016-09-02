#ifndef LASERCOMMANDERMAINWINDOW_H
#define LASERCOMMANDERMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>
#include <QPushButton>
#include <QLCDNumber>

#include "LaserModel.h"
#include "LaserWidget.h"
#include "LaserThread.h"

class LaserCommanderMainWindow : public QMainWindow
{
    Q_OBJECT
 public:
    explicit LaserCommanderMainWindow(QWidget *parent = 0);
    ~LaserCommanderMainWindow();

    public slots:

        void quit();

 signals:
        void changed(double pvalue);

 protected slots:

     void testManager();
     void updateD();

 protected:

     LaserModel* laserModel_;
     QPushButton* testButton_;
     LaserWidget* laserWidget_;
     LaserThread* laserThread_;
     
 private:
     double value;

};

#endif // LASERCOMMANDERMAINWINDOW_H                                                                                                                                                                         

