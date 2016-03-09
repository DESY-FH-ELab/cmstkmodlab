#include <iostream>

#include <QApplication>
#include <QString>

#include <nqlogger.h>

#include "AssemblyVUEyeCamera.h"
#include <iostream>

#include <unistd.h> 
#include <ueye.h>
#include <uEye.h>
//#include <uEye_tools.h>
#include <ueye_deprecated.h>


using namespace std;

AssemblyVUEyeCamera::AssemblyVUEyeCamera(QObject *parent)
    : QObject(parent),
      cameraState_(State::OFF)
{

}

AssemblyVUEyeCamera::~AssemblyVUEyeCamera()
{

}

size_t AssemblyVUEyeCamera::getCurrentPixelClockIndex() const
{
    size_t idx = 0;
    for (std::vector<unsigned int>::const_iterator it = pixelClocks_.begin();
         it != pixelClocks_.end();
         ++it) {
        if (*it == currentPixelClock_) {
            return idx;
        }
        idx++;
    }
    return idx;
}


void AssemblyVUEyeCamera::calibrateSettings()
{
   NQLog("AssemblyVUEyeCamera") << ":calibrateSettings()";
   NQLog("AssemblyVUEyeCamera") << "**********************exposure time scan***************************" ;

   loopFind_ = new AssemblySensorMarkerFinder();

   HIDS hCam = getCameraID();
   char* imgMem;
   int memId;
  
      if(is_InitCamera (&hCam, NULL)!= IS_SUCCESS){
      
   NQLog("AssemblyVUEyeCamera") << "CANT INIT CAMERA";

     }

   NQLog("AssemblyVUEyeCamera") << " INIT CAMERA";


    int img_width=2650, img_height=1920, img_bpp=24, img_step, img_data_size;
     is_AllocImageMem(hCam, img_width, img_height, img_bpp, &imgMem, &memId);
     is_SetImageMem (hCam, imgMem, memId);
     is_SetDisplayMode (hCam, IS_SET_DM_DIB);
     is_SetColorMode (hCam, IS_CM_RGB8_PACKED);
     is_SetImageSize (hCam, img_width, img_height);
 

     double enable = 1;
     double disable = 0;
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_GAIN, &enable, 0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &enable, 0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_FRAMERATE, &disable, 0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SHUTTER, &disable, 0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &enable, 0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE,&enable,0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &disable, 0);

     double FPS,NEWFPS;
     FPS = 15;
     double fps;
     is_SetFrameRate(hCam,FPS,&NEWFPS);
  
     //     double parameter = 50;
     // int error = is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*) &parameter, sizeof(parameter));
     // if (error != IS_SUCCESS) {
     //NQLog("AssemblyVUEyeCamera") << " cant set exposure";
     //        }
     //NQLog("AssemblyVUEyeCamera") << "  set exposure";
     // error = is_Exposure(hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*) &parameter, sizeof(parameter));
 
     UINT uiCaps = 0;
 
     INT nRet = is_Focus (hCam, FDT_CMD_GET_CAPABILITIES, &uiCaps, sizeof (uiCaps) );
 
     if (nRet == IS_SUCCESS && (uiCaps & FOC_CAP_AUTOFOCUS_SUPPORTED))
     {
       NQLog("AssemblyVUEyeCamera") << "if supported, enable auto focus";

       //qDebug() << "If supported, enable auto us";
     nRet = is_Focus (hCam, FOC_CMD_SET_DISABLE_AUTOFOCUS, NULL, 0);
     }
    NQLog("AssemblyVUEyeCamera") << "  set focus";

     nRet = is_SetGamma(hCam,600);
     enable = 2;
     char filename[512];


     int radius;
     bool doca_,dist1_,dist2_,dist3_;
 

 for(int pc=10.0; pc<12.0; pc=pc+5.0)
     {
   for(int ii=10.0; ii<15.0; ii=ii+20.0)
   {

    NQLog("AssemblyVUEyeCamera") << "  looping";

    // if( is_FreezeVideo(hCam, IS_DONT_WAIT) == IS_SUCCESS){
    int nPixelClock = pc;
    int nRet = is_PixelClock(hCam, IS_PIXELCLOCK_CMD_SET, (void*)&nPixelClock, sizeof(nPixelClock));

    //    double parameter = ii;
    //int error = is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*) &parameter, sizeof(parameter));

    //   if (error != IS_SUCCESS) {
    //    NQLog("AssemblyVUEyeCamera") << " cant set exposure";
    //         }
    // NQLog("AssemblyVUEyeCamera") << "  set exposure";


       if(is_FreezeVideo(hCam, IS_WAIT) == IS_SUCCESS){
   NQLog("AssemblyVUEyeCamera") << "  getting frame";
            void *pMemVoid; //pointer to where the image is stored
            is_GetImageMem (hCam, &pMemVoid);
            IplImage * img;
            img=cvCreateImage(cvSize(img_width, img_height), IPL_DEPTH_8S, 1); 
            img->nSize=112;
            img->ID=0;
            img->nChannels=3;
            img->alphaChannel=0;
            img->depth=8;
            img->dataOrder=0;
            img->origin=0;
            img->align=4;
            img->width=img_width;
            img->height=img_height;
            img->roi=NULL;
            img->maskROI=NULL;
            img->imageId=NULL;
            img->tileInfo=NULL;
	    // img->imageSize=3*img_width*img_height;
            img->imageData=(char*)pMemVoid;  //the pointer to imagaData
            img->widthStep=3*img_width;
            img->imageDataOrigin=(char*)pMemVoid; //and again
            //now you can use your img just like a normal OpenCV image
	    //    cvNamedWindow( "A", 1 );
            //cvShowImage("A",img);
            //cv::waitKey(1);

	    cv::Mat mat_img(img);
	    cv::Mat img_gs, img_rgb,img_edges;

            cvtColor(mat_img, img_gs, CV_RGB2GRAY);
	    //    QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.png");

	    std::string str_ii, str_p , str_lt, str_pc;
	    ostringstream convert;   // stream used for the conversion
	    convert << ii;      // insert the textual representation of 'Number' in the characters in the stream
	    str_ii = convert.str();

	    //   if (filename.isNull() || filename.isEmpty()) return;
	    //if (!filename.endsWith(".png")) filename += ".png";

	  
    NQLog("AssemblyVUEyeCamera") << "  got frame";
    int  expectedCircleRadius_ = 89;
    double circleCenterDetectionThreshold_  = 35.0;
    double circleEdgeDetectionThreshold_  = 35.0;

    double linesCannyEdgeDetectionThreshold1_ =50;
    double linesCannyEdgeDetectionThreshold2_ =200;
    double linesCannyEdgeDetectionApertureSize_ =3;
    double linesCannyEdgeDetectionL2Gradient_ =1;
    double linesHoughDistanceResolution_ = 1.0; 
    //    double linesHoughAngleResolution_*CV_PI/180,
    double linesHoughAngleResolution_ = 0.1;
    double linesHoughThreshold_ = 75.0;
    double linesHoughMinLineLength_=  150.0;
    double linesHoughMaxLineGap_ =25.0;


    std::vector<cv::Vec3f> circles;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > lines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > goodLines_;
    std::vector<cv::Point2f> intersections_;
    std::vector<cv::Point2f> goodIntersections_;
    // for(int lt = 150; lt<300 ; lt=lt+10){
     for(int lt = 20; lt<60 ; lt=lt+20){

   //linesHoughThreshold_ = lt
     linesCannyEdgeDetectionThreshold2_ =lt;

         for(int p = 38; p< 39; p=p+2){
     	  int pp  = p;
	  // loopFind_->setCircleEdgeDetectionThreshold(pp);
	  // loopFind_->findMarker(img_gs);
	  circleEdgeDetectionThreshold_ = pp;

	    cv::HoughCircles(img_gs, circles, CV_HOUGH_GRADIENT, 1, 20,
                     circleEdgeDetectionThreshold_, circleCenterDetectionThreshold_,
                     expectedCircleRadius_ - 15., expectedCircleRadius_ + 15.);

	  NQLog("AssemblyVUEyeCamera") << "  found circles = = "<<  circles.size();
	  convert.str(" ");
	  convert.clear();
	  convert << p;
	  str_p = convert.str();
	  convert.str(" ");
	  convert.clear();
	  convert << lt;
	  str_lt = convert.str();

	  convert.str(" ");
	  convert.clear();
	  convert << pc;
	  str_pc = convert.str();
	  // std::cout<<" exposure  = "<<str_ii << "   threshold = " <<  str_p << std::endl;

	  //   cv::Point2f center(cvRound(circleCenter_.x), cvRound(circleCenter_.y));
	  //  circle center
	  // cv::circle(local_image, center, 8, cv::Scalar(0, 255, 0), -1, CV_AA, 0);
	  //circle outline
	  //cv::circle(local_image, center, circleRadius_, cv::Scalar(0, 0, 255), 4, CV_AA, 0);
 
	   cvtColor(img_gs, img_rgb , CV_GRAY2RGB);

            /// Draw the circles detected
            for( size_t i = 0; i < circles.size(); i++ )
            {
	      cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	     int  x = cvRound(circles[i][0]);
	     int  y = cvRound(circles[i][1]);
	     radius = cvRound(circles[i][2]);
	     // circle center
	        circle( img_rgb, center, 3, cv::Scalar(0,0,255), -1, 8, 0 );
	     // circle outline;
	      circle( img_rgb, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
            }
          

	     cv::Canny(img_gs, img_edges,
	    		      linesCannyEdgeDetectionThreshold1_,
	    		      linesCannyEdgeDetectionThreshold2_,
	    		      linesCannyEdgeDetectionApertureSize_,
	    		      linesCannyEdgeDetectionL2Gradient_);

    std::vector<cv::Vec4i> tempLines;

	    lines_.clear();
	    goodLines_.clear();

	    cv::HoughLinesP(img_edges, tempLines,
                    linesHoughDistanceResolution_,
                    linesHoughAngleResolution_*CV_PI/180,
                    linesHoughThreshold_,
                    linesHoughMinLineLength_,
                    linesHoughMaxLineGap_);

 for (size_t i = 0; i < tempLines.size(); i++ ) {
        lines_.push_back(std::pair<cv::Point2f,cv::Point2f>(cv::Point2f(tempLines[i][0],tempLines[i][1]), cv::Point2f(tempLines[i][2],tempLines[i][3])));
    }

    double distance,doca,x0,x1,x2, y0,y1,y2;
    cv::Point2f circleCenter_;
    if (circles.size() != 0){
    circleCenter_.x = circles[0][0];
    circleCenter_.y = circles[0][1];
    }

  

 for (size_t i = 0; i < lines_.size(); i++ ) {
  dist1_  = dist2_ = dist3_ = doca_ = false;

   //make it a 'good' line if the start OR end of the line is within 2.5 radii of the circle centre
        distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].first.x, 2.0) +
                             std::pow(circleCenter_.y - lines_[i].first.y, 2.0));
	//        if (distance < expectedCircleRadius_*2.5) goodLines_.push_back(lines_[i]);
        if (distance < expectedCircleRadius_*6.0) dist1_ = true;


	 distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].second.x, 2.0) +
	                   std::pow(circleCenter_.y - lines_[i].second.y, 2.0));
	 //  if (distance < expectedCircleRadius_*2.5) goodLines_.push_back(lines_[i]);
          if (distance < expectedCircleRadius_*6.0)  dist2_ = true  ;


	                           
x0 = circleCenter_.x;
y0 = circleCenter_.y;
x1 =  lines_[i].first.x;
y1 =  lines_[i].first.y;
x2 =  lines_[i].second.x;
y2 =  lines_[i].second.y;

doca =  std::fabs( x0*(y2-y1)  -  y0*(x2-x1) + x2*y1 -y2*x1)/ std::sqrt(   std::pow(y2 - y1,2.0)   +    std::pow(x2 - x1,2.0)      );


 //int measured_radius = cvRound(circles[i][2]);

 //std::cout <<" measured radius  = = = = = = "<<  radius  << std::endl
 // if ((doca <expectedCircleRadius_*1.3  && doca >expectedCircleRadius_*0.7)  || (doca <expectedCircleRadius_*6.5  && doca >expectedCircleRadius_*6.0)   ) goodLines_.push_back(lines_[i]);
  if ((doca <expectedCircleRadius_*5.8  && doca >expectedCircleRadius_*5.4)  || (doca <expectedCircleRadius_*1.3  && doca >expectedCircleRadius_*0.7)   ) doca_ = true;

  //if ( (doca <expectedCircleRadius_*5.7)  && (doca >expectedCircleRadius_*5.5)   ) doca_ = true;


 std::cout<<" doca = " << doca/expectedCircleRadius_ << "   doca _ "<<  doca_ <<std::endl;

  if (dist1_ && dist2_ && doca_ ) goodLines_.push_back(lines_[i]);
 // if ( doca_ == true) goodLines_.push_back(lines_[i]);

       }

    for (size_t i = 0; i < lines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(lines_[i].first.x), cvRound(lines_[i].first.y)),
                 cv::Point(cvRound(lines_[i].second.x), cvRound(lines_[i].second.y)),
                 cv::Scalar(255, 0, 0), 2, CV_AA);
    }

        for (size_t i = 0; i < goodLines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(goodLines_[i].first.x), cvRound(goodLines_[i].first.y)),
                     cv::Point(cvRound(goodLines_[i].second.x), cvRound(goodLines_[i].second.y)),
                 cv::Scalar(255, 0, 255), 4, CV_AA);
     }


	//find and draw all intersections between goodlines
    for (size_t i = 0; i < goodLines_.size()-1; i++) {
        for (size_t j = i+1; j < goodLines_.size(); j++) {

            cv::Point2f theIntersection;
	    cv::Point2f o1,o2,p1,p2,r;
            double distance;
	    o1 = goodLines_[i].first;
	    p1 = goodLines_[i].second;
	    o2 = goodLines_[j].first;
	    p2 = goodLines_[j].second;

	    cv::Point2f x = o2 - o1;
	    cv::Point2f d1 = p1 - o1;
	    cv::Point2f d2 = p2 - o2;

	    double cross = d1.x*d2.y - d1.y*d2.x;

	    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
	    r = o1 + d1 * t1;
	    circle( img_rgb, r, 10, cv::Scalar(0,255,255), 3, 8, 0 );
        }
    }




	  std::string filename = "/home/keaveney/Desktop/calibration/set_expos/im_scan___Exp" + str_ii + "___EdgeThr"  + str_p +"___lt"+str_lt +"___pc"+str_pc+".png";
	  cv::imwrite(filename, img_rgb);
          NQLog("AssemblyVUEyeCamera") << "  found marker";
     }
 }
    /*
    for(int p = 4; p< 7; p++){
      for(int q = 4; q< 7; q++){
	for(int r = 2; r< 3; r++){
	  int pp  = p*10;
	  int qq  = q*10;
	  int rr  = (r*10) + 60 ;
	  loopFind_->setCircleEdgeDetectionThreshold(pp);
          loopFind_->setCircleCenterDetectionThreshold(qq);
          loopFind_->setExpectedCircleSize(rr);
       	  loopFind_->findMarker(img_gs);
	}
      }
    }
    */


       }
       else{
   NQLog("AssemblyVUEyeCamera") << " ERROR FREEZE";

	 // qDebug() << "ERROR FREEZE";
       }
}
}
  is_ExitCamera(hCam);





   //	acquireImage();
   //    findMarker(frame);
   //   }

	/*      for (int i = 1; i<3;i++){
	NQLog("       ")  ;
	NQLog("       ")  ;
	NQLog("AssemblyVUEyeCamera") << " :exposure time scan, Step  = "<< i ;
	setExposureTime(i*50.0);
	NQLog("       ")  ;
	NQLog("AssemblyVUEyeCamera") << " :exposure time set, sleeping";
	sleep(10);
	NQLog("AssemblyVUEyeCamera") << " awake, acquiring image";
	acquireImage();
	NQLog("       ")  ;
	NQLog("AssemblyVUEyeCamera") << " image acquired, back to shleep ";
	sleep(60);
	NQLog("       ")  ;
	NQLog("AssemblyVUEyeCamera") << " awake, recycling ";
      }
	*/

}
