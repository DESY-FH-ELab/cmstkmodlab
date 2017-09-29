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

#include <iostream>
#include <cmath>

#include <QApplication>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "AssemblySensorMarkerFinder.h"

#include <TGraph.h>
#include <TCanvas.h>
#include <TH1F.h>


using namespace cv;
using namespace std;


AssemblySensorMarkerFinder::AssemblySensorMarkerFinder(QObject *parent)
    : AssemblyVMarkerFinder(parent)
{
    ApplicationConfig* config = ApplicationConfig::instance();

    gaussianBlurKernelSize_ = config->getValue<int>("SensorMarkerGaussianBlurKernelSize", 9);
    gaussianBlurSigma_ = config->getValue<int>("SensorMarkerGaussianBlurSigma", 2);

    circleEdgeDetectionThreshold_ = config->getValue<int>("SensorMarkerCircleEdgeDetectionThreshold", 70);
    circleCenterDetectionThreshold_ = config->getValue<int>("SensorMarkerCircleCenterDetectionThreshold", 40);
    expectedCircleRadius_ = config->getValue<double>("SensorMarkerExpectedCircleRadius", 82.0);

    linesCannyEdgeDetectionThreshold1_ = config->getValue<int>("SensorMarkerCannyEdgeDetectionThreshold1", 50);
    linesCannyEdgeDetectionThreshold2_ = config->getValue<int>("SensorMarkerCannyEdgeDetectionThreshold2", 200);
    linesCannyEdgeDetectionApertureSize_ = config->getValue<int>("SensorMarkerCannyEdgeDetectionApertureSize", 3);
    linesCannyEdgeDetectionL2Gradient_ = (bool)config->getValue<int>("SensorMarkerCannyEdgeDetectionL2Gradient", 1);

    linesHoughDistanceResolution_ = config->getValue<double>("SensorMarkerLinesHoughDistanceResolution", 1.0);
    linesHoughAngleResolution_ = config->getValue<double>("SensorMarkerLinesHoughAngleResolution", 0.1);
    linesHoughThreshold_ = config->getValue<int>("SensorMarkerLinesHoughThreshold", 75);
    linesHoughMinLineLength_ = config->getValue<double>("SensorMarkerLinesHoughMinLineLength", 50);
    linesHoughMaxLineGap_ = config->getValue<double>("SensorMarkerLinesHoughMaxLineGap", 25);

    generalThreshold = 200;   //default threshold value


#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString cachedirTemp = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
    QString cachedirTemp = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
    cachedirTemp += "/assembly/calibration";
    QDir dir1(cachedirTemp);
    if (!dir1.exists()) dir1.mkpath(".");
    cacheDirectory1_ = cachedirTemp.toStdString();
    cachedirTemp += "/RotatedImages";
    QDir dir2(cachedirTemp);
    if (!dir2.exists()) dir2.mkpath(".");
    cacheDirectory2_ = cachedirTemp.toStdString();

}


AssemblySensorMarkerFinder::~AssemblySensorMarkerFinder()
{
    NQLog("AssemblySensorMarkerFinder") << "delete";
}





void AssemblySensorMarkerFinder::runObjectDetection(int labmode, int objectmode)
{
    
  NQLog("AssemblySensorMarkerFinder::runObjectDetection(), labmode = ") << labmode   << ", objectmode = "<< objectmode ;

    labmode_g = labmode;
    objectmode_g = objectmode;

    if(labmode == 1){
        
    emit acquireImage();
        
    }else if (labmode == 0){
        
        //standard signals to launch PatRec here
        int match_method;
        
            if(objectmode==0){
                
                //img = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/RawSensor_4.png",
                  //               CV_LOAD_IMAGE_COLOR);
				img = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/SensorPiece_1.png",
                                 CV_LOAD_IMAGE_COLOR);
                
              //img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/RawSensor_3_clipB_temp.png",
                //                       CV_LOAD_IMAGE_COLOR);
				img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/SensorPiece_1_clipC.png",
                                       CV_LOAD_IMAGE_COLOR);

              // img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/spacer_corner_tempate_crop.png",
                //                       CV_LOAD_IMAGE_COLOR);
            }
            
            else if (objectmode == 1 ){
                
                NQLog("AssemblySensorLocator") << "***DETECTION OF POSITIONING PIN NOT IMPLMENTED YET!!***" ;
            }
            
            else if (objectmode == 2){
                
                img = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/glassslidecorneronbaseplate_sliverpaint_A.png",
                                 CV_LOAD_IMAGE_COLOR);
                                
                img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/glassslidecorneronbaseplate_sliverpaint_A_clip.png",CV_LOAD_IMAGE_COLOR);
                
            }
        
         emit locatePickupCorner_templateMatching(img,img_clip_A);
        NQLog("AssemblySensorMarkerFinder::runObjectDetection()") << " emit locatePickupCorner_templateMatching " ;
        
    }
}

void AssemblySensorMarkerFinder::runObjectDetection_labmode(cv::Mat master_image){
    
    
    NQLog("AssemblySensorLocator::runObjectDetection() here ") << "" ;
    NQLog("AssemblySensorLocator::runObjectDetection_labmode()") << "" ;

     //objectmode_g = 0;    //hard coding for lab tests, to be reomoved!!!! 

    if(objectmode_g == 0){
      NQLog("AssemblySensorLocator") << "***DETECTIING FIDUCIAL MARKER!***" ;

        img = master_image;
        
	//        img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/RawSensor_3_clipB.png", CV_LOAD_IMAGE_COLOR);
     //   img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/RawSensor_3_clipB_temp.png", CV_LOAD_IMAGE_COLOR);
		img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/SensorPiece_1_clipC.png", CV_LOAD_IMAGE_COLOR);
    }
    
    else if (objectmode_g == 1 ){
        
        NQLog("AssemblySensorLocator") << "***DETECTION OF POSITIONING PIN NOT IMPLMENTED YET!!***" ;
    }
    
    else if (objectmode_g == 2){
        
        NQLog("AssemblySensorLocator") << "***DETECTIING SILVER PAINTER CORNER!***" ;

        img = master_image;   
	//        img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/glassslidecorneronbaseplate_sliverpaint_A_clip.png",CV_LOAD_IMAGE_COLOR);
	// img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/glassslidecorner_sliverpaint_D_crop.png",CV_LOAD_IMAGE_COLOR);
           img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/simplecorner.png",CV_LOAD_IMAGE_COLOR);
      
    }

	else if (objectmode_g == 3){
         NQLog("AssemblySensorLocator") << "***DETECTIING SPACER  CORNER!***" ;
         img = master_image;   
         img_clip_A = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/spacer_corner_tempate_crop.png",
                                       CV_LOAD_IMAGE_COLOR);
     }

    emit locatePickupCorner_templateMatching(img,img_clip_A);
}


void AssemblySensorMarkerFinder::findMarker(const cv::Mat& image)
{

    NQLog("AssemblySensorMarkerFinder") << "findMarker()";

    QMutexLocker lock(&mutex_);
    image.copyTo(image_);

    lock.unlock();
 
    if (image_.channels()==1) {
       cvtColor(image_, imageRGB_, CV_GRAY2RGB);
    } else {
       image_.copyTo(imageRGB_);
    }
  
    cv::GaussianBlur(image, image_,
                     cv::Size(gaussianBlurKernelSize_, gaussianBlurKernelSize_),
                     gaussianBlurSigma_, gaussianBlurSigma_);

    size_t ret = findCircle(image_);


    if (ret==0) {
        emit markerFound(imageRGB_);
        return;
    }
    NQLog("AssemblySensorMarkerFinder") << "found circle";


    ret = findLines();
    NQLog("AssemblySensorMarkerFinder") << "found lines";

    ret = findIntersections();

    NQLog("AssemblySensorMarkerFinder") << "found intersections";
    determineOrientation();

    NQLog("AssemblySensorMarkerFinder") << "orientation determined";

    drawCircle();
    NQLog("AssemblySensorMarkerFinder") << "circles drawn";

    drawLines();
    NQLog("AssemblySensorMarkerFinder") << "lines drawn";

    //  drawOutline();
    NQLog("AssemblySensorMarkerFinder") << "outlines drawn";

    //drawIntersections();
    NQLog("AssemblySensorMarkerFinder") <<"intersections drawn";

    // drawOrientation();

    NQLog("AssemblySensorMarkerFinder") << "shapes drawn";

    NQLog("   ") ;
    NQLog("   ") ;
    NQLog("   ") ;


    emit markerFound(imageRGB_);
}

size_t AssemblySensorMarkerFinder::findCircle(const cv::Mat& image)
{

    global_image_ptr = &image;

    circles_.clear();
    circleQuality_ = 1.0;

    std::vector<cv::Vec3f> circles;

    QMutexLocker lock(&mutex_);

    /// Apply the Hough Transform to find the circles
    cv::HoughCircles(image, circles, CV_HOUGH_GRADIENT,
                     1, 20,
                     circleEdgeDetectionThreshold_, circleCenterDetectionThreshold_,
                     expectedCircleRadius_ - 15., expectedCircleRadius_ + 15.);


    lock.unlock();
    NQLog("AssemblySensorMarkerFinder") << " N circles = " << circles.size()  ;


    if (circles.size()==0) {
        circleQuality_ = 0.0;
        return 0;
    }

    if (circles.size()==1) {
        circleCenter_.x = circles[0][0];
        circleCenter_.y = circles[0][1];
        circleRadius_ = circles[0][2];
    NQLog("AssemblySensorMarkerFinder") << " getting  circle qual"  ;


        circleQuality_ = 1.0 - std::fabs(circleRadius_-expectedCircleRadius_)/expectedCircleRadius_;

    NQLog("AssemblySensorMarkerFinder") << " got circle qual"  ;

            circles_.push_back(AssemblyMarkerCircle(circleCenter_.x,
                                                circleCenter_.y,
                                               circleRadius_,
                                               circleQuality_));
    NQLog("AssemblySensorMarkerFinder") << " got circle qual 2"  ;

        return 1;
    }

    NQLog("AssemblySensorMarkerFinder") << " before radius map"  ;

    std::map<double,cv::Vec3f> radiusMap;
    for(size_t i = 0; i < circles.size(); i++ ) {
        radiusMap[std::fabs(circles[i][2]-expectedCircleRadius_)] = circles[i];
    }
    NQLog("AssemblySensorMarkerFinder") << " after radius map"  ;

    std::map<double,cv::Vec3f>::iterator it = radiusMap.begin();

    circleCenter_.x = it->second[0];
    circleCenter_.y = it->second[1];
    circleRadius_ = it->second[2];

    circleQuality_ = 1.0;
    circleQuality_ *= 1.0-std::fabs(circleRadius_-expectedCircleRadius_)/expectedCircleRadius_;

    ++it;
    double minDistance = 1.e9;
    for (;it!=radiusMap.end();++it) {
        double distance = cv::norm(circleCenter_ - cv::Point2f(it->second[0], it->second[1]));
        if (distance < minDistance) minDistance = distance;

        circles_.push_back(AssemblyMarkerCircle(it->second[0],
                                                it->second[1],
                                                it->second[2],
                                                1.0-std::fabs(circleRadius_-expectedCircleRadius_)/expectedCircleRadius_));

    }
    if (minDistance==0) {
        circleQuality_ = 0.0;
    } else if (minDistance<=2*circleRadius_) {
        circleQuality_ *= 1.0 - minDistance/(2*circleRadius_);
    }
    NQLog("AssemblySensorMarkerFinder") << " returning circles "  ;

    return circles.size();
}

void AssemblySensorMarkerFinder::drawCircle()
{
    cv::Point2f center(cvRound(circleCenter_.x),
                       cvRound(circleCenter_.y));

    cv::Mat local_image = *global_image_ptr;
    // circle center
      cv::circle(local_image, center, 8, cv::Scalar(0, 255, 0), -1, CV_AA, 0);
    // circle outline
      cv::circle(local_image, center, circleRadius_, cv::Scalar(0, 0, 255), 4, CV_AA, 0);

}

size_t AssemblySensorMarkerFinder::findLines()
{
    cv::Canny(image_, imageEdges_,
              linesCannyEdgeDetectionThreshold1_,
              linesCannyEdgeDetectionThreshold2_,
              linesCannyEdgeDetectionApertureSize_,
              linesCannyEdgeDetectionL2Gradient_);

    emit edgesDetected(imageEdges_);

    std::vector<cv::Vec4i> tempLines;

    lines_.clear();
    goodLines_.clear();

    cv::HoughLinesP(imageEdges_, tempLines,
                    linesHoughDistanceResolution_,
                    linesHoughAngleResolution_*CV_PI/180,
                    linesHoughThreshold_,
                    linesHoughMinLineLength_,
                    linesHoughMaxLineGap_);

    for (size_t i = 0; i < tempLines.size(); i++ ) {
        lines_.push_back(std::pair<cv::Point2f,cv::Point2f>(cv::Point2f(tempLines[i][0],tempLines[i][1]),
                                                            cv::Point2f(tempLines[i][2],tempLines[i][3])));
    }

    double distance;
    for (size_t i = 0; i < lines_.size(); i++ ) {
        distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].first.x, 2.0) +
                             std::pow(circleCenter_.y - lines_[i].first.y, 2.0));
        if (distance < expectedCircleRadius_*2.5) goodLines_.push_back(lines_[i]);

        distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].second.x, 2.0) +
                             std::pow(circleCenter_.y - lines_[i].second.y, 2.0));
        if (distance < expectedCircleRadius_*2.5) goodLines_.push_back(lines_[i]);
    }

    return goodLines_.size();
}

void AssemblySensorMarkerFinder::drawLines()
{
    for (size_t i = 0; i < lines_.size(); i++ ) {
        cv::line(imageRGB_,
                 cv::Point(cvRound(lines_[i].first.x), cvRound(lines_[i].first.y)),
                 cv::Point(cvRound(lines_[i].second.x), cvRound(lines_[i].second.y)),
                 cv::Scalar(255, 0, 0), 2, CV_AA);
    }
    for (size_t i = 0; i < goodLines_.size(); i++ ) {
        cv::line(imageRGB_,
                 cv::Point(cvRound(goodLines_[i].first.x), cvRound(goodLines_[i].first.y)),
                 cv::Point(cvRound(goodLines_[i].second.x), cvRound(goodLines_[i].second.y)),
                 cv::Scalar(255, 0, 255), 4, CV_AA);
    }
 }

bool AssemblySensorMarkerFinder::intersection(cv::Point2f o1, cv::Point2f p1,
                                              cv::Point2f o2, cv::Point2f p2,
                                              cv::Point2f &r, double distance)
{
  cv::Point2f x = o2 - o1;
  cv::Point2f d1 = p1 - o1;
  cv::Point2f d2 = p2 - o2;

  double cross = d1.x*d2.y - d1.y*d2.x;
  if (std::abs(cross) < 1.e-8)
    return false;

  double t1 = (x.x * d2.y - x.y * d2.x)/cross;
  r = o1 + d1 * t1;

  if (cv::norm(r - o1) > distance &&
      cv::norm(r - p1) > distance) return false;

  if (cv::norm(r - o2) > distance &&
      cv::norm(r - p2) > distance) return false;

  return true;
}

void AssemblySensorMarkerFinder::mergeIntersections(std::vector<cv::Point2f>& intersections)
{
    std::vector<cv::Point2f> tempIntersections;
    std::vector<bool> intersectionFlag(intersections.size());

    for (size_t i = 0; i < intersections.size(); i++ ) intersectionFlag[i] = false;

    for (size_t i = 0; i < intersections.size(); i++ ) {

        if (intersectionFlag[i]) continue;

        double sx = 0, sy = 0;
        int n = 0;

        sx += intersections[i].x;
        sy += intersections[i].y;
        intersectionFlag[i] = true;
        n++;

        if (i<intersections.size()-1) {
            for (size_t j = i+1; j < intersections.size(); j++ ) {
                if (intersectionFlag[j]) continue;

                if (cv::norm(intersections[i] - intersections[j]) < 1) {
                    sx += intersections[j].x;
                    sy += intersections[j].y;
                    intersectionFlag[j] = true;
                    n++;
                }
            }
        }
        tempIntersections.push_back(cv::Point2f(sx/n, sy/n));
    }
    intersections = tempIntersections;
}

size_t AssemblySensorMarkerFinder::findIntersections()
{
    intersections_.clear();
    goodIntersections_.clear();

    if (goodLines_.size()<2) return 0;

    for (size_t i = 0; i < goodLines_.size()-1; i++) {
        for (size_t j = i+1; j < goodLines_.size(); j++) {

            cv::Point2f theIntersection;

            if (intersection(goodLines_[i].first, goodLines_[i].second,
                             goodLines_[j].first, goodLines_[j].second,
                             theIntersection, expectedCircleRadius_)) {
                goodIntersections_.push_back(theIntersection);
            }
        }
    }
    mergeIntersections(goodIntersections_);

    for (size_t i = 0; i < goodLines_.size(); i++ ) {
        for (size_t j = 0; j < lines_.size(); j++ ) {

            if (goodLines_[i]==lines_[j]) continue;

            cv::Point2f theIntersection;

            if (intersection(goodLines_[i].first, goodLines_[i].second,
                             lines_[j].first, lines_[j].second,
                             theIntersection, expectedCircleRadius_)) {
                intersections_.push_back(theIntersection);
            }
        }
    }
    mergeIntersections(intersections_);

    std::vector<cv::Point2f> tempIntersections;
    for(size_t i = 0; i < intersections_.size(); i++ ) {
        bool keep = true;
        for(size_t j = 0; j < goodIntersections_.size(); j++ ) {
            if (cv::norm(goodIntersections_[j] - intersections_[i]) < 10) keep = false;
        }
        if (keep) tempIntersections.push_back(intersections_[i]);
    }
    intersections_ = tempIntersections;

    return intersections_.size();
}

void AssemblySensorMarkerFinder::drawIntersections()
{
    for(size_t i = 0; i < intersections_.size(); i++ ) {
        cv::Point center(cvRound(intersections_[i].x), cvRound(intersections_[i].y));
      circle(imageRGB_, center, 8, cv::Scalar(0, 0, 255), -1, CV_AA, 0 );
    }

    for(size_t i = 0; i < goodIntersections_.size(); i++ ) {
        cv::Point center(cvRound(goodIntersections_[i].x), cvRound(goodIntersections_[i].y));
      circle(imageRGB_, center, 8, cv::Scalar(0, 255, 0), -1, CV_AA, 0 );
    }
}

void AssemblySensorMarkerFinder::determineOrientation()
{
    if (goodIntersections_.size()!=2 && intersections_.size()!=4) return;

    const cv::Point2f& cp = circleCenter_;
    cv::Point2f gp0 = goodIntersections_[0];
    cv::Point2f gp1 = goodIntersections_[1];

    cv::Point2f igp, ogp;

    if (cv::norm(gp0 - cp) < cv::norm(gp1 - cp)) {
        igp = gp0;
        ogp = gp1;
    } else {
        igp = gp1;
        ogp = gp0;
        goodIntersections_[0] = gp1;
        goodIntersections_[1] = gp0;
    }

    const cv::Point2f vg = igp - ogp;
    cv::Point2f il1, ol1;
    cv::Point2f il2, ol2;
    double avg = 180.0*std::atan2(vg.y, vg.x)/CV_PI;
    if (avg<0) avg += 360.0;
    double angle; //, dot, dv;

    for(size_t i = 0; i < intersections_.size(); i++ ) {
        cv::Point2f v = intersections_[i] - igp;
        // dot = v.dot(vg);
        // dv = cv::norm(v);
        angle = 180.0*std::atan2(v.y, v.x)/CV_PI;
        if (angle<0) angle += 360.0;
        angle -= avg;

        if (angle > 44.5 && angle < 45.5) {
            il1 = intersections_[i];
            // cv::Point center(cvRound(intersections_[i].x), cvRound(intersections_[i].y));
            // circle(imageRGB_, center, 6, cv::Scalar(0, 255, 0), -1, CV_AA, 0 );
        }
        if (angle > -45.5 && angle < -44.5) {
            il2 = intersections_[i];
            // cv::Point center(cvRound(intersections_[i].x), cvRound(intersections_[i].y));
            // circle(imageRGB_, center, 6, cv::Scalar(0, 255, 0), -1, CV_AA, 0 );
        }
    }

    for(size_t i = 0; i < intersections_.size(); i++ ) {
        cv::Point2f v = intersections_[i] - ogp;
        // dot = v.dot(vg);
        // dv = cv::norm(v);
        angle = 180.0*std::atan2(v.y, v.x)/CV_PI;
        if (angle<0) angle += 360.0;
        angle -= avg;

        if (angle > 44.5 && angle < 45.5) {
            ol1 = intersections_[i];
            // cv::Point center(cvRound(intersections_[i].x), cvRound(intersections_[i].y));
            // circle(imageRGB_, center, 6, cv::Scalar(0, 255, 0), -1, CV_AA, 0 );
        }
        if (angle > -45.5 && angle < -44.5) {
            ol2 = intersections_[i];
            // cv::Point center(cvRound(intersections_[i].x), cvRound(intersections_[i].y));
            // circle(imageRGB_, center, 6, cv::Scalar(0, 255, 0), -1, CV_AA, 0 );
        }
    }

    intersections_[0] = il1;
    intersections_[1] = ol1;
    intersections_[2] = ol2;
    intersections_[3] = il2;

    orientation_ = 0.0;

    cv::Point2f d = igp - il1;
    angle = std::atan2(d.y, d.x);
    if (angle<0) angle += 2.*CV_PI;
    // NQLog("AssemblySensorMarkerFinder") << "ia1 " << angle;
    orientation_ += angle;

    d = ogp - ol1;
    angle = std::atan2(d.y, d.x);
    if (angle<0) angle += 2.*CV_PI;
    // NQLog("AssemblySensorMarkerFinder") << "oa1 " << angle;
    orientation_ += angle;

    d = igp - il2;
    angle = std::atan2(d.y, d.x) + CV_PI/2;
    if (angle<0) angle += 2.*CV_PI;
    // NQLog("AssemblySensorMarkerFinder") << "ia2 " << angle;
    orientation_ += angle;

    d = ogp - ol2;
    angle = std::atan2(d.y, d.x) + CV_PI/2;
    if (angle<0) angle += 2.*CV_PI;
    // NQLog("AssemblySensorMarkerFinder") << "oa2 " << angle;
    orientation_ += angle;

    orientation_ /= 4.;
}

void AssemblySensorMarkerFinder::drawOutline()
{
    cv::line(imageRGB_,
             goodIntersections_[0], intersections_[0],
             cv::Scalar(0, 255, 255), 4, CV_AA);
    cv::line(imageRGB_,
             intersections_[0], intersections_[1],
             cv::Scalar(0, 255, 255), 4, CV_AA);
    cv::line(imageRGB_,
             intersections_[1], goodIntersections_[1],
             cv::Scalar(0, 255, 255), 4, CV_AA);
    cv::line(imageRGB_,
             goodIntersections_[1], intersections_[2],
             cv::Scalar(0, 255, 255), 4, CV_AA);
    cv::line(imageRGB_,
             intersections_[2], intersections_[3],
             cv::Scalar(0, 255, 255), 4, CV_AA);
    cv::line(imageRGB_,
             intersections_[3], goodIntersections_[0],
             cv::Scalar(0, 255, 255), 4, CV_AA);
}

void AssemblySensorMarkerFinder::drawOrientation()
{
    double dx1 = circleRadius_/4 * std::cos(orientation_);
    double dy1 = circleRadius_/4 * std::sin(orientation_);
    double dx2 = 3*circleRadius_ * std::cos(orientation_);
    double dy2 = 3*circleRadius_ * std::sin(orientation_);

    cv::line(imageRGB_,
             cv::Point(cvRound(circleCenter_.x - dx1), cvRound(circleCenter_.y - dy1)),
             cv::Point(cvRound(circleCenter_.x - dx2), cvRound(circleCenter_.y - dy2)),
             cv::Scalar(255, 255, 0), 6, CV_AA);
}


//void AssemblySensorMarkerFinder::findMarker_templateMatching(cv::Mat img, cv::Mat img_clip_A, cv::Mat img_glass_marker_raw)
void AssemblySensorMarkerFinder::findMarker_templateMatching(cv::Mat img, cv::Mat img_clip_A)
{
    NQLog("AssemblySensorMarkerFinder") << "Finding Marker (Template Matching) here";

    NQLog("AssemblySensorMarkerFinder") << ", number of image channels =  "<<img_clip_A.channels() ;
      
    Point matchLoc_1, matchLoc_2, matchLoc_final;
    Mat result_1, result_2; 
    Mat img_copy = img.clone();
   

    //Greyscale images
    Mat img_copy_gs(img_copy.size(), img_copy.type());
    Mat img_clip_A_gs(img_clip_A.size(), img_clip_A.type());
    //Mat img_glass_marker_raw_gs(img_glass_marker_raw.size(), img_glass_marker_raw.type());

    if (img.channels()> 1){
    //convert color to GS
    cvtColor( img_copy,   img_copy_gs,   CV_BGR2GRAY );
    }else {
     img_copy_gs = img_copy.clone();
    }

    if (img_clip_A.channels()> 1){
    //convert color to GS
    cvtColor( img_clip_A, img_clip_A_gs, CV_BGR2GRAY );
    }else {
     img_clip_A_gs = img_clip_A.clone();
    }

    /*if (img_glass_marker_raw.channels()> 1){
    //convert color to GS
    cvtColor( img_glass_marker_raw, img_glass_marker_raw_gs, CV_BGR2GRAY );
    }else {
     img_glass_marker_raw_gs = img_glass_marker_raw.clone();
    }*/

    //Binary images
    Mat img_copy_bin(img_copy_gs.size(), img_copy_gs.type());
    Mat img_clip_A_bin(img_clip_A_gs.size(), img_clip_A_gs.type());
    //Mat img_glass_marker_bin(img_glass_marker_raw_gs.size(), img_glass_marker_raw_gs.type());
    
    //Apply thresholding
    cv::threshold(img_copy_gs, img_copy_bin, generalThreshold, 255, cv::THRESH_BINARY);
    cv::threshold(img_clip_A_gs, img_clip_A_bin, 85, 255, cv::THRESH_BINARY);    //90 for silicon marker, 88 for glass?
    //cv::threshold(img_glass_marker_raw_gs, img_glass_marker_bin, 88, 255, cv::THRESH_BINARY);
        
    // img_copy_bin = img_copy_gs.clone();
    // img_clip_A_bin = img_clip_A_gs.clone();
    
    std::string filename_img_bin = Config::CMSTkModLabBasePath + "/share/assembly/Sensor_bin.png";
    std::string filename_clip_A_bin = Config::CMSTkModLabBasePath + "/share/assembly/clip_A_bin.png";
    //std::string filename_img_glass_marker_bin = Config::CMSTkModLabBasePath + "/share/assembly/img_glass_marker_bin.png";
    
    cv::imwrite(filename_img_bin, img_copy_bin);
    cv::imwrite(filename_clip_A_bin, img_clip_A_bin);
    //cv::imwrite(filename_img_glass_marker_bin, img_glass_marker_bin);
    
    QString filename_img_bin_qs = QString::fromStdString(filename_img_bin);
    QString filename_clip_A_bin_qs = QString::fromStdString(filename_clip_A_bin);
    //QString filename_img_glass_marker_bin_qs = QString::fromStdString(filename_img_glass_marker_bin);
    
    //emit updateImage(4, filename_img_bin_qs);
    //emit updateImage(5, filename_clip_A_bin_qs);
    
    NQLog("AssemblySensorMarkerFinder") << "Finding Marker (Template Matching) wrote images" ;
    
    /// Localizing the best match with minMaxLoc
    double FOM, FOM_inc = 1000.0, minVal, maxVal; Point minLoc; Point maxLoc;
    double thetas[10], FOMs[10], best_theta;
    
    //create result matrix to hold correlation values
    int result_cols =  img_copy_bin.cols - img_clip_A.cols + 1;
    int result_rows = img_copy_bin.rows - img_clip_A.rows + 1;
    

    NQLog("AssemblySensorMarkerFinder") << ", Master cols = "<< img_copy_bin.cols << "  Master rows " << img_copy_bin.rows << " template cols << " << img_clip_A.cols << " template rows "<<img_clip_A.rows  ;


    // result_1.create( result_rows, result_cols, CV_32FC1 );
    result_1.create( result_rows, result_cols, CV_32FC1 );
    
    int match_method = 1;
    matchTemplate( img_copy_bin, img_clip_A_bin, result_1, match_method);
    
    NQLog("AssemblySensorMarkerFinder") << "Finding Marker (Template Matching), pre- matching routine" ;

    
    minMaxLoc( result_1, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    
    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED ) {
        matchLoc_1 = minLoc;
        FOM = minVal;
    } else {
        matchLoc_1 = maxLoc;
        FOM = maxVal;
    }
    
    NQLog("AssemblySensorMarkerFinder") << "Finding Marker (Template Matching), post- matching routine" ;

    
    //circle( img_gs, Point( matchLoc_1.x + (img_clip_gs.cols/2.0) , matchLoc_1.y + (img_clip_gs.rows/2.0) ), 10, Scalar(255,0,0) );
    
    //circle( img, Point( matchLoc_1.x  , matchLoc_1.y ), 30, Scalar(255,0,0) );
    //rectangle( img, matchLoc_1, Point( matchLoc_1.x + img_clip_A_bin.cols , matchLoc_1.y + img_clip_A_bin.rows ), Scalar(255,0,0), 2, 8, 0 );
    
    NQLog("AssemblySensorMarkerFinder") <<  matchLoc_1.x<< "   "<< matchLoc_1.y;
    
    
    int i = 0;
    int color = 200;
    
    
    //    for (float theta = 20; theta < 24;  theta = theta + 0.2){
      //    for (float theta = -64.0; theta < 64.0;  theta = theta + 3.2){
    //    for (float theta = -180.0; theta < 180.0;  theta = theta + 9.0){
      
    //     for (float theta = -5.0; theta < 5.0;  theta = theta + 0.25){
    //  for (float theta = -10.0; theta <= 10.0;  theta = theta + 1.0){
  
    //        for (float theta = -3.2; theta < 3.2;  theta = theta + 0.16){

    std::string filename_rotated;

    NQLog("AssemblySensorMarkerFinder") << "looping"<< "   ";
    float scan_start = -5.0, scan_end = 180.0, scan_step =  0.1;
    float FOM_near, FOM_far;
    float theta_coarse;

    //first check if we are at the "near" or "far" corner
   // for (float theta_coarse = 0.0; theta_coarse <= 180.0;  theta_coarse = theta_coarse + 180.0){
     for (int scan = 0; scan <= 2;  scan++){
         NQLog("AssemblySensorMarkerFinder") << "*** Scan number "<< scan;
        
         if (scan == 0){
            theta_coarse = 0.0;
            scan_start = 0.001, scan_end = 0.001, scan_step =  1.0;
         } else if (scan == 1){
             theta_coarse = 180.0;
             scan_start = 0.001, scan_end = 0.001, scan_step =  1.0;
         }
         
         else{
             
             NQLog("AssemblySensorMarkerFinder") << "*** Deciding corner: "<< " FOM_near  =  "<<FOM_near <<" FOM far =  "<< FOM_far ;

             //scan_start = -0.5, scan_end = 0.85, scan_step =  0.15;
             scan_start = -0.25, scan_end = 0.425, scan_step =  0.075;

             if (FOM_near < FOM_far){
                 NQLog("AssemblySensorMarkerFinder") << " @ Near corner" ;
                 theta_coarse = 0.0;
             }else if(FOM_near > FOM_far){
                 NQLog("AssemblySensorMarkerFinder") << " @ Far corner" ;
                 theta_coarse = 180.0;
             }
         }
        
    
	for (float theta_fine = scan_start; theta_fine <= scan_end;  theta_fine = theta_fine + scan_step){
        float theta = theta_coarse + theta_fine;

         Point2f src_center(img_copy_bin.cols/2.0F, img_copy_bin.rows/2.0F);
	  // Point2f src_center( matchLoc_1.x + (img_clip_A_bin.cols/2) , matchLoc_1.y + (img_clip_A_bin.rows/2) );
        
	 // Mat rot_mat = getRotationMatrix2D(matchLoc_1, theta, 1.0);
        Mat rot_mat = getRotationMatrix2D(src_center, theta, 1.0);
        Mat dst, dst_orig;
        cv::Scalar avgPixelIntensity = cv::mean( img_copy_bin );
        warpAffine(img_copy_bin, dst, rot_mat, img_copy_bin.size(), cv::INTER_CUBIC,
                   cv::BORDER_CONSTANT, avgPixelIntensity);
        warpAffine(img_copy_gs, dst_orig, rot_mat, img_copy_bin.size(), cv::INTER_CUBIC,
                   cv::BORDER_CONSTANT, avgPixelIntensity);
	//    std::string filename_rotated_base = cacheDirectory2_ + "/Rotation_result_";

	    std::string filename_rotated_base =  "Rotation_result_";
        std::ostringstream ss;
        ss << theta;
        std::string theta_str = ss.str();
        filename_rotated = filename_rotated_base + theta_str + ".png";
        cv::imwrite(filename_rotated, dst);
        
        //create result matrix to hold correlation values
        result_cols =  img_copy_bin.cols - img_clip_A_bin.cols + 1;
        result_rows = img_copy_bin.rows - img_clip_A_bin.rows + 1;
        
        result_2.create( result_rows, result_cols, CV_32FC1 );
        matchTemplate( dst, img_clip_A_bin, result_2, match_method );
        
        // normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
        minMaxLoc( result_2, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
        
        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED ) {
            matchLoc_2 = minLoc;
            FOM = minVal;
        } else {
            matchLoc_2 = maxLoc;
            FOM = maxVal;
        }
        
        std::cout << std::setprecision(10);     
        std::cout <<"theta = "<<  theta << ",  FOM = "<<  FOM<<  std::endl;
        

        if (scan == 0 ){
            FOM_near = FOM;
        }
        else if (scan == 1){
            FOM_far = FOM;
        } else {
            thetas[i] = theta;
            FOMs[i] = FOM;
            i++;

            if (FOM < FOM_inc ) {
                
                FOM_inc = FOM;
                matchLoc_final = matchLoc_2;
                best_theta = theta;
            }
        
        }
        
    
        
        color = 50 + 200*theta;
    }
         
         NQLog("AssemblySensorMarkerFinder") << " Finished fine scan best theta = "<<  best_theta ;

    }
    
    cv::Mat img_raw = img.clone();
    //  rectangle( img, matchLoc_final, Point( matchLoc_final.x + img_clip_A_bin.cols , matchLoc_final.y + img_clip_A_bin.rows ), Scalar(255,0,255), 2, 8, 0 );

    cv::Rect rect_result = cv::Rect( matchLoc_final, Point( matchLoc_final.x + img_clip_A_bin.cols, matchLoc_final.y + img_clip_A_bin.rows));

    line(img, Point(img.cols/2.0, 0), Point(img.cols/2.0, img.rows ), Scalar(255,255,0), 2, 8, 0);
    line(img, Point(0, img.rows/2.0), Point(img.cols, img.rows/2.0 ), Scalar(255,255,0), 2, 8, 0);
    
        if (best_theta > -5.0 && best_theta < 5.0){
            //the circle of radius 4 is meant to *roughly* represnt the x,y precision of the x-y motion stage so that the
            //use can see if the patrec results make sense (the top left corner of the marker should be within the cirle)
            line(img,Point(matchLoc_final.x,matchLoc_final.y - 50),Point(matchLoc_final.x + 240,matchLoc_final.y - 50 ),Scalar(0,255,0),2,8,0);
            putText(img, "200 um", Point(matchLoc_final.x, matchLoc_final.y - 100 ), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar(0,255,0), 3, 8);
            circle(img, matchLoc_final, 4, Scalar(255,0,255), 4, 8, 0 );
            circle(img, matchLoc_final, 4, Scalar(255,0,255), 4, 8, 0 );
            rectangle(img, matchLoc_final, Point( matchLoc_final.x + img_clip_A_bin.cols, matchLoc_final.y + img_clip_A_bin.rows ), Scalar(255,0,255), 2, 8, 0);

     } else if (best_theta > 175.0 && best_theta < 185.0){
         line(img, Point( img.cols - matchLoc_final.x - 240 ,  img.rows - matchLoc_final.y - 100 ), Point( img.cols - matchLoc_final.x,  img.rows - matchLoc_final.y - 100 ), Scalar(0,255,0), 2, 8, 0);
         putText(img, "200 um", Point( img.cols - matchLoc_final.x - 240, img.rows - matchLoc_final.y - 50 ), FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Scalar(0,255,0), 3, 8);
         circle( img,  Point( img.cols - matchLoc_final.x ,  img.rows - matchLoc_final.y ) , 4, Scalar(255,0,255), 4, 8, 0 );
         rectangle(img, Point( img.cols - matchLoc_final.x,  img.rows - matchLoc_final.y ),  Point( img.cols - matchLoc_final.x  - img_clip_A_bin.cols,  img.rows - matchLoc_final.y - img_clip_A_bin.rows ) , Scalar(255,0,255), 2, 8, 0 );
     }

    //     circle( dst_orig, matchLoc_final, 10, Scalar(0,0,255), 4, 8, 0 );
    // rectangle(dst_orig, matchLoc_final, Point( matchLoc_final.x + img_clip_A_bin.cols , matchLoc_final.y + img_clip_A_bin.rows ), Scalar(255,0,255), 2, 8, 0 );
  

    TCanvas *c1 = new TCanvas("c1","Rotation extraction",200,10,700,500);
    
    if (thetas[0] && FOMs[0]){
        TGraph *gr = new TGraph(10, thetas, FOMs);

        //gr->Fit("pol6");
        gr->Draw("AC*");
        gr->GetHistogram()->GetXaxis()->SetTitle("Rotational tranformation (degrees)");
        gr->GetHistogram()->GetYaxis()->SetTitle("Minimised metric value");
        gr->GetHistogram()->SetTitle("");

        TGraph *gr_bestmatch = new TGraph(1);
        gr_bestmatch->SetPoint(0, best_theta, FOM_inc);
        gr_bestmatch->SetMarkerColor(2);
        gr_bestmatch->SetMarkerStyle(22);
        gr_bestmatch->SetMarkerSize(3);

        gr_bestmatch->Draw("PSAME");

        std::string filename_canvas = cacheDirectory1_ + "/RotationExtraction.png";
        c1->SaveAs(filename_canvas.c_str());
        QString filename_canvas_qs = QString::fromStdString(filename_canvas);
        c1->SaveAs("test.png");

        
        emit updateImage(2, filename_canvas_qs);
    }
    
    std::string filename = cacheDirectory1_ + "/PatRec_TM_result.png";
    cv::imwrite(filename, img);
    
 
    QString filename_qs = QString::fromStdString(filename);
    QString filename_template = QString::fromStdString(filename_clip_A_bin);
    QString filename_master = QString::fromStdString(filename_img_bin);
    QString filename_rotated_qs = QString::fromStdString(filename_rotated);
    
    emit updateImage(1, filename_qs);
    
    emit foundSensor(1);
    emit getImageBlur(img_raw, rect_result);

    emit updateImage(3, filename_template);
    emit updateImage(4, filename_master);
    
    
    //work out match location in field of view
    // the origin of the FOV coordinate system is the top left corner
    //the match loction (centre of the template) is calculated in mm
    //this should be enough for postion correction with moverealtive()
    

    //    matchLoc_x_lab = (matchLoc_final.x +  (img_clip_A_bin.cols/2) ) * (5.0/img.cols); // need to add the current X pos of the lang
    //matchLoc_y_lab = (matchLoc_final.y +  (img_clip_A_bin.rows/2) ) * (4.0/img.rows); // need to add the current Y pos of the lang
    
    //update line edits in view
    
    emit reportObjectLocation(1, matchLoc_final.x , matchLoc_final.y, best_theta);
    
}

void AssemblySensorMarkerFinder::findMarker_circleSeed(int mode)
{

    cv::Mat img_gs, img_rgb, img_edges;
    
    
     if (mode == 0) {
    
         NQLog("AssemblySensorMarkerFinder") << "DEMO MODE (USING DEMO IMAGES)" ;

    img_gs = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/im_scan___Exp10___EdgeThr145___lt110.png",
                        CV_LOAD_IMAGE_COLOR);
    
     }else if (mode ==1){
     
         
         NQLog("AssemblySensorMarkerFinder") << "***LAB MODE NOT IMPLMENTED YET....REVERTING TO DEMO MODE!!!***" ;

         img_gs = cv::imread(Config::CMSTkModLabBasePath + "/share/assembly/im_scan___Exp10___EdgeThr145___lt110.png",
                             CV_LOAD_IMAGE_COLOR);
     
     
     }
    
         
    cvtColor(img_gs, img_gs , CV_RGB2GRAY);
    
    int radius;
    int  expectedCircleRadius_ = 89;
    double circleCenterDetectionThreshold_  = 35.0;
    double circleEdgeDetectionThreshold_  = 145.0;
    double linesCannyEdgeDetectionThreshold1_ =50;
    double linesCannyEdgeDetectionThreshold2_ =200;
    double linesCannyEdgeDetectionApertureSize_ =3;
    double linesCannyEdgeDetectionL2Gradient_ =1;
    double linesHoughDistanceResolution_ = 1.0;
    //    double linesHoughAngleResolution_*CV_PI/180,
    double linesHoughAngleResolution_ = 0.1;
    double linesHoughThreshold_ = 110.0;
    double linesHoughMinLineLength_=  150.0;
    double linesHoughMaxLineGap_ =25.0;
    bool  dist1_, dist2_, dist3_, doca_ ;
    double slope_final = 0.0, ang_final = 0.0;
    double distance,doca,x0,x1,x2, y0,y1,y2, x,y;
    
    std::vector<cv::Vec3f> circles;
    cv::Point2f circleCenter_;
    
    std::vector<cv::Vec4i> tempLines;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > lines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > goodLines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > mergedLines_;
    
    std::vector<cv::Point2f> intersections_;
    std::vector<cv::Point2f> goodIntersections_;
    std::vector<cv::Point2f> finalIntersections_;
    std::vector<cv::Point2f> finalIntersectionsUp_;
    std::vector<cv::Point2f> finalIntersectionsDown_;
    
    lines_.clear();
    goodLines_.clear();
    intersections_.clear();
    goodIntersections_.clear();
    finalIntersections_.clear();
    
    //detect circles
    cv::HoughCircles(img_gs, circles, CV_HOUGH_GRADIENT, 1, 20,
                     circleEdgeDetectionThreshold_, circleCenterDetectionThreshold_,
                     expectedCircleRadius_ - 15., expectedCircleRadius_ + 15.);
    
    //get centre of first circle...problematic if this is not the right one.
    if (circles.size() != 0){
        circleCenter_.x = circles[0][0];
        circleCenter_.y = circles[0][1];
    }
    
    
    cvtColor(img_gs, img_rgb , CV_GRAY2RGB);
    
    /// Draw the circles detected in blue
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        x = cvRound(circles[i][0]);
        y = cvRound(circles[i][1]);
        radius = cvRound(circles[i][2]);
        // circle center
        circle( img_rgb, center, 3, cv::Scalar(0,0,255), -1, 8, 0 );
        //circle outline;
        circle( img_rgb, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
    }
    
    //detect edges
    cv::Canny(img_gs, img_edges,
              linesCannyEdgeDetectionThreshold1_,
              linesCannyEdgeDetectionThreshold2_,
              linesCannyEdgeDetectionApertureSize_,
              linesCannyEdgeDetectionL2Gradient_);
    
    NQLog("AssemblyVUEyeCamera") << "  running hough lines";
    
    //detect lines
    cv::HoughLinesP(img_edges, tempLines,
                    linesHoughDistanceResolution_,
                    linesHoughAngleResolution_*CV_PI/180,
                    linesHoughThreshold_,
                    linesHoughMinLineLength_,
                    linesHoughMaxLineGap_);
    
    
    for (size_t i = 0; i < tempLines.size(); i++ ) {
        lines_.push_back(std::pair<cv::Point2f,cv::Point2f>(cv::Point2f(tempLines[i][0],tempLines[i][1]), cv::Point2f(tempLines[i][2],tempLines[i][3])));
    }
    
    
    //draw the initial collection of lines in red (handy for debugging, monitoring of PatRec)
    for (size_t i = 0; i < lines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(lines_[i].first.x), cvRound(lines_[i].first.y)),
                 cv::Point(cvRound(lines_[i].second.x), cvRound(lines_[i].second.y)),
                 cv::Scalar(255, 0, 0), 2, CV_AA);
    }
    
    
    //select only the lines which have starting/ending points close to the circle centre and DOCA consistent with the L marker outline
    for (size_t i = 0; i < lines_.size(); i++ ) {
        dist1_  = dist2_ = dist3_ = doca_ = false;
        distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].first.x, 2.0) +
                             std::pow(circleCenter_.y - lines_[i].first.y, 2.0));
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
        
        
        if ((doca <expectedCircleRadius_*5.8  && doca >expectedCircleRadius_*5.4)  || (doca <expectedCircleRadius_*1.3  && doca >expectedCircleRadius_*0.7)   ) doca_ = true;
        
        if (dist1_ && dist2_ && doca_ ) goodLines_.push_back(lines_[i]);
    }
    
    //draw the initial collection of "good" lines in pink (handy for debugging, monitoring of PatRec)
    
    for (size_t i = 0; i < goodLines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(goodLines_[i].first.x), cvRound(goodLines_[i].first.y)),
                 cv::Point(cvRound(goodLines_[i].second.x), cvRound(goodLines_[i].second.y)),
                 cv::Scalar(255, 0, 255), 4, CV_AA);
    }
    
    
    //find all intersections between goodlines that intersect @ ~ 90 degrees
    for (size_t i = 0; i < goodLines_.size(); i++) {
        for (size_t j = 0; j < goodLines_.size(); j++) {
            
            if (i == j ) continue;
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
            double slope1 = (p1.y - o1.y) / (p1.x - o1.x);
            double slope2 = (p2.y - o2.y) / (p2.x - o2.x);
            double ang1 = atan (slope1) * 180 / 3.14;
            double ang2 = atan (slope2) * 180 / 3.14;
            
            double ang = fabs(ang2) - fabs(ang1);
            
            
            if ( fabs(ang) > 80.0  && fabs(ang) < 100.0   &&   abs(cross) > /*EPS*/1e-7){
                double t1 = (x.x * d2.y - x.y * d2.x)/cross;
                r = o1 + d1 * t1;
                intersections_.push_back(r);
                
                
                distance = std::sqrt(std::pow(circleCenter_.x - r.x, 2.0) +
                                     std::pow(circleCenter_.y - r.y, 2.0));
                //if (distance > 6.0*expectedCircleRadius_)continue;
            }
        }
    }
    
    
    // Now select intersections that occur near the start or end point of the intersecting lines. This removes random intersections from sprurious "noise" lines.
    
    for(unsigned int  intsec = 0; intsec < intersections_.size(); intsec++ ){
        bool goodIntersection = false;
        for(unsigned int  gl = 0; gl < goodLines_.size(); gl++ ){
            
            double d_start = std::sqrt(std::pow(intersections_[intsec].x - goodLines_[gl].first.x, 2.0) + std::pow(intersections_[intsec].y - goodLines_[gl].first.y, 2.0));
            double d_end = std::sqrt(std::pow(intersections_[intsec].x - goodLines_[gl].second.x, 2.0) + std::pow(intersections_[intsec].y - goodLines_[gl].second.y, 2.0));
            if(d_start< 5.0 || d_end < 5.0) goodIntersection = true;
        }
        if(goodIntersection == true) goodIntersections_.push_back(intersections_[intsec]);
    }
    
    for (unsigned int goodint =0; goodint < goodIntersections_.size(); goodint++){
        
        
        // Select the two intersections that are adjacent to the circle.
        if (std::sqrt(std::pow(circleCenter_.x - goodIntersections_[goodint].x, 2.0) + std::pow(circleCenter_.y - goodIntersections_[goodint].y, 2.0)) < expectedCircleRadius_*1.6) finalIntersections_.push_back(goodIntersections_[goodint]);
        
    }
    
    
    
    //one typically ends up with multiple intersections at each corner due extra, spurious lines
    cout <<"n final intersection  = = "<< finalIntersections_.size()<< endl;
    
    // I divide the collection of final intersections into those abpve the circle's centre "up"
    // and below "down". I use the average point of these collections to define to define the interesting intersections.
    double running_total_x  =0;
    double running_total_y  =0;
    double av_x_up;
    double av_x_down;
    double av_y_up;
    double av_y_down;
    
    finalIntersectionsDown_.clear();
    finalIntersectionsUp_.clear();
    
    if (finalIntersections_.size() >0){
        for (unsigned int finalint =0; finalint < finalIntersections_.size(); finalint++) {
            if ( y > finalIntersections_[finalint].y){
                finalIntersectionsDown_.push_back(finalIntersections_[finalint]);
            }
            else if ( y < finalIntersections_[finalint].y){
                finalIntersectionsUp_.push_back(finalIntersections_[finalint]);
            }
            
        }
        
        
        for (unsigned int finalintup =0; finalintup < finalIntersectionsUp_.size(); finalintup++) {
            running_total_x = running_total_x + finalIntersectionsUp_[finalintup].x;
            running_total_y = running_total_y + finalIntersectionsUp_[finalintup].y;
        }
        
        av_x_up = running_total_x/finalIntersectionsUp_.size();
        av_y_up = running_total_y/finalIntersectionsUp_.size();
        
        running_total_x  =0;
        running_total_y  =0;
        
        for (unsigned int finalintdown =0; finalintdown < finalIntersectionsDown_.size(); finalintdown++) {
            running_total_x = running_total_x + finalIntersectionsDown_[finalintdown].x;
            running_total_y = running_total_y + finalIntersectionsDown_[finalintdown].y;
        }
        
        av_x_down = running_total_x/finalIntersectionsDown_.size();
        av_y_down = running_total_y/finalIntersectionsDown_.size();
        
        for (unsigned int finalintup =0; finalintup < finalIntersectionsUp_.size(); finalintup++) {
            circle( img_rgb, finalIntersectionsUp_[finalintup], 10, cv::Scalar(0,255,255), 3, 8, 0 );
        }
        
        for (unsigned int finalintdown =0; finalintdown < finalIntersectionsDown_.size(); finalintdown++) {
            circle( img_rgb, finalIntersectionsDown_[finalintdown], 10, cv::Scalar(0,255,0), 3, 8, 0 );
        }
        
        cv::line(img_rgb,
                 cv::Point(cvRound(av_x_down), cvRound(av_y_down)),
                 cv::Point(cvRound(av_x_up), cvRound(av_y_up)),
                 cv::Scalar(0, 255, 255), 4, CV_AA);
        
        
        circle( img_rgb, cv::Point(x,y), 10, cv::Scalar(255,255,0), 3, 8, 0 );
        
        
        slope_final = (av_y_up -  av_y_down) / (av_x_up -  av_x_down);
        ang_final = atan (slope_final) * 180 / 3.14;    
    }
    
    
    std::ostringstream convert;   // stream used for the conversion
    convert.str(" ");
    convert.clear();
    convert << ang_final;
    std::string str_ang_final = convert.str();
    convert.str(" ");
    convert.clear();
    convert << circleCenter_.x ;
    std::string str_x = convert.str();
    convert.str(" ");
    convert.clear();
    convert << circleCenter_.y;
    std::string str_y = convert.str();
    
    std::string text_result = "Marker is at = (" +  str_x  +  ", " + str_y + "), with orientation = " + str_ang_final  + " degrees";
    
    
    putText(img_rgb, text_result, cvPoint(30,100),
            cv::FONT_HERSHEY_COMPLEX_SMALL, 2.8, cvScalar(200,200,250), 1, CV_AA);
    std::string filename = cacheDirectory1_ + "/PatRec_result.png";
    cv::imwrite(filename, img_rgb);
    
    QString filename_qs = QString::fromStdString(filename);
    
    
    emit updateImage(1, filename_qs);
    emit foundSensor(1);
    //emit sendPosition(0, circleCenter_.x,circleCenter_.y, ang_final);
    NQLog("AssemblyVUEyeCamera") << "  found marker";
    

}

void AssemblySensorMarkerFinder::setNewGeneralThreshold(int value, cv::Mat img)
{
  generalThreshold = value;
  emit sendCurrentGeneralThreshold(value);
  this -> updateThresholdImage(img);
  NQLog("AssemblySensorMarkerFinder") << " Threshold value successfuly changed to value = "<< value;
}

void AssemblySensorMarkerFinder::getCurrentGeneralThreshold()
{ 
    NQLog("AssemblySensorMarkerFinder") << " : INFO! : update signal received and threshold sent.";
    emit sendCurrentGeneralThreshold(generalThreshold); }

void AssemblySensorMarkerFinder::updateThresholdImage(cv::Mat img)
{


    Mat img_copy = img.clone();
    
    //Greyscale images
    Mat img_copy_gs(img_copy.size(), img_copy.type());    

    if (img.channels()> 1){
    //convert color to GS
    cvtColor( img_copy,   img_copy_gs,   CV_BGR2GRAY );
    }else{
     img_copy_gs = img_copy.clone();
    }

    //Binary images
    Mat img_copy_bin(img_copy_gs.size(), img_copy_gs.type());
    
    //Apply thresholding
    NQLog("AssemblySensorMarkerFinder") << "::updateThresholdImage() : applying threshold for ThresholdTuner.";
    cv::threshold(img_copy_gs, img_copy_bin, generalThreshold, 255, cv::THRESH_BINARY);
     
    std::string filename_img_bin = Config::CMSTkModLabBasePath + "/share/assembly/Sensor_bin.png";
    
    cv::imwrite(filename_img_bin, img_copy_bin);
    
    QString filename_img_bin_qs = QString::fromStdString(filename_img_bin);
    
    QString filename_master = QString::fromStdString(filename_img_bin);
    
    emit sendUpdatedThresholdImage(filename_master);
    NQLog("AssemblySensorMarkerFinder") << "::updateThresholdImage() : signal emitted!!!";
}
