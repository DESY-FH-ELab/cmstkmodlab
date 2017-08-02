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

#include <opencv2/highgui/highgui.hpp>

#include <QColor>

#include <nqlogger.h>
#include <nmatrix.h>

#include "DefoImageAverager.h"

DefoImageAverager::DefoImageAverager(const QStringList& filenames)
:filenames_(filenames)
{
  for (auto filename : filenames_) {
    NQLogMessage("DefoImageAverager") << filename;
  }
}

DefoImageAverager::~DefoImageAverager()
{

}

QImage DefoImageAverager::getAveragedImage()
{
  int width, height;

  QImage ret(filenames_.front());
  width = ret.width();
  height = ret.height();

  /*
  NMatrix<unsigned short> red(width, height);
  NMatrix<unsigned short> green(width, height);
  NMatrix<unsigned short> blue(width, height);

  int count = 0;
  for (QString filename : filenames_) {
    IplImage *img = cvLoadImage(filename.toStdString().c_str());
    IplImage *imgRed = cvCreateImage(cvGetSize(img), 8, 1);
    IplImage *imgGreen = cvCreateImage(cvGetSize(img), 8, 1);
    IplImage *imgBlue = cvCreateImage(cvGetSize(img), 8, 1);
    cvSplit(img, imgRed, imgGreen, imgBlue, NULL);
    cvReleaseImage(&img);

    for (int w=0;w<width;w++) {
      for (int h=0;h<height;h++) {

      }
    }
  }
  */

  NMatrix<unsigned short> red(width, height);
  NMatrix<unsigned short> green(width, height);
  NMatrix<unsigned short> blue(width, height);

  int count = 0;
  for (QString filename : filenames_) {

    QImage temp(filename);

    for (int w=0;w<width;w++) {
      for (int h=0;h<height;h++) {
        QRgb rgb = temp.pixel(w, h);

        int r = qRed(rgb);
        int g = qGreen(rgb);
        int b = qBlue(rgb);

        red(w, h) += r;
        green(w, h) += g;
        blue(w, h) += b;
      }
    }

    count++;
  }

  for (int w=0;w<width;w++) {
    for (int h=0;h<height;h++) {
      ret.setPixel(w, h, qRgb(red(w, h)/count,
                              green(w, h)/count,
                              blue(w, h)/count));
    }
  }

  return ret;
}
