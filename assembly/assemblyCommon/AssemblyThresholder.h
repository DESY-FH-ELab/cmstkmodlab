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

#ifndef ASSEMBLYTHRESHOLDER_H
#define ASSEMBLYTHRESHOLDER_H

/*  Description:
 *   Controller to convert image to binary (BW) image
 */

#include <QObject>
#include <QString>
#include <QMutex>

#include <opencv2/opencv.hpp>

class AssemblyThresholder : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblyThresholder(QObject* parent=nullptr);
  virtual ~AssemblyThresholder();

  void delete_image_raw();
  void delete_image_binary();

  const cv::Mat& image_raw()    const { return img_raw_; }
  const cv::Mat& image_binary() const { return img_bin_; }

 protected:

  cv::Mat img_raw_;
  cv::Mat img_bin_;

  bool updated_img_raw_;
  bool updated_img_bin_;

 private:
  Q_DISABLE_COPY(AssemblyThresholder)

  mutable QMutex mutex_;

 public slots:

  void update_image_raw(const cv::Mat&);

  void update_image_binary_threshold(const int);
  cv::Mat get_image_binary_threshold(const cv::Mat&, const int) const;

  void update_image_binary_adaptiveThreshold(const int);
  cv::Mat get_image_binary_adaptiveThreshold(const cv::Mat&, const int) const;

  void send_image_raw();
  void send_image_binary();

 signals:

  void updated_image_raw();
  void updated_image_binary();

  void updated_image_raw   (const cv::Mat&);
  void updated_image_binary(const cv::Mat&);

  void image_sent(const cv::Mat&);
};

#endif // ASSEMBLYTHRESHOLDER_H
