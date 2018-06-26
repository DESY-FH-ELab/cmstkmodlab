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

#ifndef ASSEMBLYUTILITIES_H
#define ASSEMBLYUTILITIES_H

#include <nqlogger.h>

#include <string>
#include <sstream>

#include <QString>
#include <QLineEdit>

#include <opencv2/opencv.hpp>

namespace assembly {

  // Qt helpers
  QString QtCacheDirectory();

  bool QDir_mkpath(const QString&);
  bool QDir_mkpath(const std::string&);

  bool DirectoryExists(const QString&);
  bool DirectoryExists(const std::string&);

  bool IsFile(const QString&);
  bool IsFile(const std::string&);

  // openCV helpers
  cv::Mat cv_imread(const std::string&, const int);
  cv::Mat cv_imread(const QString&    , const int);

  void cv_imwrite(const std::string&, const cv::Mat&);
  void cv_imwrite(const QString&    , const cv::Mat&);

  bool MatIsBlackAndWhite(const cv::Mat&);

  template <class T> void QLineEdit_setText(QLineEdit* const, const T&);

  // geometry helpers
  void rotation2D_deg(double&, double&, const double, const double, const double);

}

template <class T>
void assembly::QLineEdit_setText(QLineEdit* const ptr_le, const T& val)
{
  if(ptr_le == nullptr)
  {
    NQLog("AssemblyUtilities", NQLog::Critical) << "QLineEdit_setText(" << ptr_le << ", " << val << ")"
       << ": null pointer to QLineEdit object, no action taken";

    return;
  }

  std::stringstream strs;
  strs << val;

  ptr_le->setText(QString::fromStdString(strs.str()));

  return;
}

#endif // ASSEMBLYUTILITIES_H
