#include <iostream>
#include <cmath>

#include <QApplication>

#include <nqlogger.h>

#include "AssemblySensorMarkerFinder.h"

AssemblySensorMarkerFinder::AssemblySensorMarkerFinder(QObject *parent)
    : AssemblyVMarkerFinder(parent),
      gaussianBlurKernelSize_(9),
      gaussianBlurSigma_(2),
      expectedCircleRadius_(82.0)
{

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

    size_t ret = findCircle();
    if (ret==0) {
        emit markerFound(imageRGB_);
        return;
    }
    drawCircle();

    ret = findLines();
    drawLines();

    ret = findIntersections();

    emit markerFound(imageRGB_);
}

size_t AssemblySensorMarkerFinder::findCircle()
{
    circleQuality_ = 1.0;

    std::vector<cv::Vec3f> circles;

    QMutexLocker lock(&mutex_);

    /// Apply the Hough Transform to find the circles
    cv::HoughCircles(image_, circles, CV_HOUGH_GRADIENT,
                     1, 20,
                     80, 20,
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

        circleQuality_ = 1.0;
        circleQuality_ *= 1.0 - std::fabs(circleRadius_-expectedCircleRadius_)/expectedCircleRadius_;

        return 1;
    }

    std::map<float,cv::Vec3f> radiusMap;
    for(size_t i = 0; i < circles.size(); i++ ) {
        radiusMap[std::fabs(circles[i][2]-expectedCircleRadius_)] = circles[i];
    }

    std::map<float,cv::Vec3f>::iterator it = radiusMap.begin();

    circleCenter_.x = it->second[0];
    circleCenter_.y = it->second[1];
    circleRadius_ = it->second[2];

    circleQuality_ = 1.0;
    circleQuality_ *= 1.0-std::fabs(circleRadius_-expectedCircleRadius_)/expectedCircleRadius_;

    ++it;
    float minDistance = 1.e9;
    for (;it!=radiusMap.end();++it) {
        float distance = cv::norm(circleCenter_ - cv::Point2f(it->second[0], it->second[1]));
        if (distance < minDistance) minDistance = distance;
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
    cv::Mat imageEdges;
    cv::Canny(image_, imageEdges, 50, 200, 3);

    std::vector<cv::Vec4i> tempLines;

    lines_.clear();
    goodLines_.clear();

    cv::HoughLinesP(imageEdges, tempLines, 1, 0.1*CV_PI/180, 75, 50, 25);

    for (size_t i = 0; i < tempLines.size(); i++ ) {
        lines_.push_back(std::pair<cv::Point2f,cv::Point2f>(cv::Point2f(tempLines[i][0],tempLines[i][1]),
                                                            cv::Point2f(tempLines[i][2],tempLines[i][3])));
    }

    float distance;
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
                                              cv::Point2f &r, float distance)
{
  cv::Point2f x = o2 - o1;
  cv::Point2f d1 = p1 - o1;
  cv::Point2f d2 = p2 - o2;

  float cross = d1.x*d2.y - d1.y*d2.x;
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

size_t AssemblySensorMarkerFinder::findIntersections()
{
    std::vector<cv::Point2f> theIntersections;
    std::vector<cv::Point2f> theGoodIntersections;
    std::vector<cv::Point2f> tempIntersections;

    for (size_t i = 0; i < goodLines_.size()-1; i++) {
        for (size_t j = i+1; j < goodLines_.size(); j++) {

            cv::Point2f theIntersection;

            if (intersection(goodLines_[i].first, goodLines_[i].second,
                             goodLines_[j].first, goodLines_[j].second,
                             theIntersection, expectedCircleRadius_)) {
                theGoodIntersections.push_back(theIntersection);
                cv::Point center(cvRound(theIntersection.x),
                                 cvRound(theIntersection.y));
                cv::circle(imageRGB_, center, 4, cv::Scalar(0, 255, 0), -1, CV_AA, 0 );
            }
        }
    }

    tempIntersections.clear();
    std::vector<bool> intersectionFlag(theGoodIntersections.size());
    for (size_t i = 0; i < theGoodIntersections.size(); i++ ) intersectionFlag[i] = false;
    for (size_t i = 0; i < theGoodIntersections.size(); i++ ) {

        if (intersectionFlag[i]) continue;

        float sx = 0, sy = 0;
        int n = 0;

        sx += theGoodIntersections[i].x;
        sy += theGoodIntersections[i].y;
        intersectionFlag[i] = true;
        n++;

        if (i<theGoodIntersections.size()-1) {
            for (size_t j = i+1; j < theGoodIntersections.size(); j++ ) {
                if (intersectionFlag[j]) continue;

                if (cv::norm(theGoodIntersections[i] - theGoodIntersections[j]) < 1) {
                    sx += theGoodIntersections[j].x;
                    sy += theGoodIntersections[j].y;
                    intersectionFlag[j] = true;
                    n++;
                }
            }
        }
        tempIntersections.push_back(cv::Point2f(sx/n, sy/n));
    }
    theGoodIntersections = tempIntersections;

    for (size_t i = 0; i < goodLines_.size(); i++ ) {
        for (size_t j = 0; j < lines_.size(); j++ ) {

            if (goodLines_[i]==lines_[j]) continue;

            cv::Point2f theIntersection;

            if (intersection(goodLines_[i].first, goodLines_[i].second,
                             lines_[j].first, lines_[j].second,
                             theIntersection, expectedCircleRadius_)) {
                theIntersections.push_back(theIntersection);
            }
        }
    }

    tempIntersections.clear();
    intersectionFlag.resize(theIntersections.size());
    for (size_t i = 0; i < theIntersections.size(); i++ ) intersectionFlag[i] = false;
    for (size_t i = 0; i < theIntersections.size(); i++ ) {

        if (intersectionFlag[i]) continue;

        float sx = 0, sy = 0;
        int n = 0;

        sx += theIntersections[i].x;
        sy += theIntersections[i].y;
        intersectionFlag[i] = true;
        n++;

        if (i<theIntersections.size()-1) {
            for (size_t j = i+1; j < theIntersections.size(); j++ ) {
                if (intersectionFlag[j]) continue;

                if (cv::norm(theIntersections[i] - theIntersections[j]) < 1) {
                    sx += theIntersections[j].x;
                    sy += theIntersections[j].y;
                    intersectionFlag[j] = true;
                    n++;
                }
            }
        }
        tempIntersections.push_back(cv::Point2f(sx/n, sy/n));
    }
    theIntersections = tempIntersections;

    for(size_t i = 0; i < theIntersections.size(); i++ ) {
        cv::Point center(cvRound(theIntersections[i].x), cvRound(theIntersections[i].y));
      circle(imageRGB_, center, 4, cv::Scalar(0, 0, 255), -1, CV_AA, 0 );
    }
}
