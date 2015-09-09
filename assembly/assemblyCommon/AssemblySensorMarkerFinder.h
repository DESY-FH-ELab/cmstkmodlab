#ifndef ASSEMBLYSENSORMARKERFINDER_H
#define ASSEMBLYSENSORMARKERFINDER_H

#include <vector>
#include <map>

#include "AssemblyVMarkerFinder.h"

class AssemblySensorMarkerFinder : public AssemblyVMarkerFinder
{
    Q_OBJECT
public:
    explicit AssemblySensorMarkerFinder(QObject *parent = 0);
    ~AssemblySensorMarkerFinder();

    void setGaussianBlurKernelSize(int value) { gaussianBlurKernelSize_ = value; }
    void setGaussianBlurSigma(int value) { gaussianBlurSigma_ = value;}

    void setCircleEdgeDetectionThreshold(int value) { circleEdgeDetectionThreshold_ = value; }
    void setCircleCenterDetectionThreshold(int value) { circleCenterDetectionThreshold_ = value; }
    double expectedCircleRadius() const { return expectedCircleRadius_; }

    size_t findCircle(const cv::Mat&);
    const std::vector<AssemblyMarkerCircle>& getCircles() { return circles_; }

public slots:

    void setExpectedCircleSize(double value) { expectedCircleRadius_ = value; }
    virtual void findMarker(const cv::Mat&);

protected slots:

protected:

    void drawCircle();

    size_t findLines();
    void drawLines();

    size_t findIntersections();
    bool intersection(cv::Point2f o1, cv::Point2f p1,
                      cv::Point2f o2, cv::Point2f p2,
                      cv::Point2f &r, double distance);
    void mergeIntersections(std::vector<cv::Point2f>& intersections);
    void drawIntersections();

    void determineOrientation();
    void drawOrientation();

    cv::Mat image_;
    cv::Mat imageRGB_;

    int gaussianBlurKernelSize_;
    int gaussianBlurSigma_;

    int circleEdgeDetectionThreshold_;
    int circleCenterDetectionThreshold_;
    double expectedCircleRadius_;

    cv::Point2f circleCenter_;
    double circleRadius_;
    double circleQuality_;

    double orientation_;

    std::vector<AssemblyMarkerCircle> circles_;

    std::vector<std::pair<cv::Point2f,cv::Point2f> > lines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > goodLines_;
    std::vector<cv::Point2f> intersections_;
    std::vector<cv::Point2f> goodIntersections_;
};

#endif // ASSEMBLYSENSORMARKERFINDER_H
