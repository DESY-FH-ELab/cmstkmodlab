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

#ifndef ASSEMBLYSTOPWATCHWIDGET_H
#define ASSEMBLYSTOPWATCHWIDGET_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QElapsedTimer>

#include <RelayCardManager.h>

#include <map>

class AssemblyStopwatchWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyStopwatchWidget(QWidget* parent=nullptr);
  virtual ~AssemblyStopwatchWidget() {}

 protected:

  QVBoxLayout* button_layout_;
  QHBoxLayout* layout_;
  QPushButton* start_;
  QPushButton* stop_;
  QPushButton* reset_;

  double seconds_;

  QTimer* update_timer_;
  QElapsedTimer* elapsed_timer_;

  QLabel* elapsedTimeLabel_;

 public slots:

  void startStopwatch();
  void stopStopwatch();
  void resetStopwatch();

  void updateStopwatch();

};

#endif // ASSEMBLYSTOPWATCHWIDGET_H
