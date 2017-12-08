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

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#include <opencv2/opencv.hpp>

#include <QCoreApplication>
#include <QProcess>
#include <QFile>
#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include <ApplicationConfig.h>
#include <MarkerFinderManual.h>

class ImageInfo
{
public:
    cv::Mat image_;
    float circleX_;
    float circleY_;
    float circleRadius_;
};

void loadImage(const std::string& filename,
               float x, float y, float r,
               int gaussianBlurKernelSize,
               int gaussianBlurSigma,
               std::vector<ImageInfo>& images)
{
    ImageInfo ii;

    ii.image_ = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);

    if (gaussianBlurKernelSize!=0) {
        cv::GaussianBlur(ii.image_, ii.image_,
                         cv::Size(gaussianBlurKernelSize, gaussianBlurKernelSize),
                         gaussianBlurSigma, gaussianBlurSigma);
    }

    ii.circleX_ = x;
    ii.circleY_ = y;
    ii.circleRadius_ = r;

    images.push_back(ii);
}

int main( int argc, char** argv )
{
    if (argc!=2) {
        return 0;
    }

    ApplicationConfig *config = ApplicationConfig::instance(argv[1]);

    int GaussianBlurKernelSize = config->getValue<int>("GaussianBlurKernelSize");
    int GaussianBlurSigma = config->getValue<int>("GaussianBlurSigma");

    std::vector<ImageInfo> images;

    std::vector<std::string> fns = config->getValueVector<std::string>("image");
    std::vector<float> x = config->getValueVector<float>("circle_x");
    std::vector<float> y = config->getValueVector<float>("circle_y");
    std::vector<float> r = config->getValueVector<float>("circle_r");

    if (fns.size()!=x.size() || x.size()!=y.size() || y.size()!=r.size()) {
        return 0;
    }

    std::vector<float>::iterator itx = x.begin();
    std::vector<float>::iterator ity = y.begin();
    std::vector<float>::iterator itr = r.begin();
    for (std::vector<std::string>::iterator it = fns.begin();
         it != fns.end();
         ++it, ++itx, ++ity, ++itr) {
        loadImage(*it,
                  *itx, *ity, *itr,
                  GaussianBlurKernelSize, GaussianBlurSigma, images);
    }

    MarkerFinderManual finder;

    finder.setGaussianBlurKernelSize(GaussianBlurKernelSize);
    finder.setGaussianBlurSigma(GaussianBlurSigma);

    finder.setCircleEdgeDetectionThreshold(60);
    finder.setCircleCenterDetectionThreshold(40);
    finder.setExpectedCircleSize(80);

    std::vector<AssemblyMarkerCircle> circles;

    std::ofstream ofile("dump.txt");

    for (int CenterThreshold = 15;CenterThreshold<=200;CenterThreshold+=5) {
        for (int EdgeThreshold = 15;EdgeThreshold<=400;EdgeThreshold+=5) {

            finder.setCircleEdgeDetectionThreshold(EdgeThreshold);
            finder.setCircleCenterDetectionThreshold(CenterThreshold);

            float count = 0.0;
            float q = 1.0;
            float xyoffsetsum = 0.0;
            float drsum = 0.0;

            for (unsigned int i=0;i<images.size();++i) {
                ImageInfo &ii = images[i];

                finder.setExpectedCircleSize(ii.circleRadius_);

                int newcount = finder.findCircle(ii.image_);

                if (newcount>50) {
                    q = 0.0;
                    break;
                }
                if (newcount==0) {
                    q = 0.0;
                    break;
                }

                count += newcount;
                circles = finder.getCircles();

                AssemblyMarkerCircle& circle = circles.front();

                /*
                if (count==1) {
                    std::cout << circle.x() << std::endl;
                    std::cout << circle.y() << std::endl;
                }
                */

                float dx = circle.x() - ii.circleX_;
                float dy = circle.y() - ii.circleY_;
                float dr = circle.r() - ii.circleRadius_;

                //q *= 1.0 - std::fabs(dr)/ii.circleRadius_;

                float xyoffset = std::sqrt(dx*dx + dy*dy);

                if (xyoffset>2.0 && xyoffset<=25.) {
                    q *= 1.0 - xyoffset/25.;
                } else if (xyoffset>25.) {
                    q *= 0.0;
                }

                xyoffsetsum += xyoffset*xyoffset;
                drsum += dr*dr;
            }

            if (q!=0) {
                ofile << EdgeThreshold << "\t" << CenterThreshold << "\t"
                      << count/images.size() << "\t"
                      << std::sqrt(xyoffsetsum) << "\t"
                      << std::sqrt(drsum) << "\t"
                         //<< dr << "\t"
                      << q
                      << std::endl;
            }
            std::cout << EdgeThreshold << "\t" << CenterThreshold << "\t"
                      << count/images.size() << "\t"
                      << std::sqrt(xyoffsetsum) << "\t"
                      << std::sqrt(drsum) << "\t"
                         //<< dr << "\t"
                      << q
                      << std::endl;
        }
    }
}
