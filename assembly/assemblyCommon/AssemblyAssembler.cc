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

  object_deltaX_ = config->getValue<double>("AssemblyAssembler_object_deltaX");
  object_deltaY_ = config->getValue<double>("AssemblyAssembler_object_deltaY");

  alignment_step = 0;

  NQLog("AssemblyAssembler", NQLog::Debug) << "constructed";
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

//!! void AssemblyAssembler::run_sandwitchassembly(double x_a, double y_a, double z_a , double x_b, double y_b, double z_b, double x_t, double y_t, double z_t)
//!! {
//!!     NQLog("AssemblyAssembler::run_sandwichassembly")
//!!         << " x_a = " << x_a << " y_a = " << y_a << " z_a = " << z_a
//!!         << " x_b = " << x_b << " y_b = " << y_b << " z_b = " << z_b
//!!         << " x_t = " << x_t << " y_t = " << y_t << " z_t = " << z_t;
//!! 
//!!     x_assembly = x_a;
//!!     y_assembly = y_a;
//!!     z_assembly = z_a;
//!! 
//!!     x_bottom = x_b;
//!!     y_bottom = y_b;
//!!     z_bottom = z_b;
//!! 
//!!     x_top = x_t;
//!!     y_top = y_t;
//!!     z_top = z_t;
//!! 
//!!     step = 0;
//!! 
//!!     // Parameters of assembly.
//!!     z_prepickup_distance =  20.00;
//!!     platform_rotation    = -90.00;
//!!     z_spacer_thickness   =   2.00;
//!!     z_sensor_thickness   =   0.30;
//!! 
//!!     emit nextStep();
//!! 
//!!     return;
//!! }

//!! void  AssemblyAssembler::fill_positionvectors(int stage, double x_pr, double y_pr, double theta_pr)
//!! {
//!!     // x_pr     = r->Gaus(0.0,  1.0);
//!!     // y_pr     = r->Gaus(0.0,  5.0);
//!!     // theta_pr = r->Gaus(0.0, 10.0);
//!! 
//!!     NQLog("AssemblyAssembler::fill_positionvectors") << "step = " << step << " x = "<< x_pr << " y = " << y_pr << " theta = " << theta_pr;
//!! 
//!!     if(step == 2)
//!!     {
//!!         xpre_vec.push_back(x_pr);
//!!         ypre_vec.push_back(y_pr);
//!!         thetapre_vec.push_back(theta_pr);
//!!     }
//!!     else if(step == 11){
//!! 
//!!         xpost_vec.push_back(x_pr);
//!!         ypost_vec.push_back(y_pr);
//!!         thetapost_vec.push_back(theta_pr);
//!!     }
//!! 
//!!     emit nextStep();
//!! }

//void  AssemblyAssembler::run_alignment(){
//    NQLog("AssemblyAssembler::run_alignment")<<  " tolerance = " << tolerance <<  " max iterations = "<< max_iterations << endl;

// Rought manual alignment with ref marker on platform

//2. Apply rough angular correction:
//a. Run PatRec and detect X1,Y1,Theta
//b. put marker in centre of fied of view (need to accurate conversion of pixels to mm )
//c. iterate:
//    - rotate platform to allign (some small fraction of required rotation)
//    - put marker in centre of fied of view
//    repeat until target alignment reached

//emit acquireImage();
//3. Apply fine angular correction:
// a. Go to far-away corner, Run PatRec and detect X2,Y2
// b. Calculate residual angular mis-alignment from (Y2-Y1)/(X2-X1)
//c. iterate:
//    - rotate platform to allign (some small fraction of required rotation)
//    - put marker in centre of fied of view
//    repeat until target alignment reached


//4. Confirm alignment
//   - return to starting corner and conform detected X doesn't change

//  emit nextAlignmentStep();

//}

//!! void AssemblyAssembler::process_step(){
//!! 
//!!     NQLog("AssemblyAssembler::") << "process_step";
//!! 
//!!     if(step == 0)
//!!     {
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!       step++;
//!! 
//!!       // Step 0: Go to measurement position for ref corner (needs to be manually pre-determined)
//!!       emit moveAbsolute(x_assembly, y_assembly, z_assembly + z_prepickup_distance, 0.0);
//!!     }
//!!     else if(step == 1)
//!!     {
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!     }
//!!     else if(step == 2)
//!!     {
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!       step++;
//!! 
//!!       // Step 2: Turn on vacuum
//!!       emit toggleVacuum(1); // vacuum line 1, pick up tool
//!!     }
//!! 
//!!     /*
//!!     else if (step == 3){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 3: Go to top part pre-pick up position
//!!         emit moveAbsolute(x_top, y_top, (z_top + z_prepickup_distance), 0.0);
//!!         //emit nextStep();
//!!         
//!!     }else if (step == 4){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 4: Go to assembly pre-pick up position
//!!         emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_prepickup_distance), 0.0);
//!!         //emit nextStep();
//!!         
//!!     }else if (step == 5){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 5: Go to assembly pick up position
//!!         emit moveAbsolute(x_assembly, y_assembly, z_assembly, 0.0);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 6){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 6 : Timer for fast gluing!!!
//!!         emit nextStep();
//!!         
//!!     }else if (step == 7){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!!         //emit nextStep();
//!! // Step 7: Release vacuum under spacers
//!!         emit toggleVacuum(2);   //vacuum line 2, spacers pick up tool
//!! 
//!!         
//!!     }else if (step == 8){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 8: Go to assembly pre-pick up position
//!!         emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_prepickup_distance), 0.0);
//!!         //emit nextStep();
//!!         
//!!         
//!!     }else if (step == 9){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 9: Go to top pre-pick up position
//!!         emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness + z_prepickup_distance), 0.0);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 10){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 10: Go to top pick up position
//!!         emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness), 0.0);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 11){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!!         //emit nextStep();
//!! // Step 11: Release vacuum, pick up tool
//!!         emit toggleVacuum(1);   //vacuum line 1, pick up tool
//!! 
//!!     }else if (step == 12){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 9: Go to top part pre-pick up position
//!!         emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness + z_prepickup_distance), 0.0);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 13){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 13: Go to bottom part pre-pick up position
//!!         emit moveAbsolute(x_bottom, y_bottom, (z_bottom + z_prepickup_distance), 0.0);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 14){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 14: Go to bottom part pick up position
//!!         emit moveAbsolute(x_bottom, y_bottom, z_bottom, 0.0);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 15){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!!         //emit nextStep();
//!! // Step 151: Turn on. Pick up bottom part
//!!         emit toggleVacuum(1);   //vacuum line 1, pick up tool
//!! 
//!!     }else if (step == 16){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 16: Go to bottom part pre-pick up position
//!!         emit moveAbsolute(x_bottom, y_bottom, (z_bottom + z_prepickup_distance), 0.0);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 17){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 17: Go to assembly rotated(!) pre-pick up position
//!!         emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_prepickup_distance), platform_rotation);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 18){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 18: Go to assembly rotated(!) pick up position
//!!         emit moveAbsolute(x_assembly, y_assembly, z_assembly, platform_rotation);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 19){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!!         //emit nextStep();
//!! // Step 11: Release vacuum, pick up tool
//!!         emit toggleVacuum(1);   //vacuum line 1, pick up tool
//!! 
//!!     }else if (step == 20){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 20: Go to assembly rotated(!) pre-pick up position
//!!         emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_prepickup_distance), platform_rotation);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 21){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 21: Go to top pre-pick up position
//!!         emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness + z_prepickup_distance), platform_rotation);
//!!         //emit nextStep();
//!! 
//!!    
//!!     }else if (step == 22){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 22: Go to top pick up position
//!!         emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness), platform_rotation);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 23){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!!         //emit nextStep();
//!! // Step 23: Release vacuum, pick up tool
//!!         emit toggleVacuum(1);   //vacuum line 1, pick up tool
//!! 
//!!     }else if (step == 24){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 24: Go to top part pre-pick up position
//!!         emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness + z_prepickup_distance), platform_rotation);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 25){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 25: Go to assembly rotated(!) pre-pick up position
//!!         emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_spacer_thickness + z_sensor_thickness + z_prepickup_distance), platform_rotation);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 26){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 26: Go to assembly rotated(!) pick up position
//!!         emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_spacer_thickness + z_sensor_thickness), platform_rotation);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 27){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 27 : Timer for fast gluing!!! 
//!!         emit nextStep();
//!! 
//!!     }else if (step == 28){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!!         //emit nextStep();
//!! // Step 28: Release vacuum, pick up tool
//!!         emit toggleVacuum(1);   //vacuum line 1, pick up tool
//!! 
//!!     }else if (step == 29){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 29: Go to assembly rotated(!) pre-pick up position
//!!         emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_spacer_thickness + z_sensor_thickness - z_prepickup_distance), platform_rotation);
//!!         //emit nextStep();
//!! 
//!!     }else if (step == 30){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!         step++;
//!! // Step 30: Module done. Please, carefully put it to the shell for 24 hours to let main glue to be cured. Thanks for choosing automated assembly for assembling your module.
//!!         emit moveAbsolute(0.0, 0.0, 0.0, platform_rotation + 180.00);
//!!         //emit nextStep();
//!! 
//!!     }else if  (step == 11){
//!! 
//!!       NQLog("AssemblyAssembler:: step == ") << step;
//!!       step = 0;
//!!       //emit nextStep();
//!! 
//!!     }
//!! 
//!!     */
//!! }

void AssemblyAssembler::launch_next_alignment_step()
{
  NQLog("AssemblyAssembler", NQLog::Spam) << "launch_next_alignment_step"
     << ": emitting signal \"nextAlignmentStep(1, 0, 0, 0)\"";

  emit nextAlignmentStep(1, 0.0, 0.0, 0.0);
}

double x1_pos, x2_pos, y1_pos, y2_pos, slope, deg_orient, orient;

void AssemblyAssembler::run_alignment(int /* stage */, double x_pr, double y_pr, double theta_pr)
{
    // Rought manual alignment with ref marker on platform
    //1. Go to 'start' position (manually?)
    //2. Apply rough angular correction:
    //a. Run PatRec and detect X1,Y1,Theta
    //b. put marker in centre of fied of view (need to accurate conversion of pixels to mm )
    //c. iterate:
    //    - rotate platform to allign (some small fraction of required rotation)
    //    - put marker in centre of fied of view
    //    repeat until target alignment reached
    
    //    double mm_per_pixel_x = 5.632/2560.0;
    //double mm_per_pixel_y = 4.224/1920.0;

    const double mm_per_pixel_x = 0.0012;
    const double mm_per_pixel_y = 0.0012;

    //double target_x = ( y_pr - (1317.0/2.0) ) * mm_per_pixel_x;
    //double target_y = ( x_pr - (1964.0/2.0) ) * mm_per_pixel_y;

    //coordinate system is flipped
    double target_x = ( y_pr - (1920.0/2.0) ) * mm_per_pixel_x;
    double target_y = ( x_pr - (2560.0/2.0) ) * mm_per_pixel_y;

    double target_theta = theta_pr;

    if(alignment_step == 0)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

        ++alignment_step;

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        emit acquireImage();
    }

    //3. Apply fine angular correction:
    // a. Go to far-away corner, Run PatRec and detect X2,Y2
    // b. Calculate residual angular mis-alignment from (Y2-Y1)/(X2-X1)
    // c. iterate:
    //    - rotate platform to allign (some small fraction of required rotation)
    //    - put marker in centre of fied of view
    //    repeat until target alignment reached   
    else if(alignment_step == 1)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": centering camera on PatRec best-match position in first corner";

        ++alignment_step;

        if((fabs(target_x) > 0.005) || (fabs(target_y)  > 0.005))
        {
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << (alignment_step-1) << "]"
             << ": moving to PatRec best-match position in first corner";

          this->moveRelative(target_x, target_y, 0.0, 0.0);
        }
    }
    else if(alignment_step == 2)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": re-detecting first corner after centering";

        x1_pos = motion_manager_->get_position_X();
        y1_pos = motion_manager_->get_position_Y();

        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: x1-position = " << x1_pos;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: y1-position = " << y1_pos;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

        ++alignment_step;

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        emit acquireImage();
    } 
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

        const double target_deg = (target_theta * (3.14159/180.0));

        const double COS = cos(target_deg);
        const double SIN = sin(target_deg);

        const double rel_dx =  COS * markglas_deltaX + SIN * markglas_deltaY;
        const double rel_dy = -SIN * markglas_deltaX + COS * markglas_deltaY;
        const double rel_dz = 0.0; // z = -0.20 for sensor with glue, z=0 for clean sensor

        ++alignment_step;

        this->moveRelative(rel_dx, rel_dy, rel_dz, 0.0);
    }
    else if(alignment_step == 4)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": acquiring image of second corner";

        ++alignment_step;

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        emit acquireImage();
    }
    else if(alignment_step == 5)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": centering camera on PatRec best-match position in second corner";

        ++alignment_step;

        if((fabs(target_x) > 0.005) || (fabs(target_y) > 0.005))
        {
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << (alignment_step-1) << "]"
             << ": moving to PatRec best-match position in second corner";

          this->moveRelative(target_x, target_y, 0.0, 0.0);
        }
    }
    else if(alignment_step == 6)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": re-detecting second corner after centering";

        double x2_pos = motion_manager_->get_position_X();
        double y2_pos = motion_manager_->get_position_Y();

        slope = (y2_pos - y1_pos)/(x2_pos - x1_pos);

        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: x1-position = " << x1_pos;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: y1-position = " << y1_pos;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: x2-position = " << x2_pos;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: y2-position = " << y2_pos;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: sensor is aligned with slope = " << slope;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

        // Define orientation to be reached 
        const double wanted_slope = -0.50;

        const double a1 = atan(wanted_slope) * (180.0/3.14159);
        const double a2 = atan       (slope) * (180.0/3.14159);

        const double delta_theta_deg = (a1 - a2);

        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: target angle = " << a1;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: sensor is aligned with angle = " << a2;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: need to rotate by " << delta_theta_deg << " degrees ";
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        ++alignment_step;

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        emit acquireImage(); 
    }
    else if(alignment_step == 7)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": moving back to the first corner";

        const double markglas_deltaX = object_deltaX_;
        const double markglas_deltaY = object_deltaY_;

        const double target_deg = (target_theta * (3.14159/180.0));

        const double COS = cos(target_deg);
        const double SIN = sin(target_deg);

        const double rel_dx =  COS * markglas_deltaX + SIN * markglas_deltaY;
        const double rel_dy = -SIN * markglas_deltaX + COS * markglas_deltaY;
        const double rel_dz = 0.0; // z = -0.20 for sensor with glue, z=0 for clean sensor

        ++alignment_step;

        this->moveRelative(rel_dx, rel_dy, rel_dz, 0.0);
    }
    else if(alignment_step == 8)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": detecting first corner for the 2nd time";

        ++alignment_step;

        NQLog("AssemblyAssembler", NQLog::Spam) << "run_alignment step [" << alignment_step << "]"
           << ": emitting signal \"acquireImage\"";

        emit acquireImage();
    }
    else if(alignment_step == 9)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]"
           << ": centering camera on PatRec best-match position in first corner (for the 2nd time)";

        ++alignment_step;

        if((fabs(target_x) > 0.005) || (fabs(target_y) > 0.005))
        {
          NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << (alignment_step-1) << "]"
             << ": moving to PatRec best-match position in first corner (for the 2nd time)";

          this->moveRelative(target_x, target_y, 0.0, 0.0);
        }
    }
    else if(alignment_step == 10)
    {
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

        // Define orientation to be reached 
        const double wanted_slope = -0.50;

        const double a1 = atan(wanted_slope) * (180.0/3.14159);
        const double a2 = atan       (slope) * (180.0/3.14159);

        const double delta_theta_deg = (a1 - a2);

        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: target angle = " << a1;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: sensor is aligned with angle = " << a2;
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: need to rotate by " << delta_theta_deg << " degrees ";
        NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

        if(fabs(delta_theta_deg) > 0.01)
        {
            NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: delta-theta > " << 0.01 << ", action required";

            if(fabs(delta_theta_deg) < 0.5)
            {
                NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
                NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: delta-theta < " << 0.5 << ", moving to target orientation in one step";
                NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

                alignment_step = 0;

                this->moveRelative(0.0, 0.0, 0.0, delta_theta_deg);
            }
            else
            {
                NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
                NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]: delta-theta >=" << 0.5 << ", requires large rotation => iterative procedure";
                NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

                alignment_step = 0;

                const double rot_deg = (delta_theta_deg <= 0) ? -0.5 : 0.5;

                this->moveRelative(0.0, 0.0, 0.0, rot_deg);
            }
        }
        else
        {
            NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";
            NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "] ---> In Position, Alignment completed successfully";
            NQLog("AssemblyAssembler", NQLog::Message) << "run_alignment step [" << alignment_step << "]";

            alignment_step = 0;
        }
    }

    return;
}

//!! void AssemblyAssembler::run_scan(double range, int steps)
//!! {    
//!!     NQLog("AssemblyAssembler::scan") << range << ",  " <<steps;
//!! 
//!!     // steps = 10;
//!!     nTotalImages = steps;
//!!        
//!!     double step_distance = range/steps;
//!!     int nSteps = 0;
//!!     
//!!     x_vals.clear();
//!!     y_vals.clear();
//!! 
//!!     nAcquiredImages = 1;
//!! 
//!! //    emit getImage();
//!!     this->moveRelative(0.0, 0.0, 1.0, 0.0);
//!! }

//!! void AssemblyAssembler::write_image(cv::Mat newImage, cv::Rect marker_rect)
//!! {
//!!     NQLog("AssemblyAssembler") << "write_image()";
//!!     QDateTime local(QDateTime::currentDateTime());
//!!     QString local_str = local.toString();
//!!     QString filename = QString("ZScan_%1.png").arg(local_str);
//!!     filename = filename.simplified();
//!!     filename.replace(" ", "");
//!! 
//!!     cv::Point tl = marker_rect.tl();
//!!     cv::Point br = marker_rect.br();
//!! 
//!! //    rectangle(newImage, rectangle, cv::Scalar(255,0,0), 2, 8, 0);
//!! //    rectangle(newImage, rectangle, cv::Scalar(255,0,0), 2, 8, 0);
//!!     
//!!     rectangle(newImage, tl, br, cv::Scalar(255,0,0), 2, 8, 0);
//!! 
//!!     cv::imwrite(filename.toStdString(), newImage);
//!!     emit updateScanImage(newImage);
//!! 
//!!     double variance  = this->imageVariance(newImage, marker_rect);
//!!     double x = nAcquiredImages;
//!!     double current_z = motion_manager_->get_position_Z();
//!!     
//!!     x_vals.push_back(current_z);
//!!     y_vals.push_back(variance);
//!! 
//!!     //check global image counter
//!!     if (nAcquiredImages <= nTotalImages){
//!!         std::cout <<"n acquired images = "<< nAcquiredImages<<"  nTotal images = "<< nTotalImages  << std::endl;
//!!         nAcquiredImages++; 
//!!         this->moveRelative(0.0,0.0,-0.1,0.0);
//!!     } else{
//!!         
//!!         int points  =  y_vals.size();
//!!         TGraph * gr = new TGraph(points);
//!!         TCanvas *  canvas = new TCanvas();
//!!         
//!!         double variance_max, z_at_peak =0;
//!!         
//!!         //fitting the peak (Gaussian?) would improve precision a little here
//!!         for (unsigned long  i =0; i < y_vals.size(); i++){
//!!             gr->SetPoint(i, x_vals[i], y_vals[i]);
//!!             if (y_vals[i] > variance_max){
//!!                 variance_max = y_vals[i];
//!!                 z_at_peak = x_vals[i];
//!!             }
//!!         }
//!!         
//!!         gr->Draw("AC*");
//!!         const std::string img_name = "variance.png";
//!!         canvas->SaveAs(img_name.c_str());
//!!     
//!!         emit make_graph(img_name);
//!! 
//!!       //emit some signal with measured marker Z position
//!!         emit updateText(z_at_peak);
//!!     }
//!! }
//!! 
//!! double AssemblyAssembler::imageVariance(cv::Mat img_input, cv::Rect rectangle)
//!! {
//!!     cv::Mat img_copy = img_input.clone();
//!!     cv::Mat roi(img_copy, rectangle);
//!! 
//!!     // Convert color image to greyscale (GS)
//!!     cv::Mat img_copy_gs(img_copy.size(), img_copy.type());
//!! //    cvtColor(img_copy, img_copy_gs, CV_BGR2GRAY);
//!! 
//!!     // Apply laplacian function to GS image
//!!     cv::Mat img_laplace;
//!!     Laplacian(img_copy, img_laplace, CV_64F);
//!! 
//!!     // Calculate standard deviation of laplace image
//!!     cv::Scalar mean, std_dev;
//!!     meanStdDev(img_laplace, mean, std_dev, cv::Mat());
//!! 
//!!     std::cout << "Variance: " <<std_dev.val[0]*std_dev.val[0] << std::endl;
//!! 
//!!     return std_dev.val[0]*std_dev.val[0];
//!! }
