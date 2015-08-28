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

public slots:

    virtual void findMarker(const cv::Mat&);

protected slots:

protected:

    size_t findCircle();
    void drawCircle();

    size_t findLines();
    void drawLines();

    size_t findIntersections();
    bool intersection(cv::Point2f o1, cv::Point2f p1,
                      cv::Point2f o2, cv::Point2f p2,
                      cv::Point2f &r, float distance);
    void mergeIntersections(std::vector<cv::Point2f>& intersections);
    void drawIntersections();

    void determineOrientation();
    void drawOrientation();

    cv::Mat image_;
    cv::Mat imageRGB_;

    int gaussianBlurKernelSize_;
    int gaussianBlurSigma_;

    float expectedCircleRadius_;
    cv::Point2f circleCenter_;
    float circleRadius_;
    float circleQuality_;

    float orientation_;

    std::vector<std::pair<cv::Point2f,cv::Point2f> > lines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > goodLines_;
    std::vector<cv::Point2f> intersections_;
    std::vector<cv::Point2f> goodIntersections_;
};

#endif // ASSEMBLYSENSORMARKERFINDER_H
