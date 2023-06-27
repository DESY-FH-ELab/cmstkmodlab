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

#include <AssemblySmartMotionManager.h>
#include <AssemblyUtilities.h>

#include <QMessageBox>
#include <QPushButton>

#include <cmath>

AssemblySmartMotionManager::AssemblySmartMotionManager(const LStepExpressMotionManager* const motion_manager, QObject* parent)
 : QObject(parent)

 , motion_manager_(motion_manager)
 , motion_manager_enabled_(false)
 , motion_index_(-1)

 , smartMove_initial_step_up_dZ_(0.)

 , smartMotions_N_(0)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblySmartMotionManager", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    assembly::kill_application(tr("[AssemblySmartMotionManager]"), tr("Null pointer to LStepExpressMotionManager. Aborting Application."));
  }

  // smartMove configuration
  ApplicationConfig* config = ApplicationConfig::instance();
  if(config == nullptr)
  {
    NQLog("AssemblySmartMotionManager", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    assembly::kill_application(tr("[AssemblySmartMotionManager]"), tr("Null pointer to ApplicationConfig. Aborting Application."));
  }

  const std::string smartMove_steps_dZ_str = config->getDefaultValue<std::string>("main", "AssemblySmartMotionManager_steps_dZ", "");

  const QStringList smartMove_steps_dZ_strlist = QString::fromStdString(smartMove_steps_dZ_str).split(",");

  smartMove_steps_dZ_.clear();

  for(const auto& step_qstr : smartMove_steps_dZ_strlist)
  {
    bool valid_step(false);

    const double step_val = step_qstr.toDouble(&valid_step);

    if(valid_step == false)
    {
      NQLog("AssemblySmartMotionManager", NQLog::Fatal)
         << ": invalid format for smartMove step (" << step_qstr << "), closing application";

      assembly::kill_application(tr("[AssemblySmartMotionManager]"), "Invalid format for smartMove step (\""+step_qstr+"\"). Closing Application.");
    }

    if(step_val <= 0.)
    {
      NQLog("AssemblySmartMotionManager", NQLog::Fatal)
         << ": invalid (non-positive) value for smartMove step (" << step_qstr << "), closing application";

      assembly::kill_application(tr("[AssemblySmartMotionManager]"), "Invalid (non-positive) value for smartMove step (\""+step_qstr+"\"). Closing Application.");
    }

    smartMove_steps_dZ_.emplace_back(step_val);
  }

  NQLog("AssemblySmartMotionManager", NQLog::Message)
     << "loaded " << smartMove_steps_dZ_.size() << " smartMove steps (\"" << smartMove_steps_dZ_str << "\")";

  smartMove_initial_step_up_dZ_ = config->getDefaultValue<double>("main", "AssemblySmartMotionManager_initial_step_up_dZ", 1.0);

  // -----------------------

  NQLog("AssemblySmartMotionManager", NQLog::Debug) << "constructed";
}

void AssemblySmartMotionManager::enable_motion_manager(const bool arg)
{
  if(arg == motion_manager_enabled_)
  {
    NQLog("AssemblySmartMotionManager", NQLog::Debug) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblySmartMotionManager already " << (arg ? "enabled" : "disabled") << ", no action taken";

    return;
  }

  if(arg)
  {
    connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    connect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_manager_, SLOT(moveAbsolute(double, double, double, double)));

    connect(this, SIGNAL(motion_request(LStepExpressMotion)), motion_manager_, SLOT(appendMotion(LStepExpressMotion)));

    connect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(next_step()));

    motion_manager_enabled_ = true;

    NQLog("AssemblySmartMotionManager", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblySmartMotionManager enabled";
  }
  else
  {
    disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    disconnect(this, SIGNAL(move_absolute_request(double, double, double, double)), motion_manager_, SLOT(moveAbsolute(double, double, double, double)));

    disconnect(this, SIGNAL(motion_request(LStepExpressMotion)), motion_manager_, SLOT(appendMotion(LStepExpressMotion)));

    disconnect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(next_step()));

    motion_manager_enabled_ = false;

    NQLog("AssemblySmartMotionManager", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblySmartMotionManager disabled";
  }

  return;
}

void AssemblySmartMotionManager::move_absolute(const double x0, const double y0, const double z0, const double a0)
{
  const double dx0 = (x0 - motion_manager_->get_position_X());
  const double dy0 = (y0 - motion_manager_->get_position_Y());
  const double dz0 = (z0 - motion_manager_->get_position_Z());
  const double da0 = (a0 - motion_manager_->get_position_A());

  this->move_relative(dx0, dy0, dz0, da0);
}

void AssemblySmartMotionManager::move_relative(const double dx0, const double dy0, const double dz0, const double da0)
{
  if(smartMove_steps_dZ_.size() == 0)
  {
    NQLog("AssemblySmartMotionManager", NQLog::Critical) << "move_relative"
       << ": smartMove mode enabled, but empty list of dZ steps, no action taken";

    motions_.clear();

    motion_index_ = -1;

    this->disconnect_motion_manager();

    emit motion_completed();

    return;
  }

  motions_ = this->smartMotions_relative(dx0, dy0, dz0, da0, smartMove_steps_dZ_, smartMotions_N_, smartMove_initial_step_up_dZ_);

  this->connect_motion_manager();

  motion_index_ = -1;

  this->next_step();
}

void AssemblySmartMotionManager::next_step()
{
  if(motion_index_ == (motions_.size() - 1))
  {
    motions_.clear();

    motion_index_ = -1;

    this->disconnect_motion_manager();

    emit motion_completed();
  }
  else if(motion_index_ >= (motions_.size() - 1 - smartMotions_N_))
  {
    ++motion_index_;

    this->smartMove_window(motions_.at(motion_index_));
  }
  else
  {
    ++motion_index_;

    const LStepExpressMotion& motion = motions_.at(motion_index_);

    emit motion_request(motion);
  }
}

void AssemblySmartMotionManager::smartMove_window(const LStepExpressMotion& motion)
{
  QMessageBox msgBox;

  QString text_1 = tr("Do you want to apply the following movement ? ");
  text_1 += " (Motion "+QString::number(1 + motion_index_)+" of "+QString::number(motions_.size())+")";

  msgBox.setText(text_1);

  QString text_2 = motion.getMode() ? tr("moveAbsolute") : tr("moveRelative");
  text_2 += " (X="+QString::number(motion.getX());
  text_2 += ", Y="+QString::number(motion.getY());
  text_2 += ", Z="+QString::number(motion.getZ());
  text_2 += ", A="+QString::number(motion.getA());
  text_2 += ") [mm, deg]";

  msgBox.setInformativeText(text_2);

  QPushButton* moveButton = msgBox.addButton(tr("Apply Movement") , QMessageBox::ActionRole);
  QPushButton* stopButton = msgBox.addButton(tr("Motion Finished"), QMessageBox::ActionRole);

  msgBox.setDefaultButton(moveButton);

  msgBox.exec();

  if(msgBox.clickedButton() == stopButton)
  {
    motions_.clear();

    motion_index_ = -1;

    this->disconnect_motion_manager();

    emit motion_completed();
  }
  else if(msgBox.clickedButton() == moveButton)
  {
    emit motion_request(motion);
  }
}

QQueue<LStepExpressMotion> AssemblySmartMotionManager::smartMotions_relative(const double dx, const double dy, const double dz, const double da, const std::vector<double>& dz_steps, int& smart_motions_N, const double initial_dZ)
{
  QQueue<LStepExpressMotion> motions;

  smart_motions_N = 0;

  const bool move_xya = ((dx != 0.) || (dy != 0.) || (da != 0.));

  if(dz > 0.)
  {
    smart_motions_N = 1;
    motions.enqueue(LStepExpressMotion(0, 0, initial_dZ, 0, false));
    motions.enqueue(LStepExpressMotion(0, 0, dz-initial_dZ, 0, false));

    if(move_xya){ motions.enqueue(LStepExpressMotion(dx, dy, 0, da, false)); }
  }
  else
  {
    if(move_xya){ motions.enqueue(LStepExpressMotion(dx, dy, 0, da, false)); }

    std::vector<double> dz_vec;
    {
      double abs_dz_resid = fabs(dz);

      for(unsigned int istep=dz_steps.size(); istep-- > 0;)
      {
        const double istep_val = dz_steps.at(istep);

        if(abs_dz_resid > istep_val)
        {
          ++smart_motions_N;

          abs_dz_resid +=  -istep_val;
          dz_vec.push_back(-istep_val);
        }
      }

      if(abs_dz_resid > 0.){ dz_vec.push_back(-abs_dz_resid); }
    }

    // reversed loop on dz movements
    for(unsigned int i_dz=dz_vec.size(); i_dz-- > 0;)
    {
      motions.enqueue(LStepExpressMotion(0, 0, dz_vec.at(i_dz), 0, false));
    }
  }

  return motions;
}
