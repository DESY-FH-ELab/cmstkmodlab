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

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <unistd.h>

#include <AssemblyStopwatchWidget.h>
#include <AssemblyUtilities.h>

#include <QFont>
#include <QIcon>
#include <QMediaPlayer>
#include <QFile>

AssemblyStopwatchWidget::AssemblyStopwatchWidget(QWidget* parent) : QWidget(parent)
{
  layout_ = new QHBoxLayout(this);
  this->setLayout(layout_);

  update_timer_ = new QTimer();

  auto config = ApplicationConfig::instance();

  m_sound_1min = QString::fromStdString(Config::CMSTkModLabBasePath + config->getDefaultValue<std::string>("main", "StopwatchSound_1min", "/share/assembly/rooster.mp3"));
  m_sound_20min = QString::fromStdString(Config::CMSTkModLabBasePath + config->getDefaultValue<std::string>("main", "StopwatchSound_20min", "/share/assembly/letsgo.mp3"));
  volume_1min_ = config->getDefaultValue<int>("main", "StopwatchSound_1min_volume", 80);
  volume_20min_ = config->getDefaultValue<int>("main", "StopwatchSound_20min_volume", 100);

  start_ = new QPushButton(QIcon(QString(Config::CMSTkModLabBasePath.c_str())+"/share/common/play.png"), "", this);
  stop_ = new QPushButton(QIcon(QString(Config::CMSTkModLabBasePath.c_str())+"/share/common/stop.png"), "", this);
  reset_ = new QPushButton(QIcon(QString(Config::CMSTkModLabBasePath.c_str())+"/share/common/trash.png"), "", this);

  start_->setFixedSize(32,32);
  stop_->setFixedSize(32,32);
  reset_->setFixedSize(32,32);

  layout_->addWidget(start_);
  layout_->addWidget(stop_);
  layout_->addWidget(reset_);

  start_->setVisible(true);
  stop_->setVisible(false);
  reset_->setVisible(false);

  connect(start_, SIGNAL(clicked()), this, SLOT(startStopwatch()));
  connect(stop_, SIGNAL(clicked()), this, SLOT(stopStopwatch()));
  connect(reset_, SIGNAL(clicked()), this, SLOT(resetStopwatch()));

  elapsedTimeLabel_ = new QLabel("00:00");
  auto font = elapsedTimeLabel_->font();
  font.setPointSize(26);
  font.setBold(true);
  elapsedTimeLabel_->setFont(font);

  layout_->addWidget(elapsedTimeLabel_);

  connect(update_timer_, &QTimer::timeout, this, &AssemblyStopwatchWidget::updateStopwatch);

  NQLog("AssemblyStopwatchWidget", NQLog::Message) << "constructed";
}

void AssemblyStopwatchWidget::startStopwatch(){
  NQLog("AssemblyStopwatchWidget", NQLog::Message) << ": Starting stopwatch";
  reference_time_ = QTime::currentTime();
  update_timer_->start(1000);
  updateStopwatch();

  start_->setVisible(false);
  stop_->setVisible(true);
  reset_->setVisible(false);
}

void AssemblyStopwatchWidget::stopStopwatch(){
  NQLog("AssemblyStopwatchWidget", NQLog::Message) << ": Stopping stopwatch";
  update_timer_->stop();

  start_->setVisible(false);
  stop_->setVisible(false);
  reset_->setVisible(true);
}

void AssemblyStopwatchWidget::resetStopwatch(){
  NQLog("AssemblyStopwatchWidget", NQLog::Message) << ": Resetting stopwatch";
  elapsedTimeLabel_->setText("<font color='black'>00:00</font>");

  start_->setVisible(true);
  stop_->setVisible(false);
  reset_->setVisible(false);
}

void AssemblyStopwatchWidget::updateStopwatch(){
  NQLog("AssemblyStopwatchWidget", NQLog::Debug) << ": Updating stopwatch";

  QTime time_elapsed = QTime(0, 0, 0).addSecs(reference_time_.secsTo(QTime::currentTime()));

  QString color_str = time_elapsed < QTime(0, 1, 0) ? "red" : (time_elapsed < QTime(0, 20, 0) ? "orange" : "darkgreen");

  auto time_str = QString("<font color='") + color_str + "'>" + time_elapsed.toString("mm:ss") + "</font>";
  elapsedTimeLabel_->setText(time_str);

  if(m_previous_time < QTime(0, 1, 0) && time_elapsed >= QTime(0, 1, 0)) {
    NQLog("AssemblyStopwatchWidget", NQLog::Message) << ": Time for mixing is up.";

    auto mediafile = QFile(m_sound_1min);
    if(mediafile.exists()) {
        auto player = new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile(m_sound_1min));
        player->setVolume(volume_1min_);
        player->play();
    }
  } else if(m_previous_time < QTime(0, 20, 0) && time_elapsed >= QTime(0, 20, 0)) {
    NQLog("AssemblyStopwatchWidget", NQLog::Message) << ": Time to continue the assembly.";

    auto mediafile = QFile(m_sound_20min);
    if(mediafile.exists()) {
        auto player = new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile(m_sound_20min));
        player->setVolume(volume_20min_);
        player->play();
    }
  }

  m_previous_time = time_elapsed;

  NQLog("AssemblyStopwatchWidget", NQLog::Debug) << ": Time: " << time_elapsed.toString("mm:ss");
}
