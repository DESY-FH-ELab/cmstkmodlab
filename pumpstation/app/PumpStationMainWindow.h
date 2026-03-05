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

#ifndef PUMPSTATIONMAINWINDOW_H
#define PUMPSTATIONMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>

#include "PumpStationHTTPModel.h"
#include "PumpStationSVGWidget.h"

class PumpStationMainWindow : public QMainWindow
{
  Q_OBJECT

public:

  explicit PumpStationMainWindow(PumpStationHTTPModel* model,
                                 QWidget *parent = 0);

public slots:

  void lock();
  void unlock();
  void updateSketch();
  void updateTimestamp();
  void buttonDoubleClicked(int);
  void enableWidgets();

signals:

  void toggleSwitch(int);

protected slots:

protected:

  PumpStationHTTPModel* model_;

  QString sketchSource_;
  PumpStationSVGWidget * sketch_;
  QLabel * timestampLabel_;

  const QString pin_;
  bool locked_;
};

#endif // PUMPSTATIONMAINWINDOW_H
