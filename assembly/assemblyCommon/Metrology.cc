/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2023 - The DESY CMS Group                  //
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

#include <Metrology.h>
#include <AssemblyUtilities.h>

#include <cmath>

Metrology::Metrology(const LStepExpressMotionManager* const motion_manager, QObject* parent)
 : QObject(parent)

 , motion_manager_(motion_manager)
 , motion_manager_enabled_(false)

 , moving_to_start_(false)

 , config_(nullptr)
{
  if(motion_manager_ == nullptr)
  {
    NQLog("Metrology", NQLog::Fatal) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    assembly::kill_application(tr("[Metrology]"), tr("Null pointer to LStepExpressMotionManager. Aborting Application."));
  }

  //When launching metrology routine, most params get read by MetrologyView, and copied into this->configuration_.
  //But other params are not related to the View (not editable in GUI), so must read their values manually here
  config_ = ApplicationConfig::instance(); //Access config file
  if(config_ == nullptr)
  {
    NQLog("Metrology", NQLog::Fatal)
       << "ApplicationConfig::instance() not initialized (null pointer), stopped constructor";

    assembly::kill_application(tr("[Metrology]"), tr("ApplicationConfig::instance() not initialized (null pointer), closing application"));
  }

  qRegisterMetaType<Metrology::Configuration>("Metrology::Configuration"); //"After a type has been registered, you can create and destroy objects of that type dynamically at run-time"

  configuration_.reset();

  this->reset();

  connect(this, SIGNAL(nextMetrologyStep(double, double, double)), this, SLOT(run_metrology(double, double, double)));

  connect(this, SIGNAL(motion_completed()), this, SLOT(launch_next_metrology_step()));

  NQLog("Metrology", NQLog::Debug) << "constructed";
}


void Metrology::Configuration::reset()
{
  complete_at_position1 = true;
  use_autofocusing = true;

  PatRec_PSP1_configuration.reset();
  PatRec_PSP2_configuration.reset();
  PatRec_PSS1_configuration.reset();
  PatRec_PSS2_configuration.reset();

}

bool Metrology::Configuration::is_valid() const
{

  if(PatRec_PSP1_configuration.is_valid() == false){ return false; }
  if(PatRec_PSP2_configuration.is_valid() == false){ return false; }
  if(PatRec_PSS1_configuration.is_valid() == false){ return false; }
  if(PatRec_PSS2_configuration.is_valid() == false){ return false; }

  return true;
}

void Metrology::reset()
{
  metrology_step_ = 0;

  clear_results();

  return;
}

void Metrology::update_configuration(const Metrology::Configuration& conf)
{
  if(conf.is_valid() == false)
  {
    NQLog("Metrology", NQLog::Critical) << "update_configuration"
       << ": invalid input Metrology::Configuration object, no action taken";

    return;
  }

  configuration_ = conf;

  NQLog("Metrology", NQLog::Spam) << "update_configuration"
     << ": emitting signal \"configuration_updated\"";

  emit configuration_updated();
}

// FIXME
void Metrology::redirect_image(const cv::Mat& img)
{
  if(metrology_step_ == 2)
  {
    NQLog("Metrology", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecOne\"";

    emit image_PatRecOne(img);
  }
  else if(metrology_step_ == 5)
  {
    NQLog("Metrology", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecTwo\"";

    emit image_PatRecTwo(img);
  }
  else if(metrology_step_ == 8)
  {
    NQLog("Metrology", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecThree\"";

    emit image_PatRecThree(img);
  }
  else if(metrology_step_ == 11)
  {
    NQLog("Metrology", NQLog::Spam) << "redirect_image"
       << ": emitting signal \"image_PatRecFour\"";

    emit image_PatRecFour(img);
  }

  return;
}

void Metrology::enable_motion_manager(const bool arg)
{
  if(arg == motion_manager_enabled_)
  {
    NQLog("Metrology", NQLog::Debug) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for Metrology already " << (arg ? "enabled" : "disabled") << ", no action taken";

    return;
  }

  if(arg)
  {
    connect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    connect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = true;

    NQLog("Metrology", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for Metrology enabled";
  }
  else
  {
    disconnect(this, SIGNAL(move_relative_request(double, double, double, double)), motion_manager_, SLOT(moveRelative(double, double, double, double)));
    disconnect(motion_manager_, SIGNAL(motion_finished()), this, SLOT(complete_motion()));

    motion_manager_enabled_ = false;

    NQLog("Metrology", NQLog::Spam) << "enable_motion_manager(" << arg << ")"
       << ": motion-manager for Metrology disabled";
  }

  return;
}

void Metrology::move_relative(const double x, const double y, const double z, const double a)
{
  this->connect_motion_manager();

  emit move_relative_request(x, y, z, a);
}

void Metrology::complete_motion()
{
  this->disconnect_motion_manager();

  NQLog("Metrology", NQLog::Spam) << "complete_motion"
     << ": emitting signal \"motion_completed\"";

  emit motion_completed();
}

void Metrology::move_to_start()
{
  NQLog("Metrology", NQLog::Spam) << "move_to_start";

  const double dX = config_->getValue<double>("parameters", "RefPointSensor_X") - motion_manager_->get_position_X();
  const double dY = config_->getValue<double>("parameters", "RefPointSensor_Y") - motion_manager_->get_position_Y();
  const double dZ = config_->getValue<double>("parameters", "RefPointSensor_Z") + config_->getValue<double>("parameters", "Thickness_Baseplate") + config_->getValue<double>("parameters", "Thickness_GlueLayer") - motion_manager_->get_position_Z();
  const double dA = config_->getValue<double>("parameters", "RefPointSensor_A") - motion_manager_->get_position_A();

  NQLog("Metrology", NQLog::Spam) << "move_to_start:"
     << ": emitting signal \"move_relative(" << dX << ", " << dY << ", " << dZ << ", " << dA << ")\"";

  moving_to_start_ = true;

  this->move_relative(dX, dY, dZ, dA);
}

void Metrology::launch_next_metrology_step()
{
  if(moving_to_start_) {
      moving_to_start_ = false;
      return;
  }

  NQLog("Metrology", NQLog::Spam) << "launch_next_metrology_step"
     << ": emitting signal \"nextMetrologyStep(0, 0, 0)\"";

  emit nextMetrologyStep(0.0, 0.0, 0.0);
}

void Metrology::execute()
{
  this->reset();
  this->run_metrology(0., 0., 0.);
}

void Metrology::run_metrology(const double patrec_dX, const double patrec_dY, const double patrec_angle)
{
  // Step #0:
  if(metrology_step_ == 0)
  {
      NQLog("Metrology", NQLog::Message) << "\e[1;32m=== Starting metrology\e[0m";

      emit DBLogMessage("=== Starting metrology" );

      NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";

      if(this->configuration().use_autofocusing)
      {
        NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
           << ": emitting signal \"autofocused_image_request\"";

        ++metrology_step_;

        emit autofocused_image_request();
      }
      else
      {
        NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
           << ": emitting signal \"image_request\"";

        ++metrology_step_;

        emit image_request();
      }
  }
  // Step #1: PatRec on marker-1 (PSP)
  else if(metrology_step_ == 1)
  {
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";

    NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
       << ": emitting signal \"PatRec_request\"";

    ++metrology_step_;

    emit PatRec_request(this->configuration().PatRec_PSP1_configuration);
  }
  // Step #2: move to marker-2 (PSP)
  else if(metrology_step_ == 2)
  {
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]"
       << ": determining best-match position of PatRec #1";

    // marker-1: position of PatRec best-match
    posi_PSp_x1_ = motion_manager_->get_position_X() + patrec_dX;
    posi_PSp_y1_ = motion_manager_->get_position_Y() + patrec_dY;
    posi_PSp_z1_ = motion_manager_->get_position_Z();

    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: motion-stage X = " << motion_manager_->get_position_X();
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: motion-stage Y = " << motion_manager_->get_position_Y();
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: PatRec dX = " << patrec_dX;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: PatRec dY = " << patrec_dY;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: x1-position = " << posi_PSp_x1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: y1-position = " << posi_PSp_y1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: z1-position = " << posi_PSp_z1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";

    // relative movement to reach the opposite marker
    const double obj_deltaX = config_->getValue<double>("main", "AssemblyObjectAlignerView_PSP_deltaX");
    const double obj_deltaY = config_->getValue<double>("main", "AssemblyObjectAlignerView_PSP_deltaY");

    // angle from marker's outer edge to best-match position in the camera ref-frame
    //   - the 90.0 deg offset corresponds to the angle spanned by the marker (L-shape)
    const double patrec_angle_full = (patrec_angle + 90.0);

    const double camera_offset_dA = config_->getValue<double>("parameters", "AngleOfCameraFrameInRefFrame_dA");

    double dX_1to2, dY_1to2;
    assembly::rotation2D_deg(dX_1to2, dY_1to2, (patrec_angle_full + camera_offset_dA), obj_deltaX, obj_deltaY);

    // combined relative movement
    const double dX = patrec_dX + dX_1to2;
    const double dY = patrec_dY + dY_1to2;

    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]"
       << ": moving to position for PatRec #2";

    NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
       << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, 0)\"";

    ++metrology_step_;

    this->move_relative(dX, dY, 0.0, 0.0);
  }
  // Step #3: request image on marker-2 (PSP)
  else if(metrology_step_ == 3)
  {
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]"
       << ": acquiring image for PatRec #2";

    if(this->configuration().use_autofocusing)
    {
      NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
         << ": emitting signal \"autofocused_image_request\"";

      ++metrology_step_;

      emit autofocused_image_request();
    }
    else
    {
      NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
         << ": emitting signal \"image_request\"";

      ++metrology_step_;

      emit image_request();
    }
  }
  // Step #4: PatRec on marker-2 (PSP)
  else if(metrology_step_ == 4)
  {
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";

    NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
       << ": emitting signal \"PatRec_request\"";

    ++metrology_step_;

    emit PatRec_request(this->configuration().PatRec_PSP2_configuration);
  }
  // Step #5: calculate angle and move to marker-1 (PSS)
  else if(metrology_step_ == 5)
  {
    posi_PSp_x2_ = motion_manager_->get_position_X() + patrec_dX;
    posi_PSp_y2_ = motion_manager_->get_position_Y() + patrec_dY;
    posi_PSp_z2_ = motion_manager_->get_position_Z();

    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(X1) = " << posi_PSp_x1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(Y1) = " << posi_PSp_y1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(Z1) = " << posi_PSp_z1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(X2) = " << posi_PSp_x2_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(Y2) = " << posi_PSp_y2_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(Z2) = " << posi_PSp_z2_;

    // measurement of object orientation
    if(posi_PSp_x2_ == posi_PSp_x1_)
    {
      PSp_angle_deg_ = (posi_PSp_y2_ == posi_PSp_y1_) ? 0. : ((posi_PSp_y2_ > posi_PSp_y1_) ? 90. : -90.);
    }
    else
    {
      const double obj_slope = ((posi_PSp_y2_ - posi_PSp_y1_)/(posi_PSp_x2_ - posi_PSp_x1_));

      PSp_angle_deg_ = atan(obj_slope) * (180.0/M_PI);
    }

    emit measured_angle(false, PSp_angle_deg_);

    // relative (X,Y) movement to reach the opposite PSp marker (marker-1) ...
    double dX = (posi_PSp_x1_ - motion_manager_->get_position_X());
    double dY = (posi_PSp_y1_ - motion_manager_->get_position_Y());

    // ... and then from PSP to PSS
    dX += config_->getValue<double>("parameters", "FromPSPRefPointToPSSRefPoint_dX");
    dY += config_->getValue<double>("parameters", "FromPSPRefPointToPSSRefPoint_dY");
    const double dZ = config_->getValue<double>("parameters", "Thickness_GlueLayer") * 2.
                    + config_->getValue<double>("parameters", "Thickness_Spacer")
                    + config_->getValue<double>("parameters", "Thickness_MPA")
                    + config_->getValue<double>("parameters", "Thickness_PSS");


    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]"
       << ": moving to position for PatRec #3";

    NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
       << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, 0)\"";

    ++metrology_step_;

    this->move_relative(dX, dY, dZ, 0.0);
  }
  // Step #6: request image
  else if(metrology_step_ == 6)
  {
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]"
       << ": acquiring image for PatRec #3";

    if(this->configuration().use_autofocusing)
    {
      NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
         << ": emitting signal \"autofocused_image_request\"";

      ++metrology_step_;

      emit autofocused_image_request();
    }
    else
    {
      NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
         << ": emitting signal \"image_request\"";

      ++metrology_step_;

      emit image_request();
    }
  }
  // Step #7: PatRec on marker-1 (PSS)
  else if(metrology_step_ == 7)
  {
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";

    NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
       << ": emitting signal \"PatRec_request\"";

    ++metrology_step_;

    emit PatRec_request(this->configuration().PatRec_PSS1_configuration);
  }
  // Step #8: move to marker-2 (PSS)
  else if(metrology_step_ == 8)
  {
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]"
       << ": determining best-match position of PatRec #4";

    // marker-1: position of PatRec best-match
    posi_PSs_x1_ = motion_manager_->get_position_X() + patrec_dX;
    posi_PSs_y1_ = motion_manager_->get_position_Y() + patrec_dY;
    posi_PSs_z1_ = motion_manager_->get_position_Z();

    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: motion-stage X = " << motion_manager_->get_position_X();
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: motion-stage Y = " << motion_manager_->get_position_Y();
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: PatRec dX = " << patrec_dX;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: PatRec dY = " << patrec_dY;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: x1-position = " << posi_PSs_x1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: y1-position = " << posi_PSs_y1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: z1-position = " << posi_PSs_z1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";

    // relative movement to reach the opposite marker
    const double obj_deltaX = config_->getValue<double>("main", "AssemblyObjectAlignerView_PSS_deltaX");
    const double obj_deltaY = config_->getValue<double>("main", "AssemblyObjectAlignerView_PSS_deltaY");

    // angle from marker's outer edge to best-match position in the camera ref-frame
    //   - the 90.0 deg offset corresponds to the angle spanned by the marker (L-shape)
    const double patrec_angle_full = (patrec_angle + 90.0);

    const double camera_offset_dA = config_->getValue<double>("parameters", "AngleOfCameraFrameInRefFrame_dA");

    double dX_1to2, dY_1to2;
    assembly::rotation2D_deg(dX_1to2, dY_1to2, (patrec_angle_full + camera_offset_dA), obj_deltaX, obj_deltaY);

    // combined relative movement
    const double dX = patrec_dX + dX_1to2;
    const double dY = patrec_dY + dY_1to2;

    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]"
       << ": moving to position for PatRec #4";

    NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
       << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, 0)\"";

    ++metrology_step_;

    this->move_relative(dX, dY, 0.0, 0.0);
  }
  // Step #9: request image on marker-2 (PSS)
  else if(metrology_step_ == 9)
  {
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]"
       << ": acquiring image for PatRec #4";

    if(this->configuration().use_autofocusing)
    {
      NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
         << ": emitting signal \"autofocused_image_request\"";

      ++metrology_step_;

      emit autofocused_image_request();
    }
    else
    {
      NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
         << ": emitting signal \"image_request\"";

      ++metrology_step_;

      emit image_request();
    }
  }
  // Step #10: PatRec on marker-2 (PSS)
  else if(metrology_step_ == 10)
  {
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";

    NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
       << ": emitting signal \"PatRec_request\"";

    ++metrology_step_;

    emit PatRec_request(this->configuration().PatRec_PSS2_configuration);
  }
  // Step #11: calculate angle and move to marker-1 (PSS)
  else if(metrology_step_ == 11)
  {
    posi_PSs_x2_ = motion_manager_->get_position_X() + patrec_dX;
    posi_PSs_y2_ = motion_manager_->get_position_Y() + patrec_dY;
    posi_PSs_z2_ = motion_manager_->get_position_Z();

    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(X1) = " << posi_PSs_x1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(Y1) = " << posi_PSs_y1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(Z1) = " << posi_PSs_z1_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(X2) = " << posi_PSs_x2_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(Y2) = " << posi_PSs_y2_;
    NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: position(Z2) = " << posi_PSs_z2_;

    // measurement of object orientation
    if(posi_PSs_x2_ == posi_PSs_x1_)
    {
      PSs_angle_deg_ = (posi_PSs_y2_ == posi_PSs_y1_) ? 0. : ((posi_PSs_y2_ > posi_PSs_y1_) ? 90. : -90.);
    }
    else
    {
      const double obj_slope = ((posi_PSs_y2_ - posi_PSs_y1_)/(posi_PSs_x2_ - posi_PSs_x1_));

      PSs_angle_deg_ = atan(obj_slope) * (180.0/M_PI);
    }

    emit measured_angle(true, PSs_angle_deg_);

    const double delta_x = (posi_PSs_x1_ - posi_PSp_x1_);
    const double delta_x_corr = delta_x - config_->getValue<double>("parameters", "FromPSPRefPointToPSSRefPoint_dX");
    const double delta_y = (posi_PSs_y1_ - posi_PSp_y1_);
    const double delta_y_corr = delta_y - config_->getValue<double>("parameters", "FromPSPRefPointToPSSRefPoint_dY");
    const double delta_z = (posi_PSs_z1_ + posi_PSs_z2_)/2. - (posi_PSp_z1_ + posi_PSp_z2_)/2.;
    const double delta_a_deg = PSs_angle_deg_ - PSp_angle_deg_;
    const double delta_a_urad = delta_a_deg * (M_PI/180.0);

    emit measured_results(delta_x, delta_x_corr, delta_y, delta_y_corr, delta_z, delta_a_deg, delta_a_urad);

    NQLog("Metrology", NQLog::Message) << "run_metrology: Finished with results:";
    NQLog("Metrology", NQLog::Message) << "run_metrology: Delta X (raw/corrected) [mm]:        \t" << delta_x << "\t" << delta_x_corr;
    NQLog("Metrology", NQLog::Message) << "run_metrology: Delta Y (raw/corrected) [mm]:        \t" << delta_y << "\t" << delta_y_corr;
    NQLog("Metrology", NQLog::Message, 4) << "run_metrology: Delta Z                 [mm]:        \t" << delta_z;
    NQLog("Metrology", NQLog::Message, 3) << "run_metrology: Angle between PSp & PSs [deg / urad]:\t" << delta_a_deg << "\t" << delta_a_urad;


    if(this->configuration().complete_at_position1) //If box "Go back to marker-1 position before completion" is ticked, continue routine (go to marker1, take image, terminate)
    {
      NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]: metrology completed, moving back to best-match position of PatRec #1";
      NQLog("Metrology", NQLog::Message) << "run_metrology: step [" << metrology_step_ << "]";

      ++metrology_step_;

      const double dX = (posi_PSp_x1_ - motion_manager_->get_position_X());
      const double dY = (posi_PSp_y1_ - motion_manager_->get_position_Y());
      const double dZ = (config_->getValue<double>("parameters", "RefPointSensor_Z") - motion_manager_->get_position_Z());

      NQLog("Metrology", NQLog::Spam) << "run_metrology: step [" << metrology_step_ << "]"
      << ": emitting signal \"move_relative(" << dX << ", " << dY << ", 0, 0)\"";

      this->move_relative(dX, dY, dZ, 0.0);
    }
    this->reset();

    QMessageBox* msgBox = new QMessageBox;
    msgBox->setInformativeText("Metrology routine completed successfully!\nSee the results in the Metrology tab.");

    msgBox->setStandardButtons(QMessageBox::Ok);

    int ret = msgBox->exec();

    emit execution_completed();
  }
}

void Metrology::clear_results()
{
    posi_PSp_x1_ = 0.;
    posi_PSp_y1_ = 0.;
    posi_PSp_z1_ = 0.;
    posi_PSp_x2_ = 0.;
    posi_PSp_y2_ = 0.;
    posi_PSp_z2_ = 0.;
    posi_PSs_x1_ = 0.;
    posi_PSs_y1_ = 0.;
    posi_PSs_z1_ = 0.;
    posi_PSs_x2_ = 0.;
    posi_PSs_y2_ = 0.;
    posi_PSs_z2_ = 0.;

    PSp_angle_deg_ = 0.;
    PSs_angle_deg_ = 0.;
}
