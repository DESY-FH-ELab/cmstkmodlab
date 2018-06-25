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

#include <AssemblyObjectAligner.h>
#include <AssemblyParameters.h>
#include <AssemblyUtilities.h>

#include <cmath>

AssemblyObjectAligner::AssemblyObjectAligner(const LStepExpressMotionManager* const motion_manager, QObject* parent) :
  QObject(parent),
  motion_manager_(motion_manager),
  motion_manager_enabled_(false)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblyObjectAligner", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  qRegisterMetaType<AssemblyObjectAligner::Configuration>("AssemblyObjectAligner::Configuration");

  ApplicationConfig* config = ApplicationConfig::instance();
  if(config == nullptr)
  {
    NQLog("AssemblyObjectAligner", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    return;
  }

  // maximum angular difference acceptable not to trigger iterative procedure for alignment
  angle_max_dontIter_ = config->getValue<double>("AssemblyObjectAligner_angle_max_dontIter");

  // maximum angular difference acceptable to declare alignment procedure finished
  angle_max_complete_ = config->getValue<double>("AssemblyObjectAligner_angle_max_complete");

  configuration_.reset();

  this->reset();

  connect(this, SIGNAL(nextAlignmentStep(double, double, double)), this, SLOT(run_alignment(double, double, double)));

  connect(this, SIGNAL(motion_completed()), this, SLOT(launch_next_alignment_step()));

  NQLog("AssemblyObjectAligner", NQLog::Debug) << "constructed";
}

void AssemblyObjectAligner::Configuration::reset()
{
  object_deltaX = -999.;
  object_deltaY = -999.;

  completeAtPosOne = true;

  only_measure_angle = true;

  target_angle = -999.;

  PatRecOne_configuration.reset();
  PatRecTwo_configuration.reset();
}

bool AssemblyObjectAligner::Configuration::is_valid() const
{
  if(object_deltaX == -999.){ return false; }
  if(object_deltaY == -999.){ return false; }

  if(only_measure_angle == false)
  {
    if(fabs(target_angle) > 180.){ return false; }
  }

  if(PatRecOne_configuration.is_valid() == false){ return false; }
  if(PatRecTwo_configuration.is_valid() == false){ return false; }

  return true;
}

void AssemblyObjectAligner::reset()
{
  alignment_step_ = 0;

  posi_x1_ = 0.;
  posi_y1_ = 0.;
  posi_x2_ = 0.;
  posi_y2_ = 0.;

  obj_angle_deg_ = 0.;

  return;
}

void AssemblyObjectAligner::update_configuration(const AssemblyObjectAligner::Configuration& conf)
{
  if(conf.is_valid() == false)
  {
    NQLog("AssemblyObjectAligner", NQLog::Critical) << "update_configuration"
       << ": invalid input AssemblyObjectAligner::Configuration object, no action taken";

    return;
  }

  configuration_ = conf;

  NQLog("AssemblyObjectAligner", NQLog::Spam) << "update_configuration"
     << ": emitting signal \"configuration_updated\"";

  emit configuration_updated();
}

void AssemblyObjectAligner::redirect_image(const cv::Mat& img)
{
  if(alignment_step_ == 2)
  {
    NQLog("AssemblyObjectAligner", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecOne\"";

    emit image_PatRecOne(img);
  }
  else if(alignment_step_ == 5)
  {
    NQLog("AssemblyObjectAligner", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecTwo\"";

    emit image_PatRecTwo(img);
  }

  return;
}

void AssemblyObjectAligner::enable_motion_manager(const bool arg)
{
  if(arg == motion_manager_enabled_)
  {
    NQLog("AssemblyObjectAligner", NQLog::Debug) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblyObjectAligner already " << (arg ? "enabled" : "disabled") << ", no action taken";

    return;
  }

  if(arg)
  {
    connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    connect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = true;

    NQLog("AssemblyObjectAligner", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblyObjectAligner enabled";
  }
  else
  {
    disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    disconnect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = false;

    NQLog("AssemblyObjectAligner", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblyObjectAligner disabled";
  }

  return;
}

void AssemblyObjectAligner::move_relative(const double x, const double y, const double z, const double a)
{
  this->connect_motion_manager();

  emit move_relative_request(x, y, z, a);
}

void AssemblyObjectAligner::complete_motion()
{
  this->disconnect_motion_manager();

  NQLog("AssemblyObjectAligner", NQLog::Spam) << "complete_motion"
     << ": emitting signal \"motion_completed\"";

  emit motion_completed();
}

void AssemblyObjectAligner::launch_next_alignment_step()
{
  NQLog("AssemblyObjectAligner", NQLog::Spam) << "launch_next_alignment_step"
     << ": emitting signal \"nextAlignmentStep(0, 0, 0)\"";

  emit nextAlignmentStep(0.0, 0.0, 0.0);
}

void AssemblyObjectAligner::execute()
{
  alignment_step_ = 0;

  this->run_alignment(0., 0., 0.);
}

void AssemblyObjectAligner::run_alignment(const double patrec_dX, const double patrec_dY, const double patrec_angle)
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
  if(alignment_step_ == 0)
  {
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
       << ": emitting signal \"image_request\"";

    ++alignment_step_;

    emit image_request();
  } 
  // Step #1: PatRec on marker-1
  else if(alignment_step_ == 1)
  {
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
       << ": emitting signal \"PatRec_request\"";

    ++alignment_step_;

    emit PatRec_request(this->configuration().PatRecOne_configuration);
  }
  // Step #2: move to marker-2
  else if(alignment_step_ == 2)
  {
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]"
       << ": determining best-match position of PatRec #1";

    // marker-1: position of PatRec best-match
    posi_x1_ = motion_manager_->get_position_X() + patrec_dX;
    posi_y1_ = motion_manager_->get_position_Y() + patrec_dY;

    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: motion-stage X = " << motion_manager_->get_position_X();
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: motion-stage Y = " << motion_manager_->get_position_Y();
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: PatRec dX = " << patrec_dX;
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: PatRec dY = " << patrec_dY;
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: x1-position = " << posi_x1_;
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: y1-position = " << posi_y1_;
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    // relative movement to reach the opposite marker
    const double obj_deltaX = this->configuration().object_deltaX;
    const double obj_deltaY = this->configuration().object_deltaY;

    const AssemblyParameters* const params = AssemblyParameters::instance(false);

    // angle from marker's outer edge to best-match position in the camera ref-frame
    //   - the 90.0 deg offset corresponds to the angle spanned by the marker (L-shape)
    const double patrec_angle_full = (patrec_angle + 90.0);

    const double camera_offset_dA = params->get("AngleOfCameraFrameInRefFrame_dA");

    double dX_1to2, dY_1to2;
    assembly::rotation2D_deg(dX_1to2, dY_1to2, (patrec_angle_full + camera_offset_dA), obj_deltaX, obj_deltaY);

    // combined relative movement
    const double dX = patrec_dX + dX_1to2;
    const double dY = patrec_dY + dY_1to2;

    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]"
       << ": moving to position for PatRec #2";

    NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
       << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, 0)\"";

    ++alignment_step_;

    this->move_relative(dX, dY, 0.0, 0.0);
  }
  // Step #3: request image on marker-2
  else if(alignment_step_ == 3)
  {
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]"
       << ": acquiring image for PatRec #2";

    NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
       << ": emitting signal \"image_request\"";

    ++alignment_step_;

    emit image_request();
  }
  // Step #4: PatRec on marker-2
  else if(alignment_step_ == 4)
  {
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
       << ": emitting signal \"PatRec_request\"";

    ++alignment_step_;

    emit PatRec_request(this->configuration().PatRecTwo_configuration);
  }
  // Step #5: move back to marker-1
  else if(alignment_step_ == 5)
  {
    posi_x2_ = motion_manager_->get_position_X() + patrec_dX;
    posi_y2_ = motion_manager_->get_position_Y() + patrec_dY;

    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: position(X1) = " << posi_x1_;
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: position(Y1) = " << posi_y1_;
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: position(X2) = " << posi_x2_;
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: position(Y2) = " << posi_y2_;

    // measurement of object orientation
    if(posi_x2_ == posi_x1_)
    {
      obj_angle_deg_ = (posi_y2_ == posi_y1_) ? 0. : ((posi_y2_ > posi_y1_) ? 90. : -90.);
    }
    else
    {
      const double obj_slope = ((posi_y2_ - posi_y1_)/(posi_x2_ - posi_x1_));

      obj_angle_deg_ = atan(obj_slope) * (180.0/M_PI);
    }

    emit measured_angle(obj_angle_deg_);

    // relative (X,Y) movement to reach the opposite marker
    const double dX = (posi_x1_ - motion_manager_->get_position_X());
    const double dY = (posi_y1_ - motion_manager_->get_position_Y());

    if(this->configuration().only_measure_angle)
    {
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: mode = ONLY-MEASURE-ANGLE";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: object aligned with angle [deg] = " << obj_angle_deg_;
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      if(this->configuration().completeAtPosOne)
      {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: alignment completed, moving back to best-match position of PatRec #1";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

        NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
           << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, 0)\"";

        ++alignment_step_;

        this->move_relative(dX, dY, 0.0, 0.0);
      }
      else
      {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: ===> Alignment routine completed successfully";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

        NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
           << ": emitting signal \"execution_completed\"";

        this->reset();

        emit execution_completed();
      }
    }
    else
    {
      const double target_angle_deg = this->configuration().target_angle;

      const double delta_angle_deg = (target_angle_deg - obj_angle_deg_);

      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: mode = ALIGNMENT";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: object aligned with angle    [deg] = " << obj_angle_deg_;
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: target alignment angle       [deg] = " << target_angle_deg;
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: angular distance from target [deg] = " << delta_angle_deg;
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      if(fabs(delta_angle_deg) > angle_max_complete_)
      {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: angle(object, target) > " << angle_max_complete_ << ", will apply a rotation";

        if(fabs(delta_angle_deg) <= angle_max_dontIter_)
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: angle(object, target) <= " << angle_max_dontIter_ << ", moving to target angle with 1 rotation";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_
             << "]: ===> will rotate by " << delta_angle_deg << " deg, and move back to best-match position of PatRec #1";

          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
             << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, " << delta_angle_deg << ")\"";

          this->reset();

          this->move_relative(dX, dY, 0.0, delta_angle_deg);
        }
        else
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: angle(object, target) > " << angle_max_dontIter_ << " deg, large rotation required";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_
             << "]: ===> iterative procedure (will rotate by " << angle_max_dontIter_ << " deg, and move back to best-match position of PatRec #1";

          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          const double rot_deg = (delta_angle_deg <= 0) ? (-1.0 * angle_max_dontIter_) : (+1.0 * angle_max_dontIter_);

          NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
             << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, " << rot_deg << ")\"";

          this->reset();

          this->move_relative(dX, dY, 0.0, rot_deg);
        }
      }
      else
      {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: angle(object, target) < " << angle_max_complete_ << ", will NOT apply a rotation";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

        if(this->configuration().completeAtPosOne)
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: alignment completed, moving back to best-match position of PatRec #1";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
             << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, 0)\"";

          ++alignment_step_;

          this->move_relative(dX, dY, 0.0, 0.0);
        }
        else
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: object aligned with angle [deg] = " << obj_angle_deg_;
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: ===> Alignment routine completed successfully";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
             << ": emitting signal \"execution_completed\"";

          this->reset();

          emit execution_completed();
        }
      }
    }
  }
  // Step #6: request image on marker-1
  else if(alignment_step_ == 6)
  {
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]"
       << ": acquiring image in position of PatRec #1, after completing the alignment";

    NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
       << ": emitting signal \"image_request\"";

    ++alignment_step_;

    emit image_request();
  }
  // Step #7: close alignment routine
  else if(alignment_step_ == 7)
  {
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: object aligned with angle [deg] = " << obj_angle_deg_;
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: ===> Alignment routine completed successfully";
    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    this->reset();

    emit execution_completed();
  }

  return;
}
