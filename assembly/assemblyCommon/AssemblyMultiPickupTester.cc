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

#include <AssemblyMultiPickupTester.h>

AssemblyMultiPickupTester::AssemblyMultiPickupTester(const LStepExpressMotionManager* motion_manager, QObject* parent)
 : QObject(parent)

 , motion_manager_(motion_manager)
 , motion_manager_enabled_(false)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblyMultiPickupTester", NQLog::Critical) << "input error"
       << ": null pointer to LStepExpressMotionManager object, initialization stopped";

    return;
  }

  ApplicationConfig* config = ApplicationConfig::instance();
  if(config == nullptr)
  {
    NQLog("AssemblyMultiPickupTester", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    return;
  }

  pickup_vacuum_ = config->getValue<int>("Vacuum_PickupTool");
  pickup_basepl_ = config->getValue<int>("Vacuum_Baseplate");

  pickup_deltaZ_ = config->getValue<double>("AssemblyMultiPickupTester_pickup_deltaZ", 20.);

  use_vacuumBP_  = config->getValue<bool>("AssemblyMultiPickupTester_useBaseplateVacuum");

  this->reset();

  NQLog("AssemblyMultiPickupTester", NQLog::Debug) << "constructed";
}

AssemblyMultiPickupTester::~AssemblyMultiPickupTester()
{
}

void AssemblyMultiPickupTester::enable_motion_manager(const bool arg)
{
  if(arg == motion_manager_enabled_)
  {
    NQLog("AssemblyMultiPickupTester", NQLog::Debug) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for multi-pickup test already " << (arg ? "enabled" : "disabled") << ", no action taken";

    return;
  }

  if(arg)
  {
    connect(this, SIGNAL(move_relative(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    connect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(setup_next_step()));

    motion_manager_enabled_ = true;

    NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for multi-pickup test enabled";
  }
  else
  {
    disconnect(this, SIGNAL(move_relative(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    disconnect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(setup_next_step()));

    motion_manager_enabled_ = false;

    NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for multi-pickup test disabled";
  }

  return;
}

void AssemblyMultiPickupTester::reset()
{
  itera_counter_ = 0;

  mode_ = AssemblyMultiPickupTester::Mode_None;
  move_ = AssemblyMultiPickupTester::Movement_None;
  conf_ = AssemblyMultiPickupTester::Configuration();

  vacuum_on_   = false;
  vacuumBP_on_ = true;
  pickup_done_ = false;
  picked_up_   = false;

  this->disconnect_motion_manager();
}

void AssemblyMultiPickupTester::start_measurement()
{
  NQLog("AssemblyMultiPickupTester", NQLog::Message) << "start_measurement: -------------------------------";
  NQLog("AssemblyMultiPickupTester", NQLog::Message) << "start_measurement: initializing measurement [#" << itera_counter_ << "]";
  NQLog("AssemblyMultiPickupTester", NQLog::Message) << "start_measurement: -------------------------------";

  mode_ = AssemblyMultiPickupTester::Mode_measurement;

  const double dz = (conf_.measurement_Z() - motion_manager_->get_position_Z());

  NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "start_measurement"
     << ": emitting signal \"move_relative(0, 0, " << dz << ", 0)\"";

  move_ = AssemblyMultiPickupTester::Movement_Z;

  this->connect_motion_manager();

  emit move_relative(0., 0., dz, 0.);
}

void AssemblyMultiPickupTester::finish_measurement(const int exit_code)
{
  QMutexLocker ml(&mutex_);

  if(exit_code != 0)
  {
    NQLog("AssemblyMultiPickupTester", NQLog::Critical) << "finish_measurement"
       << ": measurement terminated with non-zero exit code (" << exit_code << "). Stopping test.";

    NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "finish_measurement"
       << ": emitting signal \"test_finished\"";

    this->reset();

    emit test_finished();

    return;
  }

  ++itera_counter_;

  NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "finish_measurement"
     << ": emitting signal \"measurement_finished\"";

  emit measurement_finished();
}

void AssemblyMultiPickupTester::start_pickup()
{
  if(itera_counter_ > conf_.iterations())
  {
    NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "start_pickup"
       << ": emitting signal \"test_finished\"";

    this->reset();

    emit test_finished();

    return;
  }

  mode_ = AssemblyMultiPickupTester::Mode_pickup;

  vacuum_on_   = false;
  vacuumBP_on_ = true;
  pickup_done_ = false;
  picked_up_   = false;

  const double dx = (conf_.pickup_X() - motion_manager_->get_position_X());
  const double dy = (conf_.pickup_Y() - motion_manager_->get_position_Y());

  NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "start_pickup"
     << ": emitting signal \"move_relative(" << dx << ", " << dy << ", 0, 0)\"";

  move_ = AssemblyMultiPickupTester::Movement_XY;

  this->connect_motion_manager();

  emit move_relative(dx, dy, 0., 0.);
}

void AssemblyMultiPickupTester::setup_next_step()
{
  if(itera_counter_ > conf_.iterations())
  {
    NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
       << ": emitting signal \"test_finished\"";

    this->reset();

    emit test_finished();

    return;
  }
  else
  {
    if(mode_ == AssemblyMultiPickupTester::Mode_measurement)
    {
      if(move_ == AssemblyMultiPickupTester::Movement_Z)
      {
        const double dx = (conf_.measurement_X() - motion_manager_->get_position_X());
        const double dy = (conf_.measurement_Y() - motion_manager_->get_position_Y());

        NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
           << ": emitting signal \"move_relative(" << dx << ", " << dy << ", 0, 0)\"";

        move_ = AssemblyMultiPickupTester::Movement_XY;

        this->connect_motion_manager();

        emit move_relative(dx, dy, 0., 0.);
      }
      else if(move_ == AssemblyMultiPickupTester::Movement_XY)
      {
        move_ = AssemblyMultiPickupTester::Movement_None;

        this->disconnect_motion_manager();

        NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
           << ": emitting signal \"measurement_request\"";

        emit measurement_request();
      }
      else
      {
        NQLog("AssemblyMultiPickupTester", NQLog::Fatal) << "setup_next_step"
           << ": logic error (Mode=measurement, Movement!=[XY,Z]), terminating test";

        NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
           << ": emitting signal \"test_finished\"";

        this->reset();

        emit test_finished();

        return;
      }
    }
    else if(mode_ == AssemblyMultiPickupTester::Mode_pickup)
    {
      if(pickup_done_ == true)
      {
        if(picked_up_ == true)
        {
          NQLog("AssemblyMultiPickupTester", NQLog::Critical) << "setup_next_step"
             << ": logic error: [mode=pickup] pickup completed, but picked_up switch still ON. Stopping test.";

          NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
             << ": emitting signal \"test_finished\"";

          this->reset();

          emit test_finished();

          return;
        }
        else
        {
          if(use_vacuumBP_ && vacuumBP_on_ == false)
          {
            vacuumBP_on_ = true;

            NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
               << ": emitting signal \"vacuum_toggle(" << pickup_basepl_ << ")\"";

            emit vacuum_toggle(pickup_basepl_);
          }
          else if(vacuum_on_ == true)
          {
            vacuum_on_ = false;

            NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
               << ": emitting signal \"vacuum_toggle(" << pickup_vacuum_ << ")\"";

            emit vacuum_toggle(pickup_vacuum_);
          }
          else
          {
            NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
               << ": emitting signal \"pickup_finished\"";

            move_ = AssemblyMultiPickupTester::Movement_None;

            this->disconnect_motion_manager();

            emit pickup_finished();
          }
        }
      }
      else
      {
        if(picked_up_ == false && (vacuum_on_ == false || (use_vacuumBP_ && vacuumBP_on_ == true)))
        {
          if(move_ == AssemblyMultiPickupTester::Movement_XY)
          {
            const double dz = (conf_.pickup_Z() - motion_manager_->get_position_Z());

            NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
               << ": emitting signal \"move_relative(0, 0, " << dz << ", 0)\"";

            move_ = AssemblyMultiPickupTester::Movement_Z;

            this->connect_motion_manager();

            emit move_relative(0., 0., dz, 0.);
          }
          else if(move_ == AssemblyMultiPickupTester::Movement_Z)
          {
            move_ = AssemblyMultiPickupTester::Movement_None;

            this->disconnect_motion_manager();

            vacuum_on_ = true;

            NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
               << ": emitting signal \"vacuum_toggle(" << pickup_vacuum_ << ")\"";

            emit vacuum_toggle(pickup_vacuum_);
          }
          else if(move_ == AssemblyMultiPickupTester::Movement_None)
          {
            this->disconnect_motion_manager();

            vacuumBP_on_ = false;

            NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
               << ": emitting signal \"vacuum_toggle(" << pickup_basepl_ << ")\"";

            emit vacuum_toggle(pickup_basepl_);
          }
          else
          {
            NQLog("AssemblyMultiPickupTester", NQLog::Fatal) << "setup_next_step"
               << ": logic error (Mode=pickup, Movement!=[XY,Z]), terminating test";

            NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
               << ": emitting signal \"test_finished\"";

            this->reset();

            emit test_finished();

            return;
          }
        }
        else if(picked_up_ == false && vacuum_on_ == true)
        {
          picked_up_ = true;

          const double dz = +1.0 * pickup_deltaZ_;

          NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
             << ": emitting signal \"move_relative(0, 0, " << dz << ", 0)\"";

          move_ = AssemblyMultiPickupTester::Movement_Z;

          this->connect_motion_manager();

          emit move_relative(0., 0., dz, 0.);
        }
        else if(picked_up_ == true && vacuum_on_ == true)
        {
          picked_up_ = false;

          pickup_done_ = true;

          const double dz = -1.0 * pickup_deltaZ_;

          NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
             << ": emitting signal \"move_relative(0, 0, " << dz << ", 0)\"";

          move_ = AssemblyMultiPickupTester::Movement_Z;

          this->connect_motion_manager();

          emit move_relative(0., 0., dz, 0.);
        }
      }
    }
    else
    {
      NQLog("AssemblyMultiPickupTester", NQLog::Critical) << "setup_next_step"
         << ": logic error: undefined value for AssemblyMultiPickupTester mode. Stopping test.";

      NQLog("AssemblyMultiPickupTester", NQLog::Spam) << "setup_next_step"
         << ": emitting signal \"test_finished\"";

      this->reset();

      emit test_finished();

      return;
    }
  }

  return;
}
