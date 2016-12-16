#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>
#include <QApplication>


#include <TGraph.h>
#include <TCanvas.h>


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

AssemblyScanner::AssemblyScanner(AssemblyVUEyeModel *uEyeModel_, LStepExpressModel* lStepExpressModel_)
{
    NQLog("AssemblyScanner::AssemblyScanner()");
    motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    
  //  camera_l = uEyeModel_->getCameraByID(10);

   // connect (this, SIGNAL(getImage()), camera_l, SLOT(acquireImage()));
   // connect(camera_l, SIGNAL(imageAcquired(cv::Mat)),  this, SLOT(write_image(cv::Mat)) );


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
    x_vals.push_back(x);
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






