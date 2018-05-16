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

  // offset angle to derive (X,Y) from angle determined by PatRec
  // (this offset derives from the orientation of the sensor in the template image)
  angle_PatRec_offset_ = config->getValue<double>("AssemblyObjectAligner_angle_PatRec_offset");

  configuration_.reset();

  this->reset();

  connect(this, SIGNAL(nextAlignmentStep(int, double, double, double)), this, SLOT(run_alignment(int, double, double, double)));

  connect(this, SIGNAL(motion_completed()), this, SLOT(launch_next_alignment_step()));

  NQLog("AssemblyObjectAligner", NQLog::Debug) << "constructed";
}

void AssemblyObjectAligner::Configuration::reset()
{
  object_deltaX = -999.;
  object_deltaY = -999.;

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
    connect(this, SIGNAL(move_relative(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    connect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = true;

    NQLog("AssemblyObjectAligner", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblyObjectAligner enabled";
  }
  else
  {
    disconnect(this, SIGNAL(move_relative(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    disconnect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = false;

    NQLog("AssemblyObjectAligner", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblyObjectAligner disabled";
  }

  return;
}

void AssemblyObjectAligner::moveRelative(const double x, const double y, const double z, const double a)
{
  this->connect_motion_manager();

  emit move_relative(x, y, z, a);
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
     << ": emitting signal \"nextAlignmentStep(1, 0, 0, 0)\"";

  emit nextAlignmentStep(1, 0.0, 0.0, 0.0);
}

void AssemblyObjectAligner::execute()
{
  alignment_step_ = 0;

  this->run_alignment(1, 0., 0., 0.);
}

void AssemblyObjectAligner::run_alignment(int /* stage */, double x_pr, double y_pr, double angle_pr)
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

    const double mm_per_pixel_x = 0.0012;
    const double mm_per_pixel_y = 0.0012;

    // distance (mm) between camera position and PatRec best-match position
    const double patrec_dX = ( y_pr - (1920.0/2.0) ) * mm_per_pixel_x;
    const double patrec_dY = ( x_pr - (2560.0/2.0) ) * mm_per_pixel_y;

    const double patrec_angle = (angle_pr + angle_PatRec_offset_);

    // Step #0: PatRec on first marker
    if(alignment_step_ == 0)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";

        NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment step [" << alignment_step_ << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step_;

        emit acquireImage();
    }
    // Step #1: center image, move camera to (X,Y) result from PatRec
    else if(alignment_step_ == 1)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
           << ": centering camera on PatRec best-match position in first corner";

        if((fabs(patrec_dX) > 0.005) || (fabs(patrec_dY)  > 0.005))
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
             << ": moving to PatRec best-match position in first corner";

          ++alignment_step_;

          this->moveRelative(patrec_dX, patrec_dY, 0.0, 0.0);
        }
        else
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
             << ": motion-stage already in position, emitting signal \"motion_completed\"";

          ++alignment_step_;

          emit motion_completed();
        }
    }
    // Step #2: re-run PatRec on 1st marker after centering
    else if(alignment_step_ == 2)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
           << ": re-detecting first corner after centering";

        posi_x1_ = motion_manager_->get_position_X();
        posi_y1_ = motion_manager_->get_position_Y();

        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: x1-position = " << posi_x1_;
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: y1-position = " << posi_y1_;
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";

        NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment step [" << alignment_step_ << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step_;

        emit acquireImage();
    } 
    // Step #3: move to 2nd marker
    else if(alignment_step_ == 3)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
           << ": moving to second corner";

        // silicon dummy sensor 47.4 x 99.75 with 0.7 correction in y_3
        // glass sensor with painted corners 98.5, 48.0
        //
        // effective length/width depends on sample and marker size/positions
        // 24.5 and 15.0 refer to the samples made from gluing scrap Si structures
        // to glass dummies

        const double markglas_deltaX = this->configuration().object_deltaX;
        const double markglas_deltaY = this->configuration().object_deltaY;

        const double target_deg = (patrec_angle * (M_PI/180.0));

        const double COS = cos(target_deg);
        const double SIN = sin(target_deg);

        const double rel_dx =  COS * markglas_deltaX + SIN * markglas_deltaY;
        const double rel_dy = -SIN * markglas_deltaX + COS * markglas_deltaY;
        const double rel_dz = 0.0; // z = -0.20 for sensor with glue, z=0 for clean sensor

        ++alignment_step_;

        this->moveRelative(rel_dx, rel_dy, rel_dz, 0.0);
    }
    // Step #4: run PatRec on 2nd marker
    else if(alignment_step_ == 4)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
           << ": acquiring image of second corner";

        NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment step [" << alignment_step_ << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step_;

        emit acquireImage();
    }
    // Step #5: center image, move camera to (X,Y) result from PatRec
    else if(alignment_step_ == 5)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
           << ": centering camera on PatRec best-match position in second corner";

        if((fabs(patrec_dX) > 0.005) || (fabs(patrec_dY) > 0.005))
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
             << ": moving to PatRec best-match position in second corner";

          ++alignment_step_;

          this->moveRelative(patrec_dX, patrec_dY, 0.0, 0.0);
        }
        else
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
             << ": motion-stage already in position, emitting signal \"motion_completed\"";

          ++alignment_step_;

          emit motion_completed();
        }
    }
    // Step #6: re-run PatRec on 2nd marker after centering
    else if(alignment_step_ == 6)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
           << ": re-detecting second corner after centering";

        posi_x2_ = motion_manager_->get_position_X();
        posi_y2_ = motion_manager_->get_position_Y();

        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: position(X1) = " << posi_x1_;
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: position(Y1) = " << posi_y1_;
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: position(X2) = " << posi_x2_;
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: position(Y2) = " << posi_y2_;

        const double target_angle_deg = this->configuration().target_angle;

        if(posi_x2_ == posi_x1_)
        {
          obj_angle_deg_ = (posi_y2_ == posi_y1_) ? 0. : ((posi_y2_ > posi_y1_) ? 90. : -90.);
        }
        else
        {
          const double obj_slope = ((posi_y2_ - posi_y1_)/(posi_x2_ - posi_x1_));

          obj_angle_deg_ = atan(obj_slope) * (180.0/M_PI);
        }

        const double object_angle_deg = obj_angle_deg_;

        const double delta_angle_deg = (target_angle_deg - object_angle_deg);

        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: target alignment angle       [deg] = " << target_angle_deg;
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: object aligned with angle    [deg] = " << object_angle_deg;
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: angular distance from target [deg] = " << delta_angle_deg;
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";

        NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment step [" << alignment_step_ << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step_;

        emit acquireImage();
    }
    // Step #7: move back to 1st marker
    else if(alignment_step_ == 7)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
           << ": moving back to the first corner";

        const double markglas_deltaX = this->configuration().object_deltaX;
        const double markglas_deltaY = this->configuration().object_deltaY;

        const double target_deg = (patrec_angle * (M_PI/180.0));

        const double COS = cos(target_deg);
        const double SIN = sin(target_deg);

        const double rel_dx =  COS * markglas_deltaX + SIN * markglas_deltaY;
        const double rel_dy = -SIN * markglas_deltaX + COS * markglas_deltaY;
        const double rel_dz = 0.0; // z = -0.20 for sensor with glue, z=0 for clean sensor

        ++alignment_step_;

        this->moveRelative(rel_dx, rel_dy, rel_dz, 0.0);
    }
    // Step #8: re-detecting 1st marker
    else if(alignment_step_ == 8)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
           << ": detecting first corner for the 2nd time";

        NQLog("AssemblyObjectAligner", NQLog::Spam) << "run_alignment step [" << alignment_step_ << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step_;

        emit acquireImage();
    }
    // Step #9: centering camera on PatRec result for 1st marker
    else if(alignment_step_ == 9)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
           << ": centering camera on PatRec best-match position in first corner (for the 2nd time)";

        if((fabs(patrec_dX) > 0.005) || (fabs(patrec_dY) > 0.005))
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
             << ": moving to PatRec best-match position in first corner (for the 2nd time)";

          ++alignment_step_;

          this->moveRelative(patrec_dX, patrec_dY, 0.0, 0.0);
        }
        else
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]"
             << ": motion-stage already in position, emitting signal \"motion_completed\"";

          ++alignment_step_;

          emit motion_completed();
        }
    }
    // Step #10: check alignment
    else if(alignment_step_ == 10)
    {
        NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";

        const double object_angle_deg = obj_angle_deg_;

        if(this->configuration().only_measure_angle)
        {
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: mode = ONLY-MEASURE-ANGLE";
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: object aligned with angle [deg] = " << object_angle_deg;
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";

          this->reset();

          emit measured_angle(object_angle_deg);

          emit execution_completed();
        }
        else
        {
          const double target_angle_deg = this->configuration().target_angle;

          const double delta_angle_deg = (target_angle_deg - object_angle_deg);

          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: target alignment angle       [deg] = " << target_angle_deg;
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: object aligned with angle    [deg] = " << object_angle_deg;
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: angular distance from target [deg] = " << delta_angle_deg;
          NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";

          if(fabs(delta_angle_deg) > angle_max_complete_)
          {
            NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: angle(object, target) > " << angle_max_complete_ << ", will apply a rotation";

            if(fabs(delta_angle_deg) <= angle_max_dontIter_)
            {
              NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";
              NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: angle(object, target) <= " << angle_max_dontIter_ << ", moving to target angle with 1 rotation";
              NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";

              this->reset();

              this->moveRelative(0.0, 0.0, 0.0, delta_angle_deg);
            }
            else
            {
              NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";
              NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: angle(object, target) > " << angle_max_dontIter_ << " deg, large rotation required";
              NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]: ===> iterative procedure (will now rotate by " << angle_max_dontIter_ << " deg)";
              NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";

              this->reset();

              const double rot_deg = (delta_angle_deg <= 0) ? (-1.0 * angle_max_dontIter_) : (+1.0 * angle_max_dontIter_);

              this->moveRelative(0.0, 0.0, 0.0, rot_deg);
            }
          }
          else
          {
            NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";
            NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "] ===> In Position, Alignment completed successfully";
            NQLog("AssemblyObjectAligner", NQLog::Message) << "run_alignment step [" << alignment_step_ << "]";

            this->reset();

            emit measured_angle(object_angle_deg);

            emit execution_completed();
          }
        }
    }

    return;
}
