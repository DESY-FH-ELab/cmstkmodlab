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

AssemblyScanner::AssemblyScanner(AssemblyVUEyeModel *uEyeModel_)
{
    NQLog("AssemblyZScanner::AssemblyScanner()");
    
  //  camera_l = uEyeModel_->getCameraByID(10);

   // connect (this, SIGNAL(getImage()), camera_l, SLOT(acquireImage()));
   // connect(camera_l, SIGNAL(imageAcquired(cv::Mat)),  this, SLOT(write_image(cv::Mat)) );

    
}


void AssemblyScanner::enable_autofocus(int enabled)
{
    NQLog("AssemblyScanner:enable_autofocus() ");
    
    if (enabled == 2){
        NQLog("AssemblyScanner:enable_autofocus() ") << " connecting motion/vision for scan " ;

        camera_l = uEyeModel_->getCameraByID(10);

        
 
    }
    else if (enabled == 0){
    
        NQLog("AssemblyScanner:enable_autofocus() ") << " disconnecting motion/vision for scan "  ;
        

    //    disconnect (this, SIGNAL(getImage()), camera_, SLOT(acquireImage()));
     //   disconnect(camera_, SIGNAL(imageAcquired(cv::Mat)),  this, SLOT(write_image(cv::Mat)) );

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
    emit getImage();
    
}


void  AssemblyScanner::write_image(cv::Mat newImage){
    
    NQLog("AssemblyScanner") << "write_image()";
    QDateTime local(QDateTime::currentDateTime());
    QString local_str = local.toString();
    QString filename = QString("ZScan_%1.png").arg(local_str);
    filename = filename.simplified();
    filename.replace( " ", "" );
    
    cv::imwrite(filename.toStdString(), newImage);
    emit updateScanImage(newImage);
    
    double variance  = this->imageVariance(newImage);
    double x = nAcquiredImages;     
    x_vals.push_back(x);
    y_vals.push_back(variance);

    //check global image counter
    if (nAcquiredImages <= nTotalImages){
        cout <<"n acquired images = "<< nAcquiredImages<<"  nTotal images = "<< nTotalImages  <<endl;
        nAcquiredImages++;
        
        emit getImage();
    } else{
    
      emit make_graph(x_vals, y_vals);    
    }
    
    //possibly disconnect (disconnect(camera_, SIGNAL(imageAcquired(cv::Mat)),  this, SLOT(write_image(cv::Mat)) );
    // when enough images are taken

}



double AssemblyScanner::imageVariance(cv::Mat img_input) {
    
    
    Mat img_copy = img_input.clone();
    
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






