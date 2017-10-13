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

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <QFormLayout>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>
#include <QApplication>
#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <TCanvas.h>
#include <TGraph.h>

#include "AssemblyAssembler.h"

using namespace cv;

AssemblyAssembler::AssemblyAssembler(LStepExpressModel* lStepExpressModel) : QObject(), lStepExpressModel_(lStepExpressModel)
{
    NQLog("AssemblyAssembler::AssemblyAssembler");

    motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
}

void  AssemblyAssembler::run_sandwitchassembly(double x_a, double y_a, double z_a , double x_b, double y_b, double z_b, double x_t, double y_t, double z_t)
{
    NQLog("AssemblyAssembler::run_sandwichassembly")
        << " x_a = " << x_a << " y_a = " << y_a << " z_a = " << z_a
        << " x_b = " << x_b << " y_b = " << y_b << " z_b = " << z_b
        << " x_t = " << x_t << " y_t = " << y_t << " z_t = " << z_t;

    x_assembly = x_a;
    y_assembly = y_a;
    z_assembly = z_a;

    x_bottom = x_b;
    y_bottom = y_b;
    z_bottom = z_b;

    x_top = x_t;
    y_top = y_t;
    z_top = z_t;

    step = 0;

    // Parameters of assembly.
    z_prepickup_distance = 20.00;
    platform_rotation = -90.00;
    z_spacer_thickness = 2.00;
    z_sensor_thickness = 0.30;

    emit nextStep();

    return;
}

void  AssemblyAssembler::fill_positionvectors(int stage, double x_pr, double y_pr, double theta_pr)
{
    // x_pr     = r->Gaus(0.0,  1.0);
    // y_pr     = r->Gaus(0.0,  5.0);
    // theta_pr = r->Gaus(0.0, 10.0);

    NQLog("AssemblyScanner::fill_positionvectors") << "step = " << step << " x = "<< x_pr << " y = " << y_pr << " theta = " << theta_pr;

    if(step == 2){

        xpre_vec.push_back(x_pr);
        ypre_vec.push_back(y_pr);
        thetapre_vec.push_back(theta_pr);
    }
    else if(step == 11){

        xpost_vec.push_back(x_pr);
        ypost_vec.push_back(y_pr);
        thetapost_vec.push_back(theta_pr);
    }

    emit nextStep();
}

//void  AssemblyScanner::run_alignment(){
//    NQLog("AssemblyScanner::run_alignment")<<  " tolerance = " << tolerance <<  " max iterations = "<< max_iterations << endl;

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

void  AssemblyAssembler::process_step(){

  NQLog("AssemblyAssembler::") << "process_step";
    
    if (step == 0){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
        //Step 0: Go to measurement position for ref corner (needs to be manually pre-determined)
        emit moveAbsolute(x_assembly, y_assembly, z_assembly + z_prepickup_distance, 0.0);
        //emit nextStep();
    }
    else if  (step == 1){

      NQLog("AssemblyAssembler:: step == ") << step;
      
    
    }else if (step == 2){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;

        //Step 2: Turn on vacuum
        emit toggleVacuum(1);   //vacuum line 1, pick up tool
    }
    

    
    /*
    else if (step == 3){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
// Step 3: Go to top part pre-pick up position
        emit moveAbsolute(x_top, y_top, (z_top + z_prepickup_distance), 0.0);
        //emit nextStep();
        
    }else if (step == 4){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 4: Go to assembly pre-pick up position
        emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_prepickup_distance), 0.0);
        //emit nextStep();
        
    }else if (step == 5){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 5: Go to assembly pick up position
        emit moveAbsolute(x_assembly, y_assembly, z_assembly, 0.0);
        //emit nextStep();
	
    }else if (step == 6){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 6 : Timer for fast gluing!!! 
	emit nextStep();
        
    }else if (step == 7){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
        //emit nextStep();
// Step 7: Release vacuum under spacers
        emit toggleVacuum(2);   //vacuum line 2, spacers pick up tool

        
    }else if (step == 8){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 8: Go to assembly pre-pick up position
        emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_prepickup_distance), 0.0);
        //emit nextStep();
        
        
    }else if (step == 9){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 9: Go to top pre-pick up position
        emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness + z_prepickup_distance), 0.0);
        //emit nextStep();

    }else if (step == 10){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 10: Go to top pick up position
        emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness), 0.0);
        //emit nextStep();

    }else if (step == 11){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
        //emit nextStep();
// Step 11: Release vacuum, pick up tool
        emit toggleVacuum(1);   //vacuum line 1, pick up tool

    }else if (step == 12){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 9: Go to top part pre-pick up position
        emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness + z_prepickup_distance), 0.0);
        //emit nextStep();

    }else if (step == 13){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 13: Go to bottom part pre-pick up position
        emit moveAbsolute(x_bottom, y_bottom, (z_bottom + z_prepickup_distance), 0.0);
        //emit nextStep();

    }else if (step == 14){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 14: Go to bottom part pick up position
        emit moveAbsolute(x_bottom, y_bottom, z_bottom, 0.0);
        //emit nextStep();

    }else if (step == 15){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
        //emit nextStep();
// Step 151: Turn on. Pick up bottom part
        emit toggleVacuum(1);   //vacuum line 1, pick up tool
	
    }else if (step == 16){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 16: Go to bottom part pre-pick up position
        emit moveAbsolute(x_bottom, y_bottom, (z_bottom + z_prepickup_distance), 0.0);
        //emit nextStep();

    }else if (step == 17){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 17: Go to assembly rotated(!) pre-pick up position
        emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_prepickup_distance), platform_rotation);
        //emit nextStep();

    }else if (step == 18){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 18: Go to assembly rotated(!) pick up position
        emit moveAbsolute(x_assembly, y_assembly, z_assembly, platform_rotation);
        //emit nextStep();
	
    }else if (step == 19){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
        //emit nextStep();
// Step 11: Release vacuum, pick up tool
        emit toggleVacuum(1);   //vacuum line 1, pick up tool

    }else if (step == 20){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 20: Go to assembly rotated(!) pre-pick up position
        emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_prepickup_distance), platform_rotation);
        //emit nextStep();
	
    }else if (step == 21){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 21: Go to top pre-pick up position
        emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness + z_prepickup_distance), platform_rotation);
        //emit nextStep();

   
    }else if (step == 22){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 22: Go to top pick up position
        emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness), platform_rotation);
        //emit nextStep();

    }else if (step == 23){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
        //emit nextStep();
// Step 23: Release vacuum, pick up tool
        emit toggleVacuum(1);   //vacuum line 1, pick up tool

    }else if (step == 24){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 24: Go to top part pre-pick up position
        emit moveAbsolute(x_top, y_top, (z_top + z_spacer_thickness + z_prepickup_distance), platform_rotation);
        //emit nextStep();

    }else if (step == 25){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 25: Go to assembly rotated(!) pre-pick up position
        emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_spacer_thickness + z_sensor_thickness + z_prepickup_distance), platform_rotation);
        //emit nextStep();

    }else if (step == 26){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 26: Go to assembly rotated(!) pick up position
        emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_spacer_thickness + z_sensor_thickness), platform_rotation);
        //emit nextStep();

    }else if (step == 27){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 27 : Timer for fast gluing!!! 
	emit nextStep();

    }else if (step == 28){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
        //emit nextStep();
// Step 28: Release vacuum, pick up tool
        emit toggleVacuum(1);   //vacuum line 1, pick up tool
	
    }else if (step == 29){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 29: Go to assembly rotated(!) pre-pick up position
        emit moveAbsolute(x_assembly, y_assembly, (z_assembly + z_spacer_thickness + z_sensor_thickness - z_prepickup_distance), platform_rotation);
        //emit nextStep();

    }else if (step == 30){

      NQLog("AssemblyAssembler:: step == ") << step;
        step++;
// Step 30: Module done. Please, carefully put it to the shell for 24 hours to let main glue to be cured. Thanks for choosing automated assembly for assembling your module.
        emit moveAbsolute(0.0, 0.0, 0.0, platform_rotation + 180.00);
        //emit nextStep();
    
	
    }else if  (step == 11){

      NQLog("AssemblyAssembler:: step == ") << step;
      step = 0;
      //emit nextStep();
     
    }

    */
    
}

void AssemblyAssembler::launch_next_alignment_step()
{
    emit nextAlignmentStep(1, 0.0, 0.0, 0.0);
}

double x1_pos, x2_pos, y1_pos, y2_pos, slope, deg_orient, orient;

void AssemblyAssembler::run_alignment(int stage, double x_pr, double y_pr, double theta_pr){

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

    double mm_per_pixel_x = 0.0012;
    double mm_per_pixel_y = 0.0012;


    if (theta_pr >= 175.0 && theta_pr <= 185.0){
      x_pr = 2560.0 - x_pr;
      y_pr = 1920.0 - y_pr;
    }

    
    //double target_x = ( y_pr - (1317/2.0) ) * mm_per_pixel_x;
    //double target_y = ( x_pr - (1964.0/2.0) ) * mm_per_pixel_y;

    //coordinate system is flipped
    double target_x = ( y_pr - (1920.0/2.0) ) * mm_per_pixel_x;
    double target_y = ( x_pr - (2560.0/2.0) ) * mm_per_pixel_y;
    double target_theta = theta_pr;
    double X1, Y1, X2, Y2 = 0.0;
 
    std::cout <<"   "<<std::endl;
    std::cout <<" " <<std::endl;
    std::cout <<"   "<<std::endl;
    std::cout <<" "  <<std::endl;
    std::cout <<" " <<std::endl;
    std::cout <<"  " <<std::endl;

    if (alignment_step == 0){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        alignment_step++;
        emit acquireImage();
    }
        //3. Apply fine angular correction:
        // a. Go to far-away corner, Run PatRec and detect X2,Y2
        // b. Calculate residual angular mis-alignment from (Y2-Y1)/(X2-X1)
        //c. iterate:
        //    - rotate platform to allign (some small fraction of required rotation)
        //    - put marker in centre of fied of view
        //    repeat until target alignment reached
       
    else if (alignment_step == 1){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        alignment_step++;
        if ( ( fabs(target_x)  > 0.005) || (  fabs(target_y)  > 0.005)  ){
          NQLog("AssemblyAssembler:: moving to  ") << target_x <<",  "<< target_y <<"  target theta " <<  target_theta ;
            emit moveRelative(target_x, target_y, 0.0, 0.0);
        }
      }
    
    else if (alignment_step == 2){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        alignment_step++;
		x1_pos = lStepExpressModel_ -> getPosition(0); 	
		y1_pos = lStepExpressModel_ -> getPosition(1); 	
		std::cout << " !!!!! " << std::endl;
		std::cout << " xposition = " << x1_pos << "  yposition = " << y1_pos << std::endl;
		std::cout << " !!!!! " << std::endl;
		emit acquireImage();
    } 



    else if (alignment_step == 3){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        NQLog("AssemblyAssembler::First corner aligned");	        
		alignment_step++;

		//silicon dummy sensor 47.4 x 99.75 with 0.7 correction in y_3
		// glass sensor with painted corners 98.5, 48.0

		//effective length/width depends on sample and marker size/positions
		//24.5 and 15.0 refer to the samples made from gluing scrap Si structures
		// to glass dummies

        double target_x_1 = (-1.0)*cos((target_theta*3.14)/180.0)*24.5;
        double target_y_1 = (-1.0)*sin((target_theta*3.14)/180.0)*24.5;
        
        double target_y_2 = (1.0)*cos((target_theta*3.14)/180.0)*15.0;  
        double target_x_2 = (1.0)*sin((target_theta*3.14)/180.0)*15.0;

        double target_x_3 = target_x_1 + target_x_2; 
        double target_y_3 = -target_y_1 + target_y_2 + 0.5;

	//not clear why minus sign and 0.5 is needed for y_3

        //NQLog("AssemblyAssembler::Going to second corner by moving: ") << target_x_2 <<", "   <<  target_y_2;
        
        emit moveRelative(target_x_3, target_y_3, +0.0, 0.0);  // z = -0.20 for sensor with glue, z=0 for clean sensor
    }

    else if (alignment_step == 4){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        NQLog("AssemblyAssembler::Detecting second corner");
        alignment_step++;
        emit acquireImage();
        
    } 
    
	
    else if (alignment_step == 5){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        NQLog("AssemblyAssembler::centering second corner");
        alignment_step++;
		if ( ( fabs(target_x)  > 0.005) || (  fabs(target_y)  > 0.005)  ){
          NQLog("AssemblyAssembler:: moving to  ") << target_x <<",  "<< target_y <<"  target theta " <<  target_theta ;
            emit moveRelative(target_x, target_y, 0.0, 0.0);
		}
     } 

    else if (alignment_step == 6){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        NQLog("AssemblyAssembler::Detecting second corner");
        alignment_step++;
		double x2_pos = lStepExpressModel_ -> getPosition(0); 	
		double y2_pos = lStepExpressModel_ -> getPosition(1);	
		std::cout << " !!!!! " << std::endl;
		std::cout << " x1 position = " << x1_pos << "  y1 position = " << y1_pos << std::endl;
		std::cout << " !!!!! " << std::endl;
		std::cout << " !!!!! " << std::endl;

		std::cout << " x2 position = " << x2_pos << "  y2 position = " << y2_pos << std::endl;
		std::cout << " !!!!! " << std::endl;

        slope = (y2_pos - y1_pos)/(x2_pos - x1_pos);
    	std::cout << "Sensor is aligned with slope = " << slope << std::endl;
    	double a_xaxis = atan(slope);
    	double ratio = 16.4/23.8 ;	// 47.4/99.75 for silicon dummy   48.0/98.5 for glass w/painted corners
    	double total = atan(ratio);
    	orient = total - fabs(a_xaxis);
		deg_orient = orient * (180.0/3.14);

		// Define orientation to be reached 
		double wanted_slope = -0.71;

		//determine angle between the two slopes
		double  delta_theta_arg = (wanted_slope - slope)/(1.0 + (wanted_slope * slope));
		double delta_theta = atan(delta_theta_arg);
		double delta_theta_deg = (delta_theta *180.0)/3.14;

		double a1 = atan(wanted_slope) *(180.0 / 3.14);
		double a2 = atan(slope) * (180.0/3.14);
		double del_a = a1 - a2;
		
		std::cout << " " << std::endl;
		std::cout << "target angle = " << a1 << std::endl;		
		std::cout << "sensor is aligned with angle = " << a2 << std::endl;
		std::cout << "need to rotate by " << del_a << " degrees " << std::endl;
		std::cout << " " << std::endl;
	
		
		std::cout << " " << std::endl;
		std::cout << "target slope = " << wanted_slope << std::endl;		
		std::cout << "sensor is aligned with slope = " << slope << std::endl;
		std::cout << "need to rotate by " << delta_theta_deg << " degrees " << std::endl;
		std::cout << " " << std::endl;
      
      
        //std::cout << "orientation " << orient << std::endl;
        //std::cout << "in degrees = " << deg_orient << std::endl;
        //std::cout << << std::endl;
        //std::cout << << std::endl;
        //std::cout << << std::endl;
		
		emit acquireImage(); 
    }

	else if (alignment_step == 7){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        NQLog("AssemblyAssembler::First corner aligned");	        
		alignment_step++;
        
        double target_x_1 = (-1.0)*cos((target_theta*3.14)/180.0)*24.5;
        double target_y_1 = (-1.0)*sin((target_theta*3.14)/180.0)*24.5;
        
        double target_y_2 = (1.0)*cos((target_theta*3.14)/180.0)*15.0;
        double target_x_2 = (1.0)*sin((target_theta*3.14)/180.0)*15.0;

        double target_x_3 = target_x_1 + target_x_2; 
        double target_y_3 = -target_y_1 + target_y_2 - 0.5;

        //NQLog("AssemblyAssembler::Going to second corner by moving: ") << target_x_2 <<", "   <<  target_y_2;
        
        emit moveRelative(target_x_3, target_y_3, -0.0, 0.0);  //z = 0.20 for sensor with glue, z=0 for clean sensor
	
    }
	
	else if (alignment_step == 8){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        NQLog("AssemblyAssembler::Detecting second corner");
        alignment_step++;
        emit acquireImage();
        
    } 

	else if (alignment_step == 9){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        NQLog("AssemblyAssembler::centering second corner");
        alignment_step++;
		if ( ( fabs(target_x)  > 0.005) || (  fabs(target_y)  > 0.005)  ){
          NQLog("AssemblyAssembler:: moving to  ") << target_x <<",  "<< target_y <<"  target theta " <<  target_theta ;
            emit moveRelative(target_x, target_y, 0.0, 0.0);
		}
     } 

	else if (alignment_step == 10){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        alignment_step++;

		// Define orientation to be reached 
		double wanted_slope = -0.6954;

		//determine angle between the two slopes
		double  delta_theta_arg = (wanted_slope - slope)/(1.0 + (wanted_slope * slope));
		double delta_theta = atan(delta_theta_arg);
		double delta_theta_deg = (delta_theta *180.0)/3.14;

		double a1 = atan(wanted_slope) *(180.0 / 3.14);
		double a2 = atan(slope) * (180.0/3.14);
		double del_a = a1 - a2;
		
		std::cout << " " << std::endl;
		std::cout << "target angle = " << a1 << std::endl;		
		std::cout << "sensor is aligned with angle = " << a2 << std::endl;
		std::cout << "need to rotate by " << del_a << " degrees " << std::endl;
		std::cout << " " << std::endl;
	
		
		std::cout << " " << std::endl;
		std::cout << "target slope = " << wanted_slope << std::endl;		
		std::cout << "sensor is aligned with slope = " << slope << std::endl;
		std::cout << "need to rotate by " << delta_theta_deg << " degrees " << std::endl;
		std::cout << " " << std::endl;
			
		if ( fabs ( delta_theta_deg ) > 0.01 ) { 
			std::cout << "Need to do something! " << std::endl;

			if ( fabs ( delta_theta_deg) <= 0.5  ) {
				std::cout << " " << std::endl;
				std::cout << "moving to target orientation in one step " << std::endl;
				std::cout << " " << std::endl;
				alignment_step = 0;
           		emit moveRelative(0.0, 0.0, 0.0, delta_theta_deg);
        	} 
		
			else if ( fabs ( delta_theta_deg ) > 0.5){
				std::cout << "  " << std::endl;
				std::cout << "Large rotation needed => move in steps" << std::endl;
				std::cout << " " << std::endl;
				alignment_step = 0;
				if ( delta_theta_deg <= 0) {
					emit moveRelative(0.0, 0.0, 0.0, -0.5);
				}
				else {
           			emit moveRelative(0.0, 0.0, 0.0, +0.5);
				}			
			}
			
			
		} 
		else {
			std::cout <<" In Position " << std::endl;
			std::cout << " " << std::endl; 				
			//emit nextAlignmentStep();
		} 
		
    } 

	
    else if (alignment_step == 11){
        NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;
        X1 = x_pr;
        Y1 = y_pr;
        alignment_step =0;
        NQLog("AssemblyAssembler::TARGET REACHED?! ");
	    //  emit nextAlignmentStep();
	} 
}
 

    /*
 else if(alignment_step == 2){
            NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;

            alignment_step++;
            emit moveRelative(92.60, 0.0, 0.0, 0.0);
            emit nextAlignmentStep();
        }
        else if(alignment_step == 3){
            NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;

            alignment_step++;
            emit acquireImage();
        }
        else if(alignment_step == 4){
            NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;

            alignment_step++;
            emit moveRelative(target_x, target_y, 0.0, 0.0);
        }
        else if (alignment_step == 5){
            NQLog("AssemblyAssembler::run_alignment step == ") << alignment_step;


            double fine_angle = atan ((( x_pr - (1920.0/2) ) * mm_per_pixel_x ) / (92.6)) * (180.00 / 3.141);

            if ( ( fabs(target_x)  > 0.01) || (  fabs(target_y)  > 0.01)||(  fabs(fine_angle)  > 0.01)  ){
                emit moveRelative(target_x, target_y, 0.0, fine_angle/0.2);
                emit acquireImage();
            }else{
                alignment_step++;
                emit nextAlignmentStep();
            }
            
        }else if (alignment_step == 6 ){
        
            NQLog("AssemblyAssembler::run_alignment ***ALIGNMENT COMPLETED** ");
        
        }
    
    //4. Optional Confirm alignment
    //   - return to starting corner and conform detected X doesn't change
   */    

  



void  AssemblyAssembler::run_scan(double range, int steps) {
    
    NQLog("AssemblyScanner::scan") << range << ",  " <<steps;
    
   // steps = 10;
    nTotalImages = steps;
       
    double step_distance = range/steps;
    int nSteps = 0;
    
    x_vals.clear();
    y_vals.clear();

    nAcquiredImages = 1;
    //emit getImage();
    emit moveRelative(0.0,0.0,1.0,0.0);
}


void  AssemblyAssembler::write_image(cv::Mat newImage, cv::Rect marker_rect){
    
    NQLog("AssemblyScanner") << "write_image()";
    QDateTime local(QDateTime::currentDateTime());
    QString local_str = local.toString();
    QString filename = QString("ZScan_%1.png").arg(local_str);
    filename = filename.simplified();
    filename.replace( " ", "" );
    
    Point tl = marker_rect.tl();
    Point br = marker_rect.br();
    
//    rectangle( newImage, rectangle, Scalar(255,0,0), 2, 8, 0 );
 //   rectangle(newImage, rectangle, Scalar(255,0,0), 2, 8, 0);
    
    rectangle(newImage, tl, br, Scalar(255,0,0), 2, 8, 0);

    cv::imwrite(filename.toStdString(), newImage);
    emit updateScanImage(newImage);
    
    double variance  = this->imageVariance(newImage, marker_rect);
    double x = nAcquiredImages;
    double current_z = lStepExpressModel_->getPosition(2);
    
    x_vals.push_back(current_z);
    y_vals.push_back(variance);

    //check global image counter
    if (nAcquiredImages <= nTotalImages){
        std::cout <<"n acquired images = "<< nAcquiredImages<<"  nTotal images = "<< nTotalImages  << std::endl;
        nAcquiredImages++; 
        emit moveRelative(0.0,0.0,-0.1,0.0);
    } else{
        
        int points  =  y_vals.size();
        TGraph * gr = new TGraph(points);
        TCanvas *  canvas = new TCanvas();
        
        double variance_max, z_at_peak =0;
        
        //fitting the peak (Gaussian?) would improve precision a little here
        for (unsigned long  i =0; i < y_vals.size(); i++){
            gr->SetPoint(i, x_vals[i], y_vals[i]);
            if (y_vals[i] > variance_max){
                variance_max = y_vals[i];
                z_at_peak = x_vals[i];
            }
        }
        
        gr->Draw("AC*");
        const std::string img_name = "variance.png";
        canvas->SaveAs(img_name.c_str());
    
        emit make_graph(img_name);

      //emit some signal with measured marker Z position
        emit updateText(z_at_peak);
    }
}



double AssemblyAssembler::imageVariance(cv::Mat img_input, cv::Rect rectangle)
{
    Mat img_copy = img_input.clone();
    Mat roi(img_copy, rectangle);

    // Convert color image to greyscale (GS)
    Mat img_copy_gs(img_copy.size(), img_copy.type());
//    cvtColor(img_copy, img_copy_gs, CV_BGR2GRAY);

    // Apply laplacian function to GS image
    Mat img_laplace;
    Laplacian(img_copy, img_laplace, CV_64F);

    // Calculate standard deviation of laplace image
    cv::Scalar mean, std_dev;
    meanStdDev(img_laplace, mean, std_dev, cv::Mat());

    std::cout << "Variance: " <<std_dev.val[0]*std_dev.val[0] << std::endl;

    return std_dev.val[0]*std_dev.val[0];
}
