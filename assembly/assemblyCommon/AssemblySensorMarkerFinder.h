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

    int gaussianBlurKernelSize() const { return gaussianBlurKernelSize_; }
    int gaussianBlurSigma() const { return gaussianBlurSigma_; }

    double expectedCircleRadius() const { return expectedCircleRadius_; }
    int circleEdgeDetectionThreshold() const { return circleEdgeDetectionThreshold_; }
    int circleCenterDetectionThreshold() const { return circleCenterDetectionThreshold_; }

    int linesCannyEdgeDetectionThreshold1() const { return linesCannyEdgeDetectionThreshold1_; }
    int linesCannyEdgeDetectionThreshold2() const { return linesCannyEdgeDetectionThreshold2_; }
    int linesCannyEdgeDetectionApertureSize() const { return linesCannyEdgeDetectionApertureSize_; }
    bool linesCannyEdgeDetectionL2Gradient() const { return linesCannyEdgeDetectionL2Gradient_; }

    size_t findCircle(const cv::Mat&);
    const std::vector<AssemblyMarkerCircle>& getCircles() { return circles_; }

public slots:

    void setGaussianBlurKernelSize(int value) { gaussianBlurKernelSize_ = value; }
    void setGaussianBlurSigma(int value) { gaussianBlurSigma_ = value;}

    void setExpectedCircleSize(double value) { expectedCircleRadius_ = value; }
    void setCircleEdgeDetectionThreshold(int value) { circleEdgeDetectionThreshold_ = value; }
    void setCircleCenterDetectionThreshold(int value) { circleCenterDetectionThreshold_ = value; }

    void setLinesCannyEdgeDetectionThreshold1(int value) { linesCannyEdgeDetectionThreshold1_ = value; }
    void setLinesCannyEdgeDetectionThreshold2(int value) { linesCannyEdgeDetectionThreshold2_ = value; }
    void setLinesCannyEdgeDetectionApertureSize(int value) { linesCannyEdgeDetectionApertureSize_ = value; }
    void setLinesCannyEdgeDetectionL2Gradient(bool value) { linesCannyEdgeDetectionL2Gradient_ = value; }

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
    cv::Mat imageEdges_;
    cv::Mat imageRGB_;

    int gaussianBlurKernelSize_;
    int gaussianBlurSigma_;

    int circleEdgeDetectionThreshold_;
    int circleCenterDetectionThreshold_;
    double expectedCircleRadius_;

    int linesCannyEdgeDetectionThreshold1_;
    int linesCannyEdgeDetectionThreshold2_;
    int linesCannyEdgeDetectionApertureSize_;
    bool linesCannyEdgeDetectionL2Gradient_;

    cv::Point2f circleCenter_;
    double circleRadius_;
    double circleQuality_;

    double orientation_;

    std::vector<AssemblyMarkerCircle> circles_;

    std::vector<std::pair<cv::Point2f,cv::Point2f> > lines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > goodLines_;
    std::vector<cv::Point2f> intersections_;
    std::vector<cv::Point2f> goodIntersections_;

signals:

    void edgesDetected(const cv::Mat&);
};

#endif // ASSEMBLYSENSORMARKERFINDER_H
