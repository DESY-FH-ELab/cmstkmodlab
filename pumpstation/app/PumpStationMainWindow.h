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

#ifndef PUMPSTATIONMAINWINDOW_H
#define PUMPSTATIONMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QWebView>
#else
#include <QWebEngineView>
#endif

class PumpStationMainWindow : public QMainWindow
{
  Q_OBJECT

public:

  explicit PumpStationMainWindow(double updateInterval,
                                 QWidget *parent = 0);

public slots:

  void quit();

signals:

protected slots:


protected:

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QWebView* view_;
#else
  QWebEngineView* view_;
#endif

  /// Time interval between cache refreshes; in seconds.
  const double updateInterval_;
  QTimer* timer_;
};

#endif // PUMPSTATIONMAINWINDOW_H
