/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
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

AssemblyObjectAligner::AssemblyObjectAligner(const LStepExpressMotionManager* const motion_manager, QObject* parent)
 : QObject(parent)

 , motion_manager_(motion_manager)
 , motion_manager_enabled_(false)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblyObjectAligner", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    assembly::kill_application(tr("[AssemblyObjectAligner]"), tr("Null pointer to LStepExpressMotionManager. Aborting Application."));
  }

  //When launching alignment routine, most params get read by AssemblyObjectAlignerView, and copied into this->configuration_.
  //But other params are not related to the View (not editable in GUI), so must read their values manually here
  ApplicationConfig* config = ApplicationConfig::instance(); //Access config file parameters
  if(not config)
  {
    NQLog("AssemblyObjectAligner", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    assembly::kill_application(tr("[AssemblyObjectAligner]"), tr("ApplicationConfig::instance() not initialized (null pointer), closing application"));
  }
  else {
    max_numOfRotations_ = config->getDefaultValue<int>("AssemblyObjectAligner_maxNumberOfRotations", 10);
  }

  qRegisterMetaType<AssemblyObjectAligner::Configuration>("AssemblyObjectAligner::Configuration"); //"After a type has been registered, you can create and destroy objects of that type dynamically at run-time"

  configuration_.reset();

  this->reset();
  this->reset_counter_numOfRotations();

  connect(this, SIGNAL(nextAlignmentStep(double, double, double)), this, SLOT(run_alignment(double, double, double)));

  connect(this, SIGNAL(motion_completed()), this, SLOT(launch_next_alignment_step()));

  NQLog("AssemblyObjectAligner", NQLog::Debug) << "constructed";
}

void AssemblyObjectAligner::Configuration::reset()
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

bool AssemblyObjectAligner::Configuration::is_valid() const
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
      NQLog("AssemblyObjectAligner::Configuration", NQLog::Warning) << "is_valid"
         << ": unexpected value for max. angle for 1-rotation (" << angle_max_dontIter << ")";
    }

    if(angle_max_complete <= 0.)
    {
      return false;
    }
    else if(angle_max_complete > 0.1)
    {
      NQLog("AssemblyObjectAligner::Configuration", NQLog::Warning) << "is_valid"
         << ": unexpected value for max. angle to validate alignment (" << angle_max_complete << ")";
    }
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

void AssemblyObjectAligner::reset_counter_numOfRotations()
{
  counter_numOfRotations_ = 0;
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
  this->reset();
  this->reset_counter_numOfRotations();

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
    if(counter_numOfRotations_ >= max_numOfRotations_)
    {
       NQLog("AssemblyObjectAligner", NQLog::Warning) << "\e[1;35mrun_alignment() : reached maximum number of rotations ("
          << max_numOfRotations_ << ") for the alignment procedure -- Stopping here !\e[0m";

       emit DBLogMessage("=== Reached maximum number of rotations (" + QString::number(counter_numOfRotations_) + ") for the alignment procedure. Stopping there !");

       NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
          << ": emitting signal \"execution_completed\"";

       this->reset();
       this->reset_counter_numOfRotations();

       emit execution_completed();

       return;
    }

    NQLog("AssemblyObjectAligner", NQLog::Message) << "\e[1;32m=== Starting alignment procedure\e[0m (iteration: " << counter_numOfRotations_ << ")";

    emit DBLogMessage("=== Starting alignment procedure (iteration: " + QString::number(counter_numOfRotations_) + ")" );

    NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

    if(this->configuration().use_autofocusing)
    {
      NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"autofocused_image_request\"";

      ++alignment_step_;

      emit autofocused_image_request();
    }
    else
    {
      NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"image_request\"";

      ++alignment_step_;

      emit image_request();
    }
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

    if(this->configuration().use_autofocusing)
    {
      NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"autofocused_image_request\"";

      ++alignment_step_;

      emit autofocused_image_request();
    }
    else
    {
      NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"image_request\"";

      ++alignment_step_;

      emit image_request();
    }
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

    // relative (X,Y) movement to reach the opposite marker (marker-1)
    const double dX = (posi_x1_ - motion_manager_->get_position_X());
    const double dY = (posi_y1_ - motion_manager_->get_position_Y());

    if(this->configuration().only_measure_angle) //If box "Measure Angle" is ticked --> Only measure angle, don't align motion stage
    {
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: mode = ONLY-MEASURE-ANGLE";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: object aligned with angle [deg] = " << obj_angle_deg_;
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      if(this->configuration().complete_at_position1) //If box "Go back to marker-1 position before completion" is ticked, continue routine (go to marker1, take image, terminate)
      {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: alignment completed, moving back to best-match position of PatRec #1";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

        NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
           << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, 0)\"";

        ++alignment_step_;

        this->move_relative(dX, dY, 0.0, 0.0);
      }
      else //Else, finish there and emit 'execution_completed' signal
      {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: ===> Alignment routine completed successfully";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

        NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
           << ": emitting signal \"execution_completed\"";

        this->reset();
        this->reset_counter_numOfRotations();

        emit execution_completed();
      }
    }
    else //(If 'Align object' box is ticked)
    {
      const double target_angle_deg = this->configuration().target_angle; //Angle between the line (between 2 markers) and the MS, in the MS ref. frame

      const double delta_angle_deg = (target_angle_deg - obj_angle_deg_); //Difference between current measured angle, and 'target angle' parameter

      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: mode = ALIGNMENT";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: object aligned with angle    [deg] = " << obj_angle_deg_;
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: target alignment angle       [deg] = " << target_angle_deg;
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: angular distance from target [deg] = " << delta_angle_deg;
      NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

      if(fabs(delta_angle_deg) > this->configuration().angle_max_complete) //If 'delta_angle_deg' exceeds the 'Max Distance Allowed From Target Angle To Validate Alignment' tolerance parameter (<-> the motion stage needs to be better aligned)
      {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: angle(object, target) > " << this->configuration().angle_max_complete << ", will apply a rotation";

        if(fabs(delta_angle_deg) <= this->configuration().angle_max_dontIter) //If 'delta_angle_deg' is smaller than 'Max Angle For Single Rotation [deg]' parameter -> rotate the motion stage accordingly, and restart the routine
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: angle(object, target) <= " << this->configuration().angle_max_dontIter << ", moving to target angle with 1 rotation";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_
             << "]: ===> will rotate by " << delta_angle_deg << " deg, and move back to best-match position of PatRec #1";

          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
             << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, " << delta_angle_deg << ")\"";

          this->reset();

          ++counter_numOfRotations_;

          this->move_relative(dX, dY, 0.0, delta_angle_deg);
        }
        else //Else, if exceeds the 'angle_max_dontIter' parameter, rotate motion stage by max. allowed value '+-angle_max_dontIter', and restart routine
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: angle(object, target) > " << this->configuration().angle_max_dontIter << " deg, large rotation required";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_
             << "]: ===> iterative procedure: will rotate by " << this->configuration().angle_max_dontIter << " deg, and move back to best-match position of PatRec #1";

          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          const double rot_deg = (delta_angle_deg <= 0) ? (-1.0 * this->configuration().angle_max_dontIter) : (+1.0 * this->configuration().angle_max_dontIter);

          NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
             << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, " << rot_deg << ")\"";

          this->reset();

          ++counter_numOfRotations_;

          this->move_relative(dX, dY, 0.0, rot_deg);
        }
      }
      else //If a sufficiently small angle value has been found
      {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: angle(object, target) < " << this->configuration().angle_max_complete << ", will NOT apply a rotation";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

        if(this->configuration().complete_at_position1) //If box "Go back to marker-1 position before completion" is ticked, continue routine (go to marker1, take image, terminate)
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: alignment completed, moving back to best-match position of PatRec #1";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
             << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, 0)\"";

          ++alignment_step_;

          this->move_relative(dX, dY, 0.0, 0.0);
        }
        else //Else stop there, emit 'execution_completed' signal
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: object aligned with angle [deg] = " << obj_angle_deg_;
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]: ===> Alignment routine completed successfully";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment: step [" << alignment_step_ << "]";

          NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
             << ": emitting signal \"execution_completed\"";

          this->reset();
          this->reset_counter_numOfRotations();

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

    if(this->configuration().use_autofocusing)
    {
      NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"autofocused_image_request\"";

      ++alignment_step_;

      emit autofocused_image_request();
    }
    else
    {
      NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment: step [" << alignment_step_ << "]"
         << ": emitting signal \"image_request\"";

      ++alignment_step_;

      emit image_request();
    }
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
    this->reset_counter_numOfRotations();

    emit execution_completed();
  }

  return;
}
