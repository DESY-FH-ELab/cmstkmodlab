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

#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>

#include <RohdeSchwarzNGE103BModel.h>
#include <PfeifferModel.h>

class TestWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit TestWindow(QWidget *parent = 0);

public slots:

protected:
  RohdeSchwarzNGE103BModel* nge103BModel_;
};

#endif // TESTWINDOW_H
