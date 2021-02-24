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

#include <AssemblyMultiPickupTesterWidget.h>

#include <sstream>
#include <iomanip>
#include <stdlib.h>

#include <QVBoxLayout>
#include <QStringList>

AssemblyMultiPickupTesterWidget::AssemblyMultiPickupTesterWidget(const LStepExpressMotionManager* const motion_manager, QWidget* parent) :
  QWidget(parent),

  motion_manager_(motion_manager)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Critical)
       << "input error: null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  QLabel *label = new QLabel(this);
  label->setText("<p style=color:orange><b>WARNING: you must activate the baseplate vacuum before starting !</p></b>");
  layout->addWidget(label);

  exe_button_ = new QPushButton(tr("Run Multi-Pickup Test"));
  layout->addWidget(exe_button_);

  QGridLayout* g2 = new QGridLayout;
  layout->addLayout(g2);

  measur_label_  = new QLabel("PatRec Measurement Position (x,y,z)");
  measur_lineed_ = new QLineEdit("0,0,0");
  measur_button_ = new QPushButton("Update");
  g2->addWidget(measur_label_ , 0, 0);
  g2->addWidget(measur_lineed_, 0, 1);
  g2->addWidget(measur_button_, 0, 2);

  pickup_label_  = new QLabel("Pickup Position (x,y,z)");
  pickup_lineed_ = new QLineEdit("0,0,0");
  pickup_button_ = new QPushButton("Update");
  g2->addWidget(pickup_label_ , 1, 0);
  g2->addWidget(pickup_lineed_, 1, 1);
  g2->addWidget(pickup_button_, 1, 2);

  iteraN_label_  = new QLabel("N Iterations (int)");
  iteraN_lineed_ = new QLineEdit("5");
  g2->addWidget(iteraN_label_ , 2, 0);
  g2->addWidget(iteraN_lineed_, 2, 1);

  connect(exe_button_   , SIGNAL(clicked()), this, SLOT(execute()));
  connect(measur_button_, SIGNAL(clicked()), this, SLOT(update_position_measurement()));
  connect(pickup_button_, SIGNAL(clicked()), this, SLOT(update_position_pickup()));

  layout->addStretch(1);
}

void AssemblyMultiPickupTesterWidget::update_position_measurement()
{
  const double x = motion_manager_->get_position_X();
  const double y = motion_manager_->get_position_Y();
  const double z = motion_manager_->get_position_Z();

  std::stringstream posi_strs;
  posi_strs << x << ", " << y << ", " << z;

  measur_lineed_->setText(QString::fromStdString(posi_strs.str()));

  return;
}

void AssemblyMultiPickupTesterWidget::update_position_pickup()
{
  const double x = motion_manager_->get_position_X();
  const double y = motion_manager_->get_position_Y();
  const double z = motion_manager_->get_position_Z();

  std::stringstream posi_strs;
  posi_strs << x << ", " << y << ", " << z;

  pickup_lineed_->setText(QString::fromStdString(posi_strs.str()));

  return;
}

void AssemblyMultiPickupTesterWidget::enable(const bool b)
{
  exe_button_   ->setEnabled(b);

  measur_button_->setEnabled(b);
  pickup_button_->setEnabled(b);

  measur_lineed_->setEnabled(b);
  pickup_lineed_->setEnabled(b);
  iteraN_lineed_->setEnabled(b);

  return;
}

void AssemblyMultiPickupTesterWidget::execute()
{
  // measurement position
  const QString measur_qstr = this->measur_lineed_->text().remove(" ");

  const QStringList measur_qsl = measur_qstr.split(",");

  if(measur_qsl.length() != 3)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for measurement position (" << measur_qstr << "), no action taken";

    this->enable(true);

    return;
  }

  bool valid_measur_x(false);
  bool valid_measur_y(false);
  bool valid_measur_z(false);

  const double measur_x = measur_qsl.at(0).toDouble(&valid_measur_x);
  const double measur_y = measur_qsl.at(1).toDouble(&valid_measur_y);
  const double measur_z = measur_qsl.at(2).toDouble(&valid_measur_z);

  if(!valid_measur_x)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for x-axis value of measurement position (" << measur_qsl.at(0) << "), no action taken";

    this->enable(true);

    return;
  }
  else if(!valid_measur_y)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for y-axis value of measurement position (" << measur_qsl.at(1) << "), no action taken";

    this->enable(true);

    return;
  }
  else if(!valid_measur_z)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for z-axis value of measurement position (" << measur_qsl.at(2) << "), no action taken";

    this->enable(true);

    return;
  }

  // pickup position
  const QString pickup_qstr = this->pickup_lineed_->text().remove(" ");

  const QStringList pickup_qsl = pickup_qstr.split(",");

  if(pickup_qsl.length() != 3)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for pick-up position (" << pickup_qstr << "), no action taken";

    this->enable(true);

    return;
  }

  bool valid_pickup_x(false);
  bool valid_pickup_y(false);
  bool valid_pickup_z(false);

  const double pickup_x = pickup_qsl.at(0).toDouble(&valid_pickup_x);
  const double pickup_y = pickup_qsl.at(1).toDouble(&valid_pickup_y);
  const double pickup_z = pickup_qsl.at(2).toDouble(&valid_pickup_z);

  if(!valid_pickup_x)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for x-axis value of pickup position (" << pickup_qsl.at(0) << "), no action taken";

    this->enable(true);

    return;
  }
  else if(!valid_pickup_y)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for y-axis value of pickup position (" << pickup_qsl.at(1) << "), no action taken";

    this->enable(true);

    return;
  }
  else if(!valid_pickup_z)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for z-axis value of pickup position (" << pickup_qsl.at(2) << "), no action taken";

    this->enable(true);

    return;
  }

  // number of iterations
  const QString iteraN_qstr = this->iteraN_lineed_->text();

  bool valid_iteraN(false);

  const int iteraN = iteraN_qstr.toInt(&valid_iteraN);

  if(!valid_iteraN)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for number of iterations (" << iteraN_qstr << "), no action taken";

    this->enable(true);

    return;
  }

  // output signal
  NQLog("AssemblyMultiPickupTesterWidget", NQLog::Spam) << "execute"
     << ": emitting signal \"multipickup_request"
     <<  "(m_x=" << measur_x <<  ", m_y=" << measur_y << ", m_z=" << measur_z
     << ", p_x=" << pickup_x <<  ", p_y=" << pickup_y << ", p_z=" << pickup_z
     << ", n=" << iteraN << ")\"";

  const AssemblyMultiPickupTester::Configuration mpt_conf(
    measur_x, measur_y, measur_z,
    pickup_x, pickup_y, pickup_z,
    iteraN
  );

  emit multipickup_request(mpt_conf);
}
