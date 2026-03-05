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

#ifndef PLASMAMAINWINDOW_H
#define PLASMAMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>

#include <NanotecSMCI36Model.h>
#include <NanotecSMCI36Widget.h>

#include <NanotecSMCI36LinearStageModel.h>
#include <PlasmaStageWidget.h>

class PlasmaMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit PlasmaMainWindow(QWidget *parent = 0);

public slots:

  void quit();

signals:

protected slots:

protected:

  QDir currentDir_;

  NanotecSMCI36Model *smci36ModelX_;
  NanotecSMCI36LinearStageModel *stageX_;

  QToolBar* toolBar_;

  QTabWidget* tabWidget_;

  NanotecSMCI36Widget *smci36WidgetX_;
  PlasmaStageWidget *stageWidgetX_;
};

#endif // PLASMAMAINWINDOW_H
