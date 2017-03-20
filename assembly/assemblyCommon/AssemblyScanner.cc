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


#include <TGraph.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TRandom.h>




#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include "AssemblyScanner.h"

using namespace std;
using namespace cv;

TRandom * r = new TRandom();


AssemblyScanner::AssemblyScanner(LStepExpressModel* lStepExpressModel_)
{
    NQLog("AssemblyScanner::AssemblyScanner()");
    motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);

}

void  AssemblyScanner::run_precisionestimation(double x_m, double y_m, double z_m , double x_p, double y_p, double z_p, int its){
    
    NQLog("AssemblyScanner::run_precisionestimation")<<  " x_m = " << x_m <<  " y_m = " << y_m << " z_m = " << z_m << " x_p = " << x_p  << " y_p = " << y_p  <<" z_p = " << z_p <<  " iterations = " << its;
    
    x_meas = x_m;
    y_meas = y_m;
    z_meas = z_m;
    
    x_pickup = x_p;
    y_pickup = y_p;
    z_pickup = z_p;
    
    iterations = its;
    iteration = 0;
    
    step = 0;
    z_prepickup = z_pickup + 5.00;


    emit nextStep();
}


void  AssemblyScanner::fill_positionvectors(int stage, double x_pr, double y_pr, double theta_pr ){
    
   // x_pr = r->Gaus(0.0,1.0);
   // y_pr = r->Gaus(0.0,5.0);
   // theta_pr = r->Gaus(0.0,10.0);

    NQLog("AssemblyScanner::fill_positionvectors")<< " step =  " << step<< "  x = "<< x_pr << "  y = "<< y_pr << "  theta = "<< theta_pr ;

    
    if (step == 2){
    
        xpre_vec.push_back(x_pr);
        ypre_vec.push_back(y_pr);
        thetapre_vec.push_back(theta_pr);
    
    }else if(step == 13){
    
        xpost_vec.push_back(x_pr);
        ypost_vec.push_back(y_pr);
        thetapost_vec.push_back(theta_pr);
        
    }
    
    emit nextStep();
}


void  AssemblyScanner::process_step(){

    NQLog("AssemblyScanner::process_step, iteration ==") <<iteration;
    
    if (iteration < iterations){
    
    if (step == 0){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
// Step 0: Go to measurement position
	NQLog("AssemblySensorMarkerFinder") << "   Requesting motion to:  x "<< x_meas <<" y "<< y_meas <<" "<< z_meas  ;
        emit moveAbsolute(x_meas,y_meas,z_meas, 0.0);
        //emit nextStep();
    }
    else if  (step == 1){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
// Step 1: Run pattern recognition
	emit acquireImage();
	//emit nextStep();

    }else if (step == 2){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
        // Go to pre-pickup position
	emit moveAbsolute(x_pickup,y_pickup,z_prepickup, 0.0);
	//emit nextStep();
    }
    else if (step == 3){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
	//emit nextStep();
        // Go to pickup position
	emit moveAbsolute(x_pickup,y_pickup,z_pickup, 0.0);
        
    }else if (step == 4){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
        emit nextStep();

        // Step 4: Turn on pick up vacuum
        //emit toggleVacuum(1);
        
    }else if (step == 5){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
        emit nextStep();

        // Step 5: Turn off bottom vacuum
        //emit toggleVacuum(2);
        
    }else if (step == 6){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
        //emit nextStep();
        // Step 6: Go back to pre-pickup position
	emit moveAbsolute(x_pickup,y_pickup,z_prepickup, 0.0);
    }
    else if (step == 7){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
	//emit nextStep();
        // Step 7: Go back to pickup position
        emit moveAbsolute(x_pickup,y_pickup,z_pickup, 0.0);
 
        
    }else if (step == 8){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
        emit nextStep();
        // Step 8: Turn on bottom vacuum
	// emit toggleVacuum(2);
 
        
    }else if (step == 9){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
        emit nextStep();
        // Step 9: Turn off pick up vacuum
        //emit toggleVacuum(1);

        
    }else if (step == 10){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
        //emit nextStep();
        // Step 10: Go back to pre-pickup position
	emit moveAbsolute(x_pickup,y_pickup,z_prepickup, 0.0);
        
        
    }else if (step == 11){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
	//emit nextStep();
        // Step 11: Go back to measurement position
	emit moveAbsolute(x_meas,y_meas,z_meas, 0.0);

   
    }else if (step == 12){

      NQLog("AssemblyScanner:: step == ") << step;
        step++;
        emit acquireImage();
	//emit nextStep();

    }else if  (step == 13){

      NQLog("AssemblyScanner:: step == ") << step;
      step = 0;
        iteration++;
        emit nextStep();
     
    }
    }else{
        NQLog("AssemblyScanner::processstep")<< " estimation finished ";
        //make graphs from filled vectors here
        NQLog("AssemblyScanner::processstep")<< " vector has size =  "<<xpre_vec.size();
        
        
        TH1F * h_x = new TH1F("","", 20,-0.5, 0.5);
        TH1F * h_y = new TH1F("","", 20,-0.5, 0.5);
        TH1F * h_theta = new TH1F("","", 20,-1.0,1.0);

	
	outfile.open("DataLogfile.txt");
	outfile << "x_pre,y_pre,theta_pre,x_pos,y_pos,theta_pos" << endl;
	
        for (int i = 0; i< iterations; i++){
        
            h_x->Fill(xpre_vec[i] - xpost_vec[i] );
            h_y->Fill(ypre_vec[i] - ypost_vec[i] );
            h_theta->Fill(thetapre_vec[i] - thetapost_vec[i] );
	    NQLog("AssemblyScanner::")<< " filling histos with: x pre  =  "<<  xpre_vec[i] << " x post  "<< xpost_vec[i] << " y  pre  = "<<  ypre_vec[i] << " y post  "<< ypost_vec[i] <<  " theta pre "<<   thetapre_vec[i] << " theta  post  = "<<  thetapost_vec[i] ;

	    outfile << xpre_vec[i] << "," << ypre_vec[i] << "," << thetapre_vec[i] << ",";
	    outfile << xpost_vec[i] << "," << ypost_vec[i] << "," << thetapost_vec[i];
	    outfile << std::endl;

        }

	outfile.close();

        string x_canvas_s = "c_x.png";
        string y_canvas_s = "c_y.png";
        string theta_canvas_s = "c_theta.png";

        
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



void  AssemblyScanner::run_scan(double range, int steps){
    
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


void  AssemblyScanner::write_image(cv::Mat newImage, cv::Rect marker_rect){
    
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
        cout <<"n acquired images = "<< nAcquiredImages<<"  nTotal images = "<< nTotalImages  <<endl;
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
        const string img_name = "variance.png";
        canvas->SaveAs(img_name.c_str());
    
        emit make_graph(img_name);

      //emit some signal with measured marker Z position
        emit updateText(z_at_peak);
    }
}



double AssemblyScanner::imageVariance(cv::Mat img_input, cv::Rect rectangle) {
    
    
    Mat img_copy = img_input.clone();
    Mat roi(img_copy, rectangle);
    
    //Convert color image to greyscale (GS)
    Mat img_copy_gs(img_copy.size(), img_copy.type());
  //  cvtColor(img_copy, img_copy_gs, CV_BGR2GRAY);
    
    //Apply laplacian function to GS image
    Mat img_laplace;
    Laplacian(img_copy, img_laplace, CV_64F);
    
    //Calculate standard deviation of laplace image
    cv::Scalar mean, std_dev;
    meanStdDev(img_laplace, mean, std_dev, cv::Mat());
    
    std::cout<<"Variance: "<<std_dev.val[0]*std_dev.val[0]<<std::endl;
    
    return std_dev.val[0]*std_dev.val[0];
    
}






