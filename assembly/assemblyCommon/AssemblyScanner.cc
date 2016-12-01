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

AssemblyScanner::AssemblyScanner(AssemblyVUEyeCamera *camera_)
{
    NQLog("AssemblyZScanner::AssemblyScanner()");
    connect (this, SIGNAL(getImage()), camera_, SLOT(acquireImage()));
    connect(camera_, SIGNAL(imageAcquired(cv::Mat)),  this, SLOT(write_image(cv::Mat)) );

    
}


void AssemblyScanner::enable_autofocus(int enabled)
{
    NQLog("AssemblyScanner:enable_autofocus() ");
    
    if (enabled == 2){
        NQLog("AssemblyScanner:enable_autofocus() ") << " connecting motion/vision for scan " ;

    //construct motion manager from motion model
	// motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    
    //get mobile camera from camera model
      // camera_ = uEyeModel_->getCameraByID(10);
    if (camera_){
        
    NQLog("AssemblyScanner:camera object created() ");
    
    } else{
         
    NQLog("AssemblyScanner:NULL camera object ");
    
    }

        
    connect (this, SIGNAL(getImage()), camera_, SLOT(acquireImage()));
    connect(camera_, SIGNAL(imageAcquired(cv::Mat)),  this, SLOT(write_image(cv::Mat)) );



    }
    else if (enabled == 0){
    
        NQLog("AssemblyScanner:enable_autofocus() ") << " disconnecting motion/vision for scan "  ;
        
    //    disconnect (this, SIGNAL(getImage()), camera_, SLOT(acquireImage()));
     //   disconnect(camera_, SIGNAL(imageAcquired(cv::Mat)),  this, SLOT(write_image(cv::Mat)) );

    }
    
    
}



void  AssemblyScanner::run_scan(double range, int steps){
    
    NQLog("AssemblyScanner::scan") << range << ",  " <<steps;
    
    steps = 10;
    nTotalImages = steps;
    
    double step_distance = range/steps;
    int nSteps = 0;
    
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
    
    
    //check global image counter
    if (nAcquiredImages < nTotalImages){
        cout <<"n acquired images = "<< nAcquiredImages<<"  nTotal images = "<< nTotalImages  <<endl;
        nAcquiredImages++;
        emit getImage();
    }
    
    //possibly disconnect (disconnect(camera_, SIGNAL(imageAcquired(cv::Mat)),  this, SLOT(write_image(cv::Mat)) );
    // when enough images are taken

}







