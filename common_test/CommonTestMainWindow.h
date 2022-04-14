/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef COMMONTESTMAINWINDOW_H
#define COMMONTESTMAINWINDOW_H

#include <QMainWindow>

#include "ApplicationConfigViewer.h"

class CommonTestMainWindow : public QMainWindow
{
  Q_OBJECT
public:

  explicit CommonTestMainWindow(QWidget *parent = 0);

public slots:

  void quit();
  void configChanged();

protected:

  ApplicationConfigViewer* viewer_;
};

#endif // COMMONTESTMAINWINDOW_H
