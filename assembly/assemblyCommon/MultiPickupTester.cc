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

#include <MultiPickupTester.h>
#include <nqlogger.h>

MultiPickupTester::MultiPickupTester(LStepExpressMotionManager* motion_manager, QObject* parent) :
  QObject(parent),
  motion_manager_(motion_manager)
{
  if(!motion_manager_)
  {
    NQLog("MultiPickupTester", NQLog::Critical) << "input error"
       << ": null pointer to LStepExpressMotionManager object, initialization stopped";

    return;
  }

  pickup_vacuum_ = 2;

  pickup_deltaZ_ = 20.;

  itera_counter_ = 0;

  mode_ = MultiPickupTester::None;
  conf_ = MultiPickupTester::Configuration();

  vacuum_on_   = false;
  pickup_done_ = false;
  picked_up_   = false;
}

MultiPickupTester::~MultiPickupTester()
{
}

void MultiPickupTester::execute(const MultiPickupTester::Configuration& conf)
{
  conf_ = conf;
}

void MultiPickupTester::start_measurement()
{
  mode_ = MultiPickupTester::measurement;

  const double dx = (conf_.measurement_X() - motion_manager_->get_position_X());
  const double dy = (conf_.measurement_Y() - motion_manager_->get_position_Y());
  const double dz = (conf_.measurement_Z() - motion_manager_->get_position_Z());

  NQLog("MultiPickupTester", NQLog::Debug) << "start_measurement"
     << ": emitting signal \"move_relative(" << dx << ", " << dy << ", " << dz << ", 0)\"";

  emit move_relative(dx, dy, dz, 0.);
}

void MultiPickupTester::finish_measurement(const int exit_code)
{
  if(exit_code != 0)
  {
    NQLog("MultiPickupTester", NQLog::Critical) << "finish_measurement"
       << ": measurement terminated with non-zero exit code (" << exit_code << "). Stopping test.";

    return;
  }

  ++itera_counter_;

  NQLog("MultiPickupTester", NQLog::Debug) << "finish_measurement"
     << ": emitting signal \"measurement_finished\"";

  emit measurement_finished();
}

void MultiPickupTester::start_pickup()
{
  if(itera_counter_ > conf_.iterations())
  {
    NQLog("MultiPickupTester", NQLog::Debug) << "start_pickup"
       << ": emitting signal \"test_finished\"";

    itera_counter_ = 0;

    mode_ = MultiPickupTester::None;
    conf_ = MultiPickupTester::Configuration();

    vacuum_on_   = false;
    pickup_done_ = false;
    picked_up_   = false;

    emit test_finished();

    return;
  }

  mode_ = MultiPickupTester::pickup;

  pickup_done_ = false;
  picked_up_   = false;
  vacuum_on_   = false;

  const double dx = (conf_.pickup_X() - motion_manager_->get_position_X());
  const double dy = (conf_.pickup_Y() - motion_manager_->get_position_Y());
  const double dz = (conf_.pickup_Z() - motion_manager_->get_position_Z());

  NQLog("MultiPickupTester", NQLog::Debug) << "start_pickup"
     << ": emitting signal \"move_relative(" << dx << ", " << dy << ", " << dz << ", 0)\"";

  emit move_relative(dx, dy, dz, 0.);
}

void MultiPickupTester::setup_next_step()
{
  if(itera_counter_ > conf_.iterations())
  {
    NQLog("MultiPickupTester", NQLog::Debug) << "setup_next_step"
       << ": emitting signal \"test_finished\"";

    itera_counter_ = 0;

    mode_ = MultiPickupTester::None;
    conf_ = MultiPickupTester::Configuration();

    vacuum_on_   = false;
    pickup_done_ = false;
    picked_up_   = false;

    emit test_finished();

    return;
  }
  else
  {
    if(mode_ == MultiPickupTester::measurement)
    {
      NQLog("MultiPickupTester", NQLog::Debug) << "setup_next_step"
         << ": emitting signal \"measurement_request\"";

      emit measurement_request();

      return;
    }
    else if(mode_ == MultiPickupTester::pickup)
    {
      if(pickup_done_)
      {
        if(picked_up_ == true)
        {
          NQLog("MultiPickupTester", NQLog::Critical) << "setup_next_step"
             << ": logic error: [mode=pickup] pickup completed, but picked_up switch still ON. Stopping test.";

          return;
        }
        else
        {
          if(vacuum_on_ == true)
          {
            vacuum_on_ = false;

            NQLog("MultiPickupTester", NQLog::Debug) << "setup_next_step"
               << ": emitting signal \"vacuum_toggle(" << pickup_vacuum_ << ")\"";

            emit vacuum_toggle(pickup_vacuum_);
          }
          else
          {
            NQLog("MultiPickupTester", NQLog::Debug) << "setup_next_step"
               << ": emitting signal \"pickup_finished\"";

            emit pickup_finished();
          }
        }
      }
      else
      {
        if(picked_up_ == false && vacuum_on_ == false)
        {
          vacuum_on_ = true;

          NQLog("MultiPickupTester", NQLog::Debug) << "setup_next_step"
             << ": emitting signal \"vacuum_toggle(" << pickup_vacuum_ << ")\"";

          emit vacuum_toggle(pickup_vacuum_);
        }
        else if(picked_up_ == false && vacuum_on_ == true)
        {
          picked_up_ = true;

          const double dz = +1.0 * pickup_deltaZ_;

          NQLog("MultiPickupTester", NQLog::Debug) << "setup_next_step"
             << ": emitting signal \"move_relative(0, 0, " << dz << ", 0)\"";

          emit move_relative(0., 0., dz, 0.);
        }
        else if(picked_up_ == true && vacuum_on_ == true)
        {
          picked_up_ = false;

          pickup_done_ = true;

          const double dz = -1.0 * pickup_deltaZ_;

          NQLog("MultiPickupTester", NQLog::Debug) << "setup_next_step"
             << ": emitting signal \"move_relative(0, 0, " << dz << ", 0)\"";

          emit move_relative(0., 0., dz, 0.);
        }
      }
    }
    else
    {
      NQLog("MultiPickupTester", NQLog::Critical) << "setup_next_step"
         << ": logic error: undefined value for MultiPickupTester mode. Stopping test.";

      return;
    }
  }

  return;
}
