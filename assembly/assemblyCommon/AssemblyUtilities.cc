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

#include <nqlogger.h>
#include <AssemblyUtilities.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif
#include <QDir>
#include <QFileInfo>
#include <QImageReader>
#include <QMessageBox>

void assembly::kill_application(const QString& log1, const QString& log2)
{
  QMessageBox::critical(0, log1, log2, QMessageBox::Abort);

  throw; // must abort
}

QString assembly::QtCacheDirectory()
{
 #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  return QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
 #else
  return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
 #endif
}

bool assembly::QDir_mkpath(const QString& path)
{
  const QDir dir(path);
  if(dir.exists() == false){ return dir.mkpath("."); }

  return true;
}

bool assembly::QDir_mkpath(const std::string& path)
{
  return assembly::QDir_mkpath(QString(path.c_str()));
}

bool assembly::DirectoryExists(const QString& path)
{
  const QDir dir(path);

  return dir.exists();
}

bool assembly::DirectoryExists(const std::string& path)
{
  return assembly::DirectoryExists(QString(path.c_str()));
}

bool assembly::IsFile(const QString& path)
{
  const QFileInfo qfileinfo(path);

  return (qfileinfo.exists() && qfileinfo.isFile());
}

bool assembly::IsFile(const std::string& path)
{
  return assembly::IsFile(QString(path.c_str()));
}

cv::Mat assembly::cv_imread(const std::string& path_str, const int imread_flag)
{
  return assembly::cv_imread(QString::fromStdString(path_str), imread_flag);
}

cv::Mat assembly::cv_imread(const QString& path_qstr, const int imread_flag)
{
  const std::string path_str = path_qstr.toUtf8().constData();

  const cv::Mat img = cv::imread(path_str, imread_flag);

  const QImageReader img_reader(path_qstr);

  cv::Mat img_1;

  if(img_reader.format() == "png" && (img.channels() > 1))
  {
    cv::cvtColor(img, img_1, cv::COLOR_RGB2BGR);
  }
  else
  {
    img_1 = img.clone();
  }

  return img_1;
}

void assembly::cv_imwrite(const std::string& path_str, const cv::Mat& img)
{
  assembly::cv_imwrite(QString::fromStdString(path_str), img);
}

void assembly::cv_imwrite(const QString& path_qstr, const cv::Mat& img)
{
  if(path_qstr.endsWith(".png", Qt::CaseInsensitive) == false)
  {
    NQLog("AssemblyUtilities", NQLog::Warning) << "cv_imwrite(" << path_qstr << ", cv::Mat)"
       << ": target output path does not end with \".png\", file not saved to disk";

    return;
  }
  else if(img.channels() > 1)
  {
    cv::Mat img_1;
    cv::cvtColor(img, img_1, cv::COLOR_BGR2RGB);
    cv::imwrite(path_qstr.toUtf8().constData(), img_1);
  }
  else
  {
    cv::imwrite(path_qstr.toUtf8().constData(), img);
  }

  return;
}

bool assembly::MatIsBlackAndWhite(const cv::Mat& img)
{
  cv::Mat bgr[3];
  cv::split(img, bgr);

  cv::Mat dst;

  cv::absdiff(bgr[0], bgr[1], dst);
  if(cv::countNonZero(dst)){ return false; }

  cv::absdiff(bgr[0], bgr[2], dst);
  if(cv::countNonZero(dst)){ return false; }

  return true;
}

void assembly::rotation2D_deg(double& X_1, double& Y_1, const double angle_deg, const double X_0, const double Y_0)
{
  const double angle_rad = (angle_deg * (M_PI/180.0));

  const double COS = std::cos(angle_rad);
  const double SIN = std::sin(angle_rad);

  X_1 = (COS * X_0) - (SIN * Y_0);
  Y_1 = (SIN * X_0) + (COS * Y_0);

  return;
}
