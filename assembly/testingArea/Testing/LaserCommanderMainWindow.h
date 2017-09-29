/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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

