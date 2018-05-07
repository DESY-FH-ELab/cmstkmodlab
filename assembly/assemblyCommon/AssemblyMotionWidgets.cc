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

#include <AssemblyMotionWidgets.h>
#include <nqlogger.h>
#include <Util.h>

#include <QStringList>

AssemblyMoveWidget::AssemblyMoveWidget(const QString& label, const QString& default_entry, const bool move_relative, QWidget* parent) :
  QWidget(parent),
  moveRelative_(move_relative)
{
  layout_ = new QFormLayout;
  this->setLayout(layout_);

  button_ = new QPushButton(label, this);
  button_->setStyleSheet(
    "Text-align: left;"
    "padding-left:   4px;"
    "padding-right:  4px;"
    "padding-top:    3px;"
    "padding-bottom: 3px;"
  );

  lineed_ = new QLineEdit(this);
  lineed_->setText(default_entry);

  layout_->addRow(button_, lineed_);

  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
}

QString AssemblyMoveWidget::get_input_string() const
{
  return this->lineed_->text();
}

void AssemblyMoveWidget::enable(const bool b)
{
  button_->setEnabled(b);
  lineed_->setEnabled(b);

  return;
}

void AssemblyMoveWidget::execute()
{
  QString line_entry = this->get_input_string();

  // parse lineEdit text to get target coordinates
  const QStringList entries = line_entry.remove(" ").split(",");

  if((entries.length() == 4) || (moveRelative_ && (entries.length() == 3)))
  {
    bool valid_x_d(false);
    bool valid_y_d(false);
    bool valid_z_d(false);
    bool valid_a_d(true);

    const double x_d = entries.value(0).toDouble(&valid_x_d);
    const double y_d = entries.value(1).toDouble(&valid_y_d);
    const double z_d = entries.value(2).toDouble(&valid_z_d);

    const double a_d = (entries.length() == 4) ? entries.value(3).toDouble(&valid_a_d) : 0.0;

    if(!valid_x_d)
    {
      NQLog("AssemblyMoveWidget", NQLog::Warning) << "execute"
         << ": invalid format for x-axis value (" << entries.value(0) << "), no action taken";

      return;
    }
    else if(!valid_y_d)
    {
      NQLog("AssemblyMoveWidget", NQLog::Warning) << "execute"
         << ": invalid format for y-axis value (" << entries.value(1) << "), no action taken";

      return;
    }
    else if(!valid_z_d)
    {
      NQLog("AssemblyMoveWidget", NQLog::Warning) << "execute"
         << ": invalid format for z-axis value (" << entries.value(2) << "), no action taken";

      return;
    }
    else if(!valid_a_d)
    {
      NQLog("AssemblyMoveWidget", NQLog::Warning) << "execute"
         << ": invalid format for a-axis value (" << entries.value(3) << "), no action taken";

      return;
    }
    // ---

    if(moveRelative_)
    {
      NQLog("AssemblyMoveWidget", NQLog::Message) << "execute"
         << ": emitting signal \"moveRelative("
         << x_d << ", " << y_d << ", " << z_d << ", " << a_d << ")\"";

      this->enable(false);

      emit moveRelative(x_d, y_d, z_d, a_d);
    }
    else
    {
      NQLog("AssemblyMoveWidget", NQLog::Message) << "execute"
         << ": emitting signal \"moveAbsolute("
         << x_d << ", " << y_d << ", " << z_d << ", " << a_d << ")\"";

      this->enable(false);

      emit moveAbsolute(x_d, y_d, z_d, a_d);
    }
  }
  else
  {
    NQLog("AssemblyMoveWidget", NQLog::Warning) << "execute"
       << ": [moveRelative=" << moveRelative_ << "] invalid input string format (" << line_entry << "), no action taken";

    return;
  }

  return;
}
// ===========================================================================
