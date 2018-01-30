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

#include <AssemblyAssembler.h>
#include <ApplicationConfig.h>
#include <nqlogger.h>

#include <cmath>

AssemblyAssembler::AssemblyAssembler(const LStepExpressMotionManager* motion_manager, QObject* parent) :
  QObject(parent),
  motion_manager_(motion_manager),
  motion_manager_enabled_(false)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblyAssembler", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  ApplicationConfig* config = ApplicationConfig::instance();
  if(config == nullptr)
  {
    NQLog("AssemblyAssembler", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    return;
  }

  object_deltaX_    = 0.;
  object_deltaY_    = 0.;
  target_angle_deg_ = 0.;

  only_measure_ang_ = false;

  // maximum angular difference acceptable not to trigger iterative procedure for alignment
  angle_max_dontIter_ = config->getValue<double>("AssemblyAssembler_angle_max_dontIter");

  // maximum angular difference acceptable to declare alignment procedure finished
  angle_max_complete_ = config->getValue<double>("AssemblyAssembler_angle_max_complete");

  this->reset();

  NQLog("AssemblyAssembler", NQLog::Debug) << "constructed";
}

void AssemblyAssembler::reset()
{
  alignment_step = 0;

  posi_x1_ = 0.;
  posi_y1_ = 0.;
  posi_x2_ = 0.;
  posi_y2_ = 0.;

  obj_angle_deg_ = 0.;

  return;
}

void AssemblyAssembler::enable_motion_manager(const bool arg)
{
  if(arg == motion_manager_enabled_)
  {
    NQLog("AssemblyAssembler", NQLog::Debug) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblyAssembler already " << (arg ? "enabled" : "disabled") << ", no action taken";

    return;
  }

  if(arg)
  {
    connect(this, SIGNAL(move_relative(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    connect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(finish_motion()));

    motion_manager_enabled_ = true;

    NQLog("AssemblyAssembler", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblyAssembler enabled";
  }
  else
  {
    disconnect(this, SIGNAL(move_relative(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    disconnect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(finish_motion()));

    motion_manager_enabled_ = false;

    NQLog("AssemblyAssembler", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for AssemblyAssembler disabled";
  }

  return;
}

void AssemblyAssembler::moveRelative(const double x, const double y, const double z, const double a)
{
  this->connect_motion_manager();

  emit move_relative(x, y, z, a);
}

void AssemblyAssembler::finish_motion()
{
  this->disconnect_motion_manager();

  NQLog("AssemblyAssembler", NQLog::Spam) << "finish_motion"
     << ": emitting signal \"motion_finished\"";

  emit motion_finished();
}

void AssemblyAssembler::launch_next_alignment_step()
{
  NQLog("AssemblyAssembler", NQLog::Spam) << "launch_next_alignment_step"
     << ": emitting signal \"nextAlignmentStep(1, 0, 0, 0)\"";

  emit nextAlignmentStep(1, 0.0, 0.0, 0.0);
}

void AssemblyAssembler::start_alignment(const double obj_deltaX, const double obj_deltaY)
{
  object_deltaX_ = obj_deltaX;
  object_deltaY_ = obj_deltaY;

  only_measure_ang_ = true;

  alignment_step = 0;

  this->run_alignment(1, 0., 0., 0.);
}

void AssemblyAssembler::start_alignment(const double obj_deltaX, const double obj_deltaY, const double ang_target)
{
  if((-90. <= ang_target) && (ang_target <= 90.))
  {
    object_deltaX_ = obj_deltaX;
    object_deltaY_ = obj_deltaY;

    target_angle_deg_ = ang_target;

    only_measure_ang_ = false;
  }
  else
  {
    NQLog("AssemblyAssembler", NQLog::Spam) << "start_alignment"
       << ": invalid value for target angle (" << ang_target << " not in [-90,90]), alignment not initiated";

    return;
  }

  alignment_step = 0;

  this->run_alignment(1, 0., 0., 0.);
}

void AssemblyAssembler::run_alignment(int /* stage */, double x_pr, double y_pr, double angle_pr)
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

    const double patrec_angle = angle_pr;

    // Step #0: PatRec on first marker
    if(alignment_step == 0)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step;

        emit acquireImage();
    }
    // Step #1: center image, move camera to (X,Y) result from PatRec
    else if(alignment_step == 1)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": centering camera on PatRec best-match position in first corner";

        ++alignment_step;

        if((fabs(patrec_dX) > 0.005) || (fabs(patrec_dY)  > 0.005))
        {
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << (alignment_step-1) << "]"
             << ": moving to PatRec best-match position in first corner";

          this->moveRelative(patrec_dX, patrec_dY, 0.0, 0.0);
        }
    }
    // Step #2: re-run PatRec on 1st marker after centering
    else if(alignment_step == 2)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": re-detecting first corner after centering";

        posi_x1_ = motion_manager_->get_position_X();
        posi_y1_ = motion_manager_->get_position_Y();

        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: x1-position = " << posi_x1_;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: y1-position = " << posi_y1_;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step;

        emit acquireImage();
    } 
    // Step #3: move to 2nd marker
    else if(alignment_step == 3)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": moving to second corner";

        // silicon dummy sensor 47.4 x 99.75 with 0.7 correction in y_3
        // glass sensor with painted corners 98.5, 48.0
        //
        // effective length/width depends on sample and marker size/positions
        // 24.5 and 15.0 refer to the samples made from gluing scrap Si structures
        // to glass dummies

        const double markglas_deltaX = object_deltaX_;
        const double markglas_deltaY = object_deltaY_;

        const double target_deg = (patrec_angle * (M_PI/180.0));

        const double COS = cos(target_deg);
        const double SIN = sin(target_deg);

        const double rel_dx =  COS * markglas_deltaX + SIN * markglas_deltaY;
        const double rel_dy = -SIN * markglas_deltaX + COS * markglas_deltaY;
        const double rel_dz = 0.0; // z = -0.20 for sensor with glue, z=0 for clean sensor

        ++alignment_step;

        this->moveRelative(rel_dx, rel_dy, rel_dz, 0.0);
    }
    // Step #4: run PatRec on 2nd marker
    else if(alignment_step == 4)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": acquiring image of second corner";

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step;

        emit acquireImage();
    }
    // Step #5: center image, move camera to (X,Y) result from PatRec
    else if(alignment_step == 5)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": centering camera on PatRec best-match position in second corner";

        ++alignment_step;

        if((fabs(patrec_dX) > 0.005) || (fabs(patrec_dY) > 0.005))
        {
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << (alignment_step-1) << "]"
             << ": moving to PatRec best-match position in second corner";

          this->moveRelative(patrec_dX, patrec_dY, 0.0, 0.0);
        }
    }
    // Step #6: re-run PatRec on 2nd marker after centering
    else if(alignment_step == 6)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": re-detecting second corner after centering";

        posi_x2_ = motion_manager_->get_position_X();
        posi_y2_ = motion_manager_->get_position_Y();

        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: position(X1) = " << posi_x1_;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: position(Y1) = " << posi_y1_;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: position(X2) = " << posi_x2_;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: position(Y2) = " << posi_y2_;

        const double target_angle_deg = target_angle_deg_;

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

        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: target alignment angle       [deg] = " << target_angle_deg;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: object aligned with angle    [deg] = " << object_angle_deg;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: angular distance from target [deg] = " << delta_angle_deg;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step;

        emit acquireImage();
    }
    // Step #7: move back to 1st marker
    else if(alignment_step == 7)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": moving back to the first corner";

        const double markglas_deltaX = object_deltaX_;
        const double markglas_deltaY = object_deltaY_;

        const double target_deg = (patrec_angle * (M_PI/180.0));

        const double COS = cos(target_deg);
        const double SIN = sin(target_deg);

        const double rel_dx =  COS * markglas_deltaX + SIN * markglas_deltaY;
        const double rel_dy = -SIN * markglas_deltaX + COS * markglas_deltaY;
        const double rel_dz = 0.0; // z = -0.20 for sensor with glue, z=0 for clean sensor

        ++alignment_step;

        this->moveRelative(rel_dx, rel_dy, rel_dz, 0.0);
    }
    // Step #8: re-detecting 1st marker
    else if(alignment_step == 8)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": detecting first corner for the 2nd time";

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step;

        emit acquireImage();
    }
    // Step #9: centering camera on PatRec result for 1st marker
    else if(alignment_step == 9)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": centering camera on PatRec best-match position in first corner (for the 2nd time)";

        ++alignment_step;

        if((fabs(patrec_dX) > 0.005) || (fabs(patrec_dY) > 0.005))
        {
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << (alignment_step-1) << "]"
             << ": moving to PatRec best-match position in first corner (for the 2nd time)";

          this->moveRelative(patrec_dX, patrec_dY, 0.0, 0.0);
        }
    }
    // Step #10: check alignment
    else if(alignment_step == 10)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

        const double object_angle_deg = obj_angle_deg_;

        if(only_measure_ang_)
        {
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: mode = ONLY-MEASURE-ANGLE";
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: object aligned with angle [deg] = " << object_angle_deg;
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

          this->reset();

          emit object_angle(object_angle_deg);

          emit alignment_finished();
        }
        else
        {
          const double target_angle_deg = target_angle_deg_;

          const double delta_angle_deg = (target_angle_deg - object_angle_deg);

          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: target alignment angle       [deg] = " << target_angle_deg;
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: object aligned with angle    [deg] = " << object_angle_deg;
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: angular distance from target [deg] = " << delta_angle_deg;
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

          if(fabs(delta_angle_deg) > angle_max_complete_)
          {
            NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: angle(object, target) > " << angle_max_complete_ << ", will apply a rotation";

            if(fabs(delta_angle_deg) <= angle_max_dontIter_)
            {
              NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
              NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: angle(object, target) <= " << angle_max_dontIter_ << ", moving to target angle with 1 rotation";
              NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

              this->reset();

              this->moveRelative(0.0, 0.0, 0.0, delta_angle_deg);
            }
            else
            {
              NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
              NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: angle(object, target) > " << angle_max_dontIter_ << " deg, large rotation required";
              NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: ===> iterative procedure (will now rotate by " << angle_max_dontIter_ << " deg)";
              NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

              this->reset();

              const double rot_deg = (delta_angle_deg <= 0) ? (-1.0 * angle_max_dontIter_) : (+1.0 * angle_max_dontIter_);

              this->moveRelative(0.0, 0.0, 0.0, rot_deg);
            }
          }
          else
          {
            NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
            NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "] ===> In Position, Alignment completed successfully";
            NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

            this->reset();

            emit object_angle(object_angle_deg);

            emit alignment_finished();
          }
        }
    }

    return;
}
