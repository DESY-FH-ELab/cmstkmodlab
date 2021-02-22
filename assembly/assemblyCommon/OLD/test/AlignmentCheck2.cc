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

#include <AlignmentCheck.h>
#include <AssemblyUtilities.h>

#include <string>
#include <QMessageBox>

#include <AssemblyObjectAligner.h>
#include <AlignmentCheck.h>
#include <AssemblyParameters.h>
#include <AssemblyUtilities.h>

#include <cmath>





void AlignmentCheck::GoToSensorMarkerPreAlignment_start()
{
  const AssemblyParameters* const params = AssemblyParameters::instance(false);


  const double x0 = params->get("RefPointSensor_X");
  const double y0 = params->get("RefPointSensor_Y");
  const double z0 = params->get("RefPointSensor_Z");
  const double a0 = params->get("RefPointSensor_A");


  this-> move_absolute(x0, y0, z0, a0);

}


AlignmentCheck::AlignmentCheck(const LStepExpressMotionManager* const motion_manager, QObject* parent)
 : QObject(parent)

 , motion_manager_(motion_manager)
 , motion_manager_enabled_(false)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("AlignmentCheck", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    assembly::kill_application(tr("[AlignmentCheck]"), tr("Null pointer to LStepExpressMotionManager. Aborting Application."));
  }

  qRegisterMetaType<AlignmentCheck::Configuration>("AlignmentCheck::Configuration");

  configuration_.reset();

  this->reset();

  connect(this, SIGNAL(nextAlignmentStep(double, double, double)), this, SLOT(run_alignment(double, double, double)));

  connect(this, SIGNAL(motion_completed()), this, SLOT(launch_next_alignment_step()));

  NQLog("AlignmentCheck", NQLog::Debug) << "constructed";
}

void AlignmentCheck::Configuration::reset()
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

bool AlignmentCheck::Configuration::is_valid() const
{
  if(object_deltaX == -999.){ return false; }
  if(object_deltaY == -999.){ return false; }

  if(only_measure_angle == false)
  {
    if(fabs(target_angle) > 180.){ return false; }

    if(angle_max_dontIter <= 0.)
    {
      return false;
    }
    else if(angle_max_dontIter > 1.0)
    {
      NQLog("AlignmentCheck::Configuration", NQLog::Warning) << "is_valid"
         << ": unexpected value for max. angle for 1-rotation (" << angle_max_dontIter << ")";
    }

    if(angle_max_complete <= 0.)
    {
      return false;
    }
    else if(angle_max_complete > 0.1)
    {
      NQLog("AlignmentCheck::Configuration", NQLog::Warning) << "is_valid"
         << ": unexpected value for max. angle to validate alignment (" << angle_max_complete << ")";
    }
  }

  if(PatRecOne_configuration.is_valid() == false){ return false; }
  if(PatRecTwo_configuration.is_valid() == false){ return false; }

  return true;
}

void AlignmentCheck::reset()
{
  alignment_step_ = 0;

  posi_x1_ = 0.;
  posi_y1_ = 0.;
  posi_x2_ = 0.;
  posi_y2_ = 0.;

  obj_angle_deg_ = 0.;

  return;
}

void AlignmentCheck::update_configuration(const AlignmentCheck::Configuration& conf)
{
  if(conf.is_valid() == false)
  {
    NQLog("AlignmentCheck", NQLog::Critical) << "update_configuration"
       << ": invalid input AlignmentCheck::Configuration object, no action taken";

    return;
  }

  configuration_ = conf;

  NQLog("AlignmentCheck", NQLog::Spam) << "update_configuration"
     << ": emitting signal \"configuration_updated\"";

  emit configuration_updated();
}

void AlignmentCheck::redirect_image(const cv::Mat& img)
{
  if(alignment_step_ == 2)
  {
    NQLog("AlignmentCheck", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecOne\"";

    emit image_PatRecOne(img);
  }
  else if(alignment_step_ == 5)
  {
    NQLog("AlignmentCheck", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecTwo\"";

    emit image_PatRecTwo(img);
  }

  return;
}

void AlignmentCheck::enable_motion_manager(const bool arg)
{
  if(arg == motion_manager_enabled_)
  {
    NQLog("AlignmentCheck", NQLog::Debug) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AlignmentCheck already " << (arg ? "enabled" : "disabled") << ", no action taken";

    return;
  }

  if(arg)
  {
    connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    connect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = true;

    NQLog("AlignmentCheck", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AlignmentCheck enabled";
  }
  else
  {
    disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    disconnect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = false;

    NQLog("AlignmentCheck", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AlignmentCheck disabled";
  }

  return;
}

void AlignmentCheck::move_relative(const double x, const double y, const double z, const double a)
{
  this->connect_motion_manager();

  emit move_relative_request(x, y, z, a);
}

void AlignmentCheck::complete_motion()
{
  this->disconnect_motion_manager();

  NQLog("AlignmentCheck", NQLog::Spam) << "complete_motion"
     << ": emitting signal \"motion_completed\"";

  emit motion_completed();
}

void AlignmentCheck::launch_next_alignment_step()
{
  NQLog("AlignmentCheck", NQLog::Spam) << "launch_next_alignment_step"
     << ": emitting signal \"nextAlignmentStep(0, 0, 0)\"";

  emit nextAlignmentStep(0.0, 0.0, 0.0);
}

void AlignmentCheck::execute()
{
  alignment_step_ = 0;

  this->run_alignment(0., 0., 0.);
}

void AlignmentCheck::run_alignment(const double patrec_dX, const double patrec_dY, const double patrec_angle)
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

void AlignmentCheck::run_alignment(const double patrec_dX, const double patrec_dY, const double patrec_angle)
{
  const AssemblyParameters* const params = AssemblyParameters::instance(false);

  // Step #0: request image on marker-1                                                                                             
  if(alignment_step_ == 0)
  {
    NQLog("AlignmentCheck", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    if(this->configuration().use_autofocusing)
    {
      NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"autofocused_image_request\"";

      ++alignment_step_;

      emit autofocused_image_request();
    }
    else
    {
      NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"image_request\"";

      ++alignment_step_;

      emit image_request();
    }
  }
  // Step #1: PatRec on marker-1                                                                                                    \
                                                                                                                                     
  else if(alignment_step_ == 1)
    {
    NQLog("AlignmentCheck", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
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

      const double dX = params->get("FromPSPRefPointToPSSRefPoint_dX");
      const double dZ = params->get("FromPSPRefPointToPSSRefPoint_dZ");

      ++alignment_step_;

      this->move_relative(dX, 0.0, dZ, 0.0);

    }

  if(alignment_step_ == 3)
    {
      NQLog("AlignmentCheck", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      if(this->configuration().use_autofocusing)
	{
          NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                               << ": emitting signal \"autofocused_image_request\"";


          ++alignment_step_;

          emit autofocused_image_request();
        }
      else
        {
          NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                               << ": emitting signal \"image_request\"";

          ++alignment_step_;

          emit image_request();
	}
    }

  else if(alignment_step_ == 4)
    {
      NQLog("AlignmentCheck", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                           << ": emitting signal \"PatRec_request\"";

      ++alignment_step_;

      emit PatRec_request(this->configuration().PatRecOne_configuration);
  }

  else if(alignment_step_ == 5)
    {
    // marker-2: position of PatRec best-match                                                                                      \
                                                                                                                                     

      posi_x2_ = motion_manager_->get_position_X() + patrec_dX;
      posi_y2_ = motion_manager_->get_position_Y() + patrec_dY;

      const double dX = params->get("FromPSSRefPointToPSSTLRefPoint_dX");
      const double dZ = params->get("FromPSSRefPointToPSSTLRefPoint_dZ");

      ++alignment_step_;

      this->move_relative(dX, 0.0, dZ, 0.0);
    }


  if(alignment_step_ == 6)
    {
      NQLog("AlignmentCheck", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      if(this->configuration().use_autofocusing)
	{
          NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                               << ": emitting signal \"autofocused_image_request\"";

          ++alignment_step_;

          emit autofocused_image_request();
        }
      else
        {
          NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                               << ": emitting signal \"image_request\"";

          ++alignment_step_;

          emit image_request();
        }
    }

  else if(alignment_step_ == 7)
    {
      NQLog("AlignmentCheck", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                           << ": emitting signal \"PatRec_request\"";

      ++alignment_step_;

      emit PatRec_request(this->configuration().PatRecTwo_configuration);
  }

  else if(alignment_step_ == 8)
    {
    // marker-3: position of PatRec best-match                                                                                       

      posi_x3_ = motion_manager_->get_position_X() + patrec_dX;
      posi_y3_ = motion_manager_->get_position_Y() + patrec_dY;

      const double dX = params->get("FromPSSTLToPSPTL_dX");
      const double dZ = params->get("FromPSSTLToPSPTL_dZ");

      ++alignment_step_;

      this->move_relative(dX, 0.0, dZ, 0.0);
    }
  else if(alignment_step_ == 9)
    {
      NQLog("AlignmentCheck", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      if(this->configuration().use_autofocusing)
	{
          NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                               << ": emitting signal \"autofocused_image_request\"";

          ++alignment_step_;

          emit autofocused_image_request();
        }
      else
        {
          NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                               << ": emitting signal \"image_request\"";

          ++alignment_step_;

          emit image_request();
        }
    }
  else if(alignment_step_ == 10)
    {
      NQLog("AlignmentCheck", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      NQLog("AlignmentCheck", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
                                           << ": emitting signal \"PatRec_request\"";

      ++alignment_step_;
      emit PatRec_request(this->configuration().PatRecTwo_configuration);
    }


  else if(alignment_step_ == 11)
    {
    // marker-4: position of PatRec best-match                                                                                       

      posi_x4_ = motion_manager_->get_position_X() + patrec_dX;
      posi_y4_ = motion_manager_->get_position_Y() + patrec_dY;

      ++alignment_step_;
    }
  else if(alignment_step_ == 12)
    {
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: object aligned with angle \
[d\                                                                                                                                  
eg] = " << obj_angle_deg_;
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: ===> Alignment routine com\
pl\                                                                                                                                  
eted successfully";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      this->reset();

      emit execution_completed();
    }

}
