#include <iostream>
#include <cmath>

#include <QApplication>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "AssemblySensorMarkerFinder.h"

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
}

AssemblySensorMarkerFinder::~AssemblySensorMarkerFinder()
{
    NQLog("AssemblySensorMarkerFinder") << "delete";
}

void AssemblySensorMarkerFinder::findMarker(const cv::Mat& image)
{
    QMutexLocker lock(&mutex_);

    image.copyTo(image_);

    lock.unlock();

    if (image_.channels()==1) {
        cvtColor(image_, imageRGB_, CV_GRAY2RGB);
    } else {
        image_.copyTo(imageRGB_);
    }

    cv::GaussianBlur(image_, image_,
                     cv::Size(gaussianBlurKernelSize_, gaussianBlurKernelSize_),
                     gaussianBlurSigma_, gaussianBlurSigma_);

    size_t ret = findCircle(image_);
    if (ret==0) {
        emit markerFound(imageRGB_);
        return;
    }
    drawCircle();

    /*
    ret = findLines();
    drawLines();

    ret = findIntersections();
    drawIntersections();

    determineOrientation();
    drawOrientation();
    */

    emit markerFound(imageRGB_);
}

size_t AssemblySensorMarkerFinder::findCircle(const cv::Mat& image)
{
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

    if (circles.size()==0) {
        circleQuality_ = 0.0;
        return 0;
    }

    if (circles.size()==1) {
        circleCenter_.x = circles[0][0];
        circleCenter_.y = circles[0][1];
        circleRadius_ = circles[0][2];

        circleQuality_ = 1.0 - std::fabs(circleRadius_-expectedCircleRadius_)/expectedCircleRadius_;

        circles_.push_back(AssemblyMarkerCircle(circleCenter_.x,
                                                circleCenter_.y,
                                                circleRadius_,
                                                circleQuality_));

        return 1;
    }

    std::map<double,cv::Vec3f> radiusMap;
    for(size_t i = 0; i < circles.size(); i++ ) {
        radiusMap[std::fabs(circles[i][2]-expectedCircleRadius_)] = circles[i];
    }

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

    return circles.size();
}

void AssemblySensorMarkerFinder::drawCircle()
{
    cv::Point2f center(cvRound(circleCenter_.x),
                       cvRound(circleCenter_.y));
    // circle center
    cv::circle(imageRGB_, center, 4, cv::Scalar(0, 255, 0), -1, CV_AA, 0);
    // circle outline
    cv::circle(imageRGB_, center, circleRadius_, cv::Scalar(0, 0, 255), 4, CV_AA, 0);
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

    cv::HoughLinesP(imageEdges, tempLines, 1, 0.1*CV_PI/180, 75, 50, 25);

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
      circle(imageRGB_, center, 6, cv::Scalar(0, 0, 255), -1, CV_AA, 0 );
    }

    for(size_t i = 0; i < goodIntersections_.size(); i++ ) {
        cv::Point center(cvRound(goodIntersections_[i].x), cvRound(goodIntersections_[i].y));
      circle(imageRGB_, center, 6, cv::Scalar(0, 255, 0), -1, CV_AA, 0 );
    }
}

void AssemblySensorMarkerFinder::determineOrientation()
{
    if (goodIntersections_.size()!=2 && intersections_.size()!=4) return;

    const cv::Point2f& cp = circleCenter_;
    const cv::Point2f& gp0 = goodIntersections_[0];
    const cv::Point2f& gp1 = goodIntersections_[1];

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
    double angle, dot, dv;

    for(size_t i = 0; i < intersections_.size(); i++ ) {
        cv::Point2f v = intersections_[i] - igp;
        dot = v.dot(vg);
        dv = cv::norm(v);
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
        dot = v.dot(vg);
        dv = cv::norm(v);
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
