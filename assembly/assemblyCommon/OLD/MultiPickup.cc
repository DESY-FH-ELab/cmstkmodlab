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

#include <MutiPickup.h>
#include <AssemblyParameters.h>
#include <AssemblyUtilities.h>

#include <cmath>

MutiPickup::MutiPickup(const LStepExpressMotionManager* const motion_manager, QObject* parent)
 : QObject(parent)

 , motion_manager_(motion_manager)
 , motion_manager_enabled_(false)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("MutiPickup", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    assembly::kill_application(tr("[MutiPickup]"), tr("Null pointer to LStepExpressMotionManager. Aborting Application."));
  }

  qRegisterMetaType<MutiPickup::Configuration>("MutiPickup::Configuration");

  configuration_.reset();

  this->reset();

  connect(this, SIGNAL(nextAlignmentStep(double, double, double)), this, SLOT(run_alignment(double, double, double)));

  connect(this, SIGNAL(motion_completed()), this, SLOT(launch_next_alignment_step()));

  NQLog("MutiPickup", NQLog::Debug) << "constructed";
}

void MutiPickup::Configuration::reset()
{
  object_deltaX = -999.;
  object_deltaY = -999.;

  target_angle = -999.;

  only_measure_angle = true;

  complete_at_position1 = true;

  use_autofocusing = true;

  angle_max_dontIter = -999.;
  angle_max_complete = -999.;

  PatRecOne_configuration.reset();
  PatRecTwo_configuration.reset();
}

bool MutiPickup::Configuration::is_valid() const
{
  if(PatRecOne_configuration.is_valid() == false){ return false; }
  if(PatRecTwo_configuration.is_valid() == false){ return false; }

  return true;
}

void MutiPickup::reset()
{
  alignment_step_ = 0;

  posi_x1_ = 0.;
  posi_y1_ = 0.;
  posi_x2_ = 0.;
  posi_y2_ = 0.;

  obj_angle_deg_ = 0.;

  return;
}

void MutiPickup::update_configuration(const MutiPickup::Configuration& conf)
{
  if(conf.is_valid() == false)
  {
    NQLog("MutiPickup", NQLog::Critical) << "update_configuration"
       << ": invalid input MutiPickup::Configuration object, no action taken";

    return;
  }

  configuration_ = conf;

  NQLog("MutiPickup", NQLog::Spam) << "update_configuration"
     << ": emitting signal \"configuration_updated\"";

  emit configuration_updated();
}

void MutiPickup::redirect_image(const cv::Mat& img)
{
  if(alignment_step_ == 2)
  {
    NQLog("MutiPickup", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecOne\"";

    emit image_PatRecOne(img);
  }
  else if(alignment_step_ == 5)
  {
    NQLog("MutiPickup", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecTwo\"";

    emit image_PatRecTwo(img);
  }

  return;
}

void MutiPickup::enable_motion_manager(const bool arg)
{
  if(arg == motion_manager_enabled_)
  {
    NQLog("MutiPickup", NQLog::Debug) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for MutiPickup already " << (arg ? "enabled" : "disabled") << ", no action taken";

    return;
  }

  if(arg)
  {
    connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    connect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = true;

    NQLog("MutiPickup", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for MutiPickup enabled";
  }
  else
  {
    disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    disconnect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = false;

    NQLog("MutiPickup", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for MutiPickup disabled";
  }

  return;
}

void MutiPickup::move_relative(const double x, const double y, const double z, const double a)
{
  this->connect_motion_manager();

  emit move_relative_request(x, y, z, a);
}

void MutiPickup::complete_motion()
{
  this->disconnect_motion_manager();

  NQLog("MutiPickup", NQLog::Spam) << "complete_motion"
     << ": emitting signal \"motion_completed\"";

  emit motion_completed();
}

void MutiPickup::launch_next_alignment_step()
{
  NQLog("MutiPickup", NQLog::Spam) << "launch_next_alignment_step"
     << ": emitting signal \"nextAlignmentStep(0, 0, 0)\"";

  emit nextAlignmentStep(0.0, 0.0, 0.0);
}

void MutiPickup::execute()
{
  alignment_step_ = 0;

  this->run_alignment(0., 0., 0.);
}

void MutiPickup::run_alignment(const double patrec_dX, const double patrec_dY, const double patrec_angle)
{
  //
  // Alignment of 2-marker sensor:
  //
  //  * routine assumes the initial position corresponds to the position of the 1st marker
  //  * for each of the 2 markers:
  //     a. PatRec: run PatRec and detect (X1,Y1)
  //     b. re-centering: move camera to PatRec result (put marker in centre of fied of view)
  //  * measure angle between the 2 markers
  //  * align to targer orientation (input parameter) by rotating the platform
  //  * iterative procedure to reach target orientation with small-enough rotations
  //

  // Step #0: request image on marker-1
  const AssemblyParameters* const params = AssemblyParameters::instance(false);

  // Step #0: request image on marker-1                                                                                             
  if(alignment_step_ == 0)
  {
    NQLog("MutiPickup", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    if(this->configuration().use_autofocusing)
    {
      NQLog("MutiPickup", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"autofocused_image_request\"";

      ++alignment_step_;

      emit autofocused_image_request();
    }
    else
    {
      NQLog("MutiPickup", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"image_request\"";

      ++alignment_step_;

      emit image_request();
    }
  }
  // Step #1: PatRec on marker-1                                                                                                    \
                                                                                                                                     
  else if(alignment_step_ == 1)
    {
    NQLog("MutiPickup", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    NQLog("MutiPickup", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
					 << ": emitting signal \"PatRec_request\"";

    ++alignment_step_;

    emit PatRec_request(this->configuration().PatRecOne_configuration);
  }

  // Step #2: Report back the x,y position of the image

  else if(alignment_step_ == 2)
    {
      // marker-1: position of PatRec best-match 
      posi_x1_ = motion_manager_->get_position_X() + patrec_dX;
      posi_y1_ = motion_manager_->get_position_Y() + patrec_dY;

      const double dX = params->get("FromPSSRefPointToPSSTLRefPoint_dX");

      emit pspbl_pos(posi_x1_,posi_y1_);
      
      ++alignment_step_;

      this->move_relative(dX, 0.0, 0.0, 0.0);
      NQLog("MutiPickup", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("MutiPickup", NQLog::Message) << "run_alignment: Moving Relative [" << alignment_step_ << "]";


    }

  else if(alignment_step_ == 3)
    {
      NQLog("MutiPickup", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      if(this->configuration().use_autofocusing)
	{
          NQLog("MutiPickup", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                               << ": emitting signal \"autofocused_image_request\"";
          ++alignment_step_;

          emit autofocused_image_request();
        }
      else
        {
          NQLog("MutiPickup", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                               << ": emitting signal \"image_request\"";

          ++alignment_step_;

          emit image_request();
        }
    }

  else if(alignment_step_ == 4)
    {
      NQLog("MutiPickup", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      NQLog("MutiPickup", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                           << ": emitting signal \"PatRec_request\"";

      ++alignment_step_;
      emit PatRec_request(this->configuration().PatRecTwo_configuration);
  }

  else if(alignment_step_ == 5)
    {
    // marker-2: position of PatRec best-match                                                                                      \
                                                                                                                                    
      posi_x2_ = motion_manager_->get_position_X() + patrec_dX;
      posi_y2_ = motion_manager_->get_position_Y() + patrec_dY;

      const double dX = params->get("FromPSSRefPointToPSSTLRefPoint_dX");
 
      emit pssbl_pos (posi_x2_, posi_y2_);

      ++alignment_step_;

      this->move_relative(-dX, 0.0, 0.0, 0.0);
    }


  else if(alignment_step_ == 6)
    {
    // marker-4: position of PatRec best-match   

      double slope  = (posi_y2_ - posi_y1_) / (posi_x2_ - posi_x1_);
      double meas_angle = atan2(slope) *1000000.;


      emit measured_angle(meas_angle);

      NQLog("MutiPickup", NQLog::Message) << "Angle measured from the PS-S to the PS-P: step [" << meas_angle << "]";

      this->reset();

      emit execution_completed();

    }
}
