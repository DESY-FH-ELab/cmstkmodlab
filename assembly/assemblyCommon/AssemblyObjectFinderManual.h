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

#ifndef ASSEMBLYOBJECTFINDERMANUAL_H
#define ASSEMBLYOBJECTFINDERMANUAL_H

#include <nqlogger.h>

#include <vector>

#include <QTimer>
#include <QDateTime>
#include <QString>
#include <QMutex>

#include <opencv2/opencv.hpp>

class AssemblyMarkerCircle
{
public:

    AssemblyMarkerCircle(float x, float y, float r, float q);

    float x() const { return x_; }
    float y() const { return y_; }
    float r() const { return r_; }
    float q() const { return q_; }

protected:

    float x_, y_, r_, q_;
};

class AssemblyObjectFinderManual : public QObject
{
    Q_OBJECT
protected:
    
    int generalThreshold_;

    double matchLoc_x_lab;
    double matchLoc_y_lab;
    int labmode_g, objectmode_g;

public:
    explicit AssemblyObjectFinderManual(QObject *parent = 0);
    ~AssemblyObjectFinderManual();

    int gaussianBlurKernelSize() const { return gaussianBlurKernelSize_; }
    int gaussianBlurSigma() const { return gaussianBlurSigma_; }

    double expectedCircleRadius() const { return expectedCircleRadius_; }
    int circleEdgeDetectionThreshold() const { return circleEdgeDetectionThreshold_; }
    int circleCenterDetectionThreshold() const { return circleCenterDetectionThreshold_; }

    int linesCannyEdgeDetectionThreshold1() const { return linesCannyEdgeDetectionThreshold1_; }
    int linesCannyEdgeDetectionThreshold2() const { return linesCannyEdgeDetectionThreshold2_; }
    int linesCannyEdgeDetectionApertureSize() const { return linesCannyEdgeDetectionApertureSize_; }
    bool linesCannyEdgeDetectionL2Gradient() const { return linesCannyEdgeDetectionL2Gradient_; }

    double linesHoughDistanceResolution() const { return linesHoughDistanceResolution_; }
    double linesHoughAngleResolution() const { return linesHoughAngleResolution_; }
    int linesHoughThreshold() const { return linesHoughThreshold_; }
    double linesHoughMinLineLength() const { return linesHoughMinLineLength_; }
    double linesHoughMaxLineGap() const { return linesHoughMaxLineGap_; }

    int getGeneralThresholdValue() {return generalThreshold_; }
    void setGeneralThresholdValue(int newThresholdValue) { generalThreshold_ = newThresholdValue; }

    cv::Mat img, img_clip_A, img_clip_B, result_1, result_2, dst;


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

    void setLinesHoughDistanceResolution(double value) { linesHoughDistanceResolution_ = value; }
    void setLinesHoughAngleResolution(double value) { linesHoughAngleResolution_ = value; }
    void setLinesHoughThreshold(int value) { linesHoughThreshold_ = value; }
    void setLinesHoughMinLineLength(double value) { linesHoughMinLineLength_ = value; }
    void setLinesHoughMaxLineGap(double value) { linesHoughMaxLineGap_ = value; }

    virtual void findMarker(const cv::Mat&);
    virtual void findMarker_circleSeed(int);

    //ThresholdTunerSlots
    void setNewGeneralThreshold(int, cv::Mat);
    void getCurrentGeneralThreshold();
    void updateThresholdImage(cv::Mat);

protected slots:
    void runObjectDetection(int labmode, int objectmode);
    void runObjectDetection_labmode(cv::Mat);

protected:

    QMutex mutex_;

    std::string cacheDirectory1_;
    std::string cacheDirectory2_;

    
    int nAcquiredImages;
    int nTotalImages;
 
    size_t findLines();

    size_t findIntersections();
    bool intersection(cv::Point2f o1, cv::Point2f p1,
                      cv::Point2f o2, cv::Point2f p2,
                      cv::Point2f &r, double distance);
    void mergeIntersections(std::vector<cv::Point2f>& intersections);

    void determineOrientation();

    void drawCircle();
    void drawLines();
    void drawIntersections();
    void drawOutline();
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

    double linesHoughDistanceResolution_;
    double linesHoughAngleResolution_;
    int linesHoughThreshold_;
    double linesHoughMinLineLength_;
    double linesHoughMaxLineGap_;

    cv::Point2f circleCenter_;
    double circleRadius_;
    double circleQuality_;

    double orientation_;
    
    const cv::Mat * global_image_ptr;
    
    std::vector<AssemblyMarkerCircle> circles_;

    std::vector<std::pair<cv::Point2f,cv::Point2f> > lines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > goodLines_;
    std::vector<cv::Point2f> intersections_;
    std::vector<cv::Point2f> goodIntersections_;


signals:
    
    void markerFound(const cv::Mat&);

    void locatePickupCorner_templateMatching(cv::Mat, cv::Mat);

    void reportObjectLocation(int, double,double, double);
    void edgesDetected(const cv::Mat&);
    void updateImage(int, QString);
    void foundSensor(int);
    void getImage();
    void getImageBlur(cv::Mat, cv::Rect);
    void acquireImage();
    void sendCurrentGeneralThreshold(int);
    void sendUpdatedThresholdImage(QString);
};

#endif // ASSEMBLYOBJECTFINDERMANUAL_H
