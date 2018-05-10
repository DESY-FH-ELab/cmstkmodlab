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

#include <AssemblyMotionWidgets.h>
#include <AssemblyUtilities.h>

#include <QStringList>
#include <QLabel>

AssemblyMoveWidget::AssemblyMoveWidget(const LStepExpressMotionManager* const manager, const QString& label, const bool move_relative, QWidget* parent) :
  QWidget(parent),
  manager_(manager),
  move_relative_(move_relative),

  layout_(nullptr),
  button_(nullptr),

  XYZA_lay_(nullptr),

  X_lineed_(nullptr),
  Y_lineed_(nullptr),
  Z_lineed_(nullptr),
  A_lineed_(nullptr)
{
  // motion manager
  if(manager_ == nullptr)
  {
    NQLog("AssemblyMoveWidget", NQLog::Critical) << "initialization error"
      << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  connect(this, SIGNAL(move_absolute(double, double, double, double)), manager_, SLOT(moveAbsolute(double, double, double, double)));
  connect(this, SIGNAL(move_relative(double, double, double, double)), manager_, SLOT(moveRelative(double, double, double, double)));

  connect(manager_, SIGNAL(motion_finished()), this, SLOT(enable()));
  // --------------

  // layout
  layout_ = new QVBoxLayout;
  this->setLayout(layout_);

  button_ = new QPushButton(label);
  button_->setStyleSheet(
    "Text-align: left;"
    "padding-left:   4px;"
    "padding-right:  4px;"
    "padding-top:    3px;"
    "padding-bottom: 3px;"
  );

  layout_->addWidget(button_);

  XYZA_lay_ = new QHBoxLayout;

  QLabel* X_label = new QLabel("X");
  QLabel* Y_label = new QLabel("Y");
  QLabel* Z_label = new QLabel("Z");
  QLabel* A_label = new QLabel("A");

  X_lineed_ = new QLineEdit("");
  Y_lineed_ = new QLineEdit("");
  Z_lineed_ = new QLineEdit("");
  A_lineed_ = new QLineEdit("");

  XYZA_lay_->addWidget(X_label  );
  XYZA_lay_->addWidget(X_lineed_);

  XYZA_lay_->addWidget(Y_label  );
  XYZA_lay_->addWidget(Y_lineed_);

  XYZA_lay_->addWidget(Z_label  );
  XYZA_lay_->addWidget(Z_lineed_);

  XYZA_lay_->addWidget(A_label  );
  XYZA_lay_->addWidget(A_lineed_);

  layout_->addLayout(XYZA_lay_);

  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
  // --------------
}

void AssemblyMoveWidget::execute()
{
  // X ---
  double X_val(0.);

  const QString X_str = X_lineed_->text().remove(" ");

  if(X_str.isEmpty())
  {
    X_val = move_relative_ ? 0. : this->manager()->get_position_X();
  }
  else
  {
    bool valid_X(false);

    X_val = X_str.toDouble(&valid_X);

    if(valid_X == false)
    {
      NQLog("AssemblyMoveWidget", NQLog::Warning) << "execute"
         << ": invalid format for x-axis value (" << X_str << "), no action taken";

      return;
    }
  }
  // -----

  // Y ---
  double Y_val(0.);

  const QString Y_str = Y_lineed_->text().remove(" ");

  if(Y_str.isEmpty())
  {
    Y_val = move_relative_ ? 0. : this->manager()->get_position_Y();
  }
  else
  {
    bool valid_Y(false);

    Y_val = Y_str.toDouble(&valid_Y);

    if(valid_Y == false)
    {
      NQLog("AssemblyMoveWidget", NQLog::Warning) << "execute"
         << ": invalid format for y-axis value (" << Y_str << "), no action taken";

      return;
    }
  }
  // -----

  // Z ---
  double Z_val(0.);

  const QString Z_str = Z_lineed_->text().remove(" ");

  if(Z_str.isEmpty())
  {
    Z_val = move_relative_ ? 0. : this->manager()->get_position_Z();
  }
  else
  {
    bool valid_Z(false);

    Z_val = Z_str.toDouble(&valid_Z);

    if(valid_Z == false)
    {
      NQLog("AssemblyMoveWidget", NQLog::Warning) << "execute"
         << ": invalid format for z-axis value (" << Z_str << "), no action taken";

      return;
    }
  }
  // -----

  // A ---
  double A_val(0.);

  const QString A_str = A_lineed_->text().remove(" ");

  if(A_str.isEmpty())
  {
    A_val = move_relative_ ? 0. : this->manager()->get_position_A();
  }
  else
  {
    bool valid_A(false);

    A_val = A_str.toDouble(&valid_A);

    if(valid_A == false)
    {
      NQLog("AssemblyMoveWidget", NQLog::Warning) << "execute"
         << ": invalid format for a-axis value (" << A_str << "), no action taken";

      return;
    }
  }
  // -----

  if(move_relative_)
  {
    NQLog("AssemblyMoveWidget", NQLog::Message) << "execute"
       << ": emitting signal \"move_relative("
       << X_val << ", " << Y_val << ", " << Z_val << ", " << A_val << ")\"";

    this->disable();

    emit move_relative(X_val, Y_val, Z_val, A_val);
  }
  else
  {
    NQLog("AssemblyMoveWidget", NQLog::Message) << "execute"
       << ": emitting signal \"move_absolute("
       << X_val << ", " << Y_val << ", " << Z_val << ", " << A_val << ")\"";

    this->enable();

    emit move_absolute(X_val, Y_val, Z_val, A_val);
  }

  return;
}
