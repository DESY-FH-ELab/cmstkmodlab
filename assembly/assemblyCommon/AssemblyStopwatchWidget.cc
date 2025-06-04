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

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <AssemblyStopwatchWidget.h>
#include <AssemblyUtilities.h>

#include <QTime>

AssemblyStopwatchWidget::AssemblyStopwatchWidget(QWidget* parent) : QWidget(parent)
{
  layout_ = new QHBoxLayout(this);
  this->setLayout(layout_);

  elapsed_timer_ = new QElapsedTimer();

  update_timer_ = new QTimer();

  button_layout_ = new QVBoxLayout();

  start_ = new QPushButton("Start", this);
  stop_ = new QPushButton("Stop", this);
  reset_ = new QPushButton("Reset", this);

  button_layout_->addWidget(start_);
  button_layout_->addWidget(stop_);
  button_layout_->addWidget(reset_);

  layout_->addLayout(button_layout_);

  connect(start_, SIGNAL(clicked()), this, SLOT(startStopwatch()));
  connect(stop_, SIGNAL(clicked()), this, SLOT(stopStopwatch()));
  connect(reset_, SIGNAL(clicked()), this, SLOT(resetStopwatch()));

  QTime time_elapsed = QTime(0, 0, 0, elapsed_timer_->elapsed());
  elapsedTimeLabel_ = new QLabel(time_elapsed.toString("mm:ss"));
  layout_->addWidget(elapsedTimeLabel_);

  connect(update_timer_, &QTimer::timeout, this, &AssemblyStopwatchWidget::updateStopwatch);

  NQLog("AssemblyStopwatchWidget", NQLog::Message) << "constructed";
}

void AssemblyStopwatchWidget::startStopwatch(){
  NQLog("AssemblyStopwatchWidget", NQLog::Message) << ": Starting stopwatch";
  elapsed_timer_->start();
  update_timer_->start(500);
}

void AssemblyStopwatchWidget::stopStopwatch(){
  NQLog("AssemblyStopwatchWidget", NQLog::Message) << ": Stopping stopwatch";

  elapsed_timer_->invalidate();
  update_timer_->stop();
}

void AssemblyStopwatchWidget::resetStopwatch(){
  NQLog("AssemblyStopwatchWidget", NQLog::Message) << ": Resetting stopwatch";
}

void AssemblyStopwatchWidget::updateStopwatch(){
  NQLog("AssemblyStopwatchWidget", NQLog::Message) << ": Updating stopwatch";

  QTime time_elapsed = QTime(0, 0, 0, elapsed_timer_->elapsed());
  elapsedTimeLabel_ = new QLabel(time_elapsed.toString("mm:ss"));
  NQLog("AssemblyStopwatchWidget", NQLog::Message) << ": Time: " << time_elapsed.toString("mm:ss");
}
