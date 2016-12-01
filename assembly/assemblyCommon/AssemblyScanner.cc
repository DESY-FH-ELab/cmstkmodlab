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

AssemblyScanner::AssemblyScanner(AssemblyVUEyeModel *uEyeModel_,LStepExpressModel* lStepExpressModel_)
{
    NQLog("AssemblyZScanner::AssemblyScanner()");
    
    

    
}


void AssemblyScanner::enable_autofocus(int enabled)
{
    NQLog("AssemblyScanner:enable_autofocus() ");
    
    if (enabled == 2){
        NQLog("AssemblyZScanner:enable_autofocus() ") << " connecting motion/vision for scan " ;

    //construct motion manager from motion model
	 motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    
    //get mobile camera from camera model
       camera_ = uEyeModel_->getCameraByID(10);
    if (camera_){
    NQLog("AssemblyZScanner:camera object created() ");
     } else{
    NQLog("AssemblyZScanner:NULL camera object ");
    }

        
    connect (this, SIGNAL(getImage()), camera_, SLOT(acquireImage()));
	  connect(camera_, SIGNAL(imageAcquired(cv::Mat)),  this, SLOT(write_image(cv::Mat)) );



    }
    else if (enabled == 0){
    
        NQLog("AssemblyScanner:enable_autofocus() ") << " disconnecting motion/vision for scan "  ;
        
        disconnect (this, SIGNAL(getImage()), camera_, SLOT(acquireImage()));
        disconnect(camera_, SIGNAL(imageAcquired(cv::Mat)),  this, SLOT(write_image(cv::Mat)) );

    }
    
    
}


void AssemblyScanner::run_scan(double step_size, int nSteps)
{
    NQLog("AssemblyScanner:run_scan()");
    
}


void AssemblyScanner::write_image(cv::Mat)
{
    NQLog("AssemblyScanner:write_image()");
    
    
}






