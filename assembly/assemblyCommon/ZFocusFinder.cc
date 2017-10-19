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

#include <ZFocusFinder.h>
#include <nqlogger.h>
#include <Log.h>

#include <QFormLayout>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>
#include <QApplication>

#include <TGraph.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TRandom.h>

#include <vector>
#include <iostream>
#include <iomanip>

ZFocusFinder::ZFocusFinder(AssemblyVUEyeCamera* camera, LStepExpressModel* lStepExpressModel_, QObject* parent) :
  QObject(parent),
  camera_(camera)
{
//    qt = new QTimer(this);
//    qt->setSingleShot(true);
//    connect(qt, SIGNAL(timeout()), this, SLOT(process_step()));

//  NQLog("ZFocusFinder::ZFocusFinder") << "constructed";
}

//!!void  ZFocusFinder::run_precisionestimation(double x_m, double y_m, double z_m , double x_p, double y_p, double z_p, int its){
//!!    
//!!    NQLog("ZFocusFinder::run_precisionestimation")<<  " x_m = " << x_m <<  " y_m = " << y_m << " z_m = " << z_m << " x_p = " << x_p  << " y_p = " << y_p  <<" z_p = " << z_p <<  " iterations = " << its;
//!!    
//!!    x_meas = x_m;
//!!    y_meas = y_m;
//!!    z_meas = z_m;
//!!    
//!!    x_pickup = x_p;
//!!    y_pickup = y_p;
//!!    z_pickup = z_p;
//!!    
//!!    iterations = its;
//!!    iteration = 0;
//!!    
//!!    step = 0;
//!!    z_prepickup = z_pickup + 5.00;
//!!
//!!
//!!    emit nextStep();
//!!}

//!!void  ZFocusFinder::fill_positionvectors(int stage, double x_pr, double y_pr, double theta_pr ){
//!!    
//!!    NQLog("ZFocusFinder::fill_positionvectors")<< " step =  " << step<< "  x = "<< x_pr << "  y = "<< y_pr << "  theta = "<< theta_pr ;
//!!
//!!    
//!!    if (step == 2){
//!!    
//!!        xpre_vec.push_back(x_pr);
//!!        ypre_vec.push_back(y_pr);
//!!        thetapre_vec.push_back(theta_pr);
//!!    
//!!    }else if(step == 13){
//!!    
//!!        xpost_vec.push_back(x_pr);
//!!        ypost_vec.push_back(y_pr);
//!!        thetapost_vec.push_back(theta_pr);
//!!        
//!!    }
//!!    
//!!    emit nextStep();
//!!}

//!!void ZFocusFinder::toggleVacuum()
//!!{
//!!    NQLog("ZFocusFinder") << ": toggling vacuum voltage";
//!!    
//!!            if (cnrd1->getSwitchState(1) == 0){
//!!                cnrd1->setSwitchEnabled(1, true);
//!!                
//!!            }else if (cnrd1->getSwitchState(1) == 1){
//!!                cnrd1->setSwitchEnabled(1, false);
//!!            }
//!!        
//!!}

void ZFocusFinder::do_autofocus()
{
}

void ZFocusFinder::process_step(){

    NQLog("ZFocusFinder::process_step, iteration ==") <<iteration <<" step = " << step;

    if (iteration < iterations){
    
    if (step == 0){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
// Step 0: Go to measurement position
	NQLog("AssemblySensorMarkerFinder") << "   Requesting motion to:  x "<< x_meas <<" y "<< y_meas <<" "<< z_meas  ;
    emit moveAbsolute(x_meas,y_meas,z_meas, 0.0);
    }
    else if  (step == 1){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
// Step 1: Run pattern recognition
	emit acquireImage();
	//emit nextStep();

    }else if (step == 2){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
        // Go to pre-pickup position
	emit moveAbsolute(x_pickup,y_pickup,z_prepickup, 0.0);
	//emit nextStep();

    }
    else if (step == 3){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
	//emit nextStep();

	emit moveAbsolute(x_pickup,y_pickup,z_pickup, 0.0);
        
    }else if (step == 4){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
        emit nextStep();

        // Step 4: Turn on pick up vacuum
        //emit toggleVacuum(1);

        
    }else if (step == 5){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
        emit nextStep();

        // Step 5: Turn off bottom vacuum
        //emit toggleVacuum(2);
        
    }else if (step == 6){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
        //emit nextStep();
        // Step 6: Go back to pre-pickup position
	emit moveAbsolute(x_pickup,y_pickup,z_prepickup, 0.0);

    }
    else if (step == 7){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
	//emit nextStep();
        // Step 7: Go back to pickup position
        emit moveAbsolute(x_pickup,y_pickup,z_pickup, 0.0);
 
        
    }else if (step == 8){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
        emit nextStep();
        // Step 8: Turn on bottom vacuum
	// emit toggleVacuum(2);
 
        
    }else if (step == 9){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
        emit nextStep();
        // Step 9: Turn off pick up vacuum
        //emit toggleVacuum(1);

        
    }else if (step == 10){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
        //emit nextStep();
        // Step 10: Go back to pre-pickup position
	emit moveAbsolute(x_pickup,y_pickup,z_prepickup, 0.0);
        
        
    }else if (step == 11){

      NQLog("ZFocusFinder:: step == ") << step;
        step++;
	//emit nextStep();
        // Step 11: Go back to measurement position
	emit moveAbsolute(x_meas,y_meas,z_meas, 0.0);

   
    }else if (step == 12){

      NQLog("ZFocusFinder:: step == ") << step;

        step++;
        emit acquireImage();
	//emit nextStep();

    }else if  (step == 13){

      NQLog("ZFocusFinder:: step == ") << step;
      step = 0;
        iteration++;
        emit nextStep();
     
    }
    }else{
        NQLog("ZFocusFinder::processstep")<< " estimation finished ";
        //make graphs from filled vectors here
        NQLog("ZFocusFinder::processstep")<< " vector has size =  "<<xpre_vec.size();
        
        
        TH1F * h_x = new TH1F("","", 20,-0.5, 0.5);
        TH1F * h_y = new TH1F("","", 20,-0.5, 0.5);
        TH1F * h_theta = new TH1F("","", 20,-1.0,1.0);

	
//!!	outfile.open("DataLogfile.txt");
//!!	outfile << "x_pre,y_pre,theta_pre,x_pos,y_pos,theta_pos" << std::endl;
//!!	
//!!        for (int i = 0; i< iterations; i++){
//!!        
//!!            h_x->Fill(xpre_vec[i] - xpost_vec[i] );
//!!            h_y->Fill(ypre_vec[i] - ypost_vec[i] );
//!!            h_theta->Fill(thetapre_vec[i] - thetapost_vec[i] );
//!!	    NQLog("ZFocusFinder::")<< " filling histos with: x pre  =  "<<  xpre_vec[i] << " x post  "<< xpost_vec[i] << " y  pre  = "<<  ypre_vec[i] << " y post  "<< ypost_vec[i] <<  " theta pre "<<   thetapre_vec[i] << " theta  post  = "<<  thetapost_vec[i] ;
//!!
//!!	    outfile << xpre_vec[i] << "," << ypre_vec[i] << "," << thetapre_vec[i] << ",";
//!!	    outfile << xpost_vec[i] << "," << ypost_vec[i] << "," << thetapost_vec[i];
//!!	    outfile << std::endl;
//!!
//!!        }
//!!
//!!	outfile.close();

        std::string x_canvas_s = "c_x.png";
        std::string y_canvas_s = "c_y.png";
        std::string theta_canvas_s = "c_theta.png";

        
        TCanvas * c_x  = new TCanvas();
        h_x->Draw();
        h_x->Fit("gaus");
        TF1 *fit_x = h_x->GetFunction("gaus");
        double x_mean = fit_x->GetParameter(1);
        double x_sigma = fit_x->GetParameter(2);

        c_x->SaveAs(x_canvas_s.c_str());

        TCanvas * c_y  = new TCanvas();
        h_y->Draw();
        h_y->Fit("gaus");
        c_y->SaveAs(y_canvas_s.c_str());


        TCanvas * c_theta  = new TCanvas();
        h_theta->Draw();
        h_theta->Fit("gaus");
        c_theta->SaveAs(theta_canvas_s.c_str());
        
        
        QString x_canvas_qs = QString::fromLocal8Bit(x_canvas_s.c_str());
        QString y_canvas_qs = QString::fromLocal8Bit(y_canvas_s.c_str());
        QString theta_canvas_qs = QString::fromLocal8Bit(theta_canvas_s.c_str());
        
        
        emit showHistos( 1, x_canvas_qs);
        emit showHistos( 2, y_canvas_qs);
        emit showHistos( 3, theta_canvas_qs);
        
    }
    
}

void  ZFocusFinder::run_scan(double range, int steps){
    
    NQLog("ZFocusFinder::scan") << range << ",  " <<steps;
    
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

void  ZFocusFinder::write_image(cv::Mat newImage, cv::Rect marker_rect){

    NQLog("ZFocusFinder") << "write_image()";
    QDateTime local(QDateTime::currentDateTime());
    QString local_str = local.toString();
    QString filename = QString("ZScan_%1.png").arg(local_str);
    filename = filename.simplified();
    filename.replace( " ", "" );
    
    cv::Point tl = marker_rect.tl();
    cv::Point br = marker_rect.br();
    
//    rectangle( newImage, rectangle, cv::Scalar(255,0,0), 2, 8, 0 );
 //   rectangle(newImage, rectangle, cv::Scalar(255,0,0), 2, 8, 0);
    
    rectangle(newImage, tl, br, cv::Scalar(255,0,0), 2, 8, 0);

    cv::imwrite(filename.toStdString(), newImage);
    emit updateScanImage(newImage);
    
    double variance  = this->image_focus_value(newImage);
    double x = nAcquiredImages;
    double current_z = lStepExpressModel_->getPosition(2);
    
    x_vals.push_back(current_z);
    y_vals.push_back(variance);

    //check global image counter
    if (nAcquiredImages <= nTotalImages){
        std::cout <<"n acquired images = "<< nAcquiredImages<<"  nTotal images = "<< nTotalImages  <<std::endl;
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

/* \Brief Image-focus discriminant based on Laplacian method in OpenCV
 *        REF: https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/laplace_operator/laplace_operator.html
 */
double ZFocusFinder::image_focus_value(const cv::Mat& img)
{
  // Remove noise by blurring with a Gaussian filter
  cv::Mat img_gaus;
  cv::GaussianBlur(img, img_gaus, cv::Size(img.cols, img.rows), 0, 0, cv::BORDER_DEFAULT);

  // Convert the image to grayscale
  cv::Mat img_gray;
  cv::cvtColor(img_gaus, img_gray, CV_BGR2GRAY);

  // Apply laplacian function to GS image
  cv::Mat img_lap;
  cv::Laplacian(img_gray, img_lap, CV_64F);

  cv::Mat img_lap_abs;
//  cv::convertScaleAbs(img_lap, img_lap_abs);
  img_lap_abs = img_lap.clone();

  // Calculate standard deviation of laplace image
  cv::Scalar mean, std_dev;
  cv::meanStdDev(img_lap_abs, mean, std_dev);

  const float value = (std_dev.val[0] * std_dev.val[0]);

  NQLog("ZFocusFinder::image_focus_value") << "value = " << value;

  return value;
}
