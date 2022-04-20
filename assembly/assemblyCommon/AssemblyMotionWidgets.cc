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

AssemblyMoveWidget::AssemblyMoveWidget(const LStepExpressMotionManager* const manager, QWidget* parent)
 : QWidget(parent)

 , manager_(manager)

 , in_execution_(false)

 , layout_(nullptr)

 , lay_XYZA_(nullptr)

 , X_lineed_(nullptr)
 , Y_lineed_(nullptr)
 , Z_lineed_(nullptr)
 , A_lineed_(nullptr)

 , lay_cmds_(nullptr)

 , button_moveabs_(nullptr)
 , button_moverel_(nullptr)
 , button_readpos_(nullptr)
 , button_clear_  (nullptr)
{
  // motion manager
  if(manager_ == nullptr)
  {
    NQLog("AssemblyMoveWidget", NQLog::Critical) << "initialization error"
      << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }
  // --------------

  // layout
  layout_ = new QVBoxLayout;
  this->setLayout(layout_);

  lay_XYZA_ = new QHBoxLayout;
  layout_->addLayout(lay_XYZA_);

  QLabel* X_label = new QLabel("X [mm]");
  QLabel* Y_label = new QLabel("Y [mm]");
  QLabel* Z_label = new QLabel("Z [mm]");
  QLabel* A_label = new QLabel("A [deg]");

  X_lineed_ = new QLineEdit("");
  Y_lineed_ = new QLineEdit("");
  Z_lineed_ = new QLineEdit("");
  A_lineed_ = new QLineEdit("");

  lay_XYZA_->addWidget(X_label);
  lay_XYZA_->addWidget(X_lineed_);

  lay_XYZA_->addWidget(Y_label);
  lay_XYZA_->addWidget(Y_lineed_);

  lay_XYZA_->addWidget(Z_label);
  lay_XYZA_->addWidget(Z_lineed_);

  lay_XYZA_->addWidget(A_label);
  lay_XYZA_->addWidget(A_lineed_);
  // ------

  lay_cmds_ = new QHBoxLayout;
  layout_->addLayout(lay_cmds_);

  button_moveabs_ = new QPushButton("Move Absolute");
  button_moverel_ = new QPushButton("Move Relative");
  button_readpos_ = new QPushButton("Read Current XYZA");
  button_clear_   = new QPushButton("Clear XYZA");

  lay_cmds_->addWidget(button_moveabs_);
  lay_cmds_->addWidget(button_moverel_);
  lay_cmds_->addWidget(button_readpos_);
  lay_cmds_->addWidget(button_clear_);
  // ------

  connect(this, SIGNAL(move_absolute(double, double, double, double)), manager_, SLOT(moveAbsolute(double, double, double, double)));
  connect(this, SIGNAL(move_relative(double, double, double, double)), manager_, SLOT(moveRelative(double, double, double, double)));

  connect(manager_, SIGNAL(signalDeactivate()), this, SLOT(deactivate()));
  connect(manager_, SIGNAL(signalReactivate()), this, SLOT(reactivate()));
  
  connect(manager_, SIGNAL(motion_finished()), this, SLOT(reactivate()));

  connect(button_moveabs_, SIGNAL(clicked()), this, SLOT(move_absolute()));
  connect(button_moverel_, SIGNAL(clicked()), this, SLOT(move_relative()));

  connect(button_readpos_, SIGNAL(clicked()), this, SLOT( read_positions()));
  connect(button_clear_  , SIGNAL(clicked()), this, SLOT(clear_positions()));
  // --------------
}

void AssemblyMoveWidget::reactivate()
{
  if(in_execution_)
  {
    this->setEnabled(true);

    in_execution_ = false;
  }

  return;
}

void AssemblyMoveWidget::deactivate()
{
  in_execution_ = true;

  this->setEnabled(false);

  return;
}

void AssemblyMoveWidget::clear_positions()
{
  X_lineed_->setText(tr(""));
  Y_lineed_->setText(tr(""));
  Z_lineed_->setText(tr(""));
  A_lineed_->setText(tr(""));
}

void AssemblyMoveWidget::read_positions()
{
  const double x = manager_->get_position_X();
  const double y = manager_->get_position_Y();
  const double z = manager_->get_position_Z();
  const double a = manager_->get_position_A();

  std::stringstream posi_strs_X; posi_strs_X << x;
  std::stringstream posi_strs_Y; posi_strs_Y << y;
  std::stringstream posi_strs_Z; posi_strs_Z << z;
  std::stringstream posi_strs_A; posi_strs_A << a;

  X_lineed_->setText(QString::fromStdString(posi_strs_X.str()));
  Y_lineed_->setText(QString::fromStdString(posi_strs_Y.str()));
  Z_lineed_->setText(QString::fromStdString(posi_strs_Z.str()));
  A_lineed_->setText(QString::fromStdString(posi_strs_A.str()));
}

void AssemblyMoveWidget::execute(const bool relative_move)
{
  in_execution_ = true;

  // X ---
  double X_val(0.);

  const QString X_str = X_lineed_->text().remove(" ");

  if(X_str.isEmpty())
  {
    X_val = relative_move ? 0. : this->manager()->get_position_X();
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
    Y_val = relative_move ? 0. : this->manager()->get_position_Y();
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
    Z_val = relative_move ? 0. : this->manager()->get_position_Z();
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
    A_val = relative_move ? 0. : this->manager()->get_position_A();
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

  if(relative_move)
  {
    NQLog("AssemblyMoveWidget", NQLog::Message) << "execute"
       << ": emitting signal \"move_relative("
       << X_val << ", " << Y_val << ", " << Z_val << ", " << A_val << ")\"";

    this->deactivate();

    emit move_relative(X_val, Y_val, Z_val, A_val);
  }
  else
  {
    NQLog("AssemblyMoveWidget", NQLog::Message) << "execute"
       << ": emitting signal \"move_absolute("
       << X_val << ", " << Y_val << ", " << Z_val << ", " << A_val << ")\"";

    this->deactivate();

    emit move_absolute(X_val, Y_val, Z_val, A_val);
  }

  return;
}
