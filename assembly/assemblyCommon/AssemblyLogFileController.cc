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

#include <AssemblyLogFileController.h>

#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QScrollBar>

AssemblyLogFileController::AssemblyLogFileController(const QString& file_path, QObject* parent)
 : QObject(parent)

 , file_path_(file_path)
 , last_read_endline_(0)
 , reading_(false)

 , update_timer_(nullptr)
{
  // timer
  update_timer_ = new QTimer;
  update_timer_->setInterval(500);

  connect(update_timer_, SIGNAL(timeout()), this, SLOT(update()));

  update_timer_->start();
};

AssemblyLogFileController::~AssemblyLogFileController()
{
  update_timer_->stop();

  delete update_timer_;

  update_timer_ = nullptr;
}

void AssemblyLogFileController::update()
{
  if(reading_ == false)
  {
    reading_ = true;

    QFile file(file_path_);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text) == false){ return; }

    QTextStream in(&file);

    uint line_number(0);

    QStringList qstr_list;

    while(in.atEnd() == false)
    {
      const QString line = in.readLine();

      ++line_number;

      if(line_number > last_read_endline_)
      {
        qstr_list.append(line);
      }
    }

    NQLog("AssemblyLogFileController", NQLog::Debug) << "update"
       << ": emitting signal \"new_lines(QStringList)\"";

    emit new_lines(qstr_list);

    last_read_endline_ = line_number;

    file.close();

//    // Scrolls to the bottom
//    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());

    reading_ = false;
  }
}
