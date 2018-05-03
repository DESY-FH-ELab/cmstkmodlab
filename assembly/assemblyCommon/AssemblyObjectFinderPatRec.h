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

#ifndef ASSEMBLYOBJECTFINDERPATREC_H
#define ASSEMBLYOBJECTFINDERPATREC_H

#include <QObject>
#include <QString>
#include <QMutex>

#include <opencv2/opencv.hpp>

class AssemblyObjectFinderPatRec : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblyObjectFinderPatRec(const QString&, const QString&, QObject* parent=nullptr);
  virtual ~AssemblyObjectFinderPatRec();

  void set_threshold(const int);
  int  get_threshold() const { return threshold_; }

  void delete_image_master();
  void delete_image_master_PatRec();

 protected:

  static int exe_counter_;

  std::string output_dir_path_;
  std::string output_subdir_name_;

  int threshold_;
  int threshold_template_;

  cv::Mat img_master_;        // original master image
  cv::Mat img_master_PatRec_; // master image used in PatRec (example: binary/post-thresholding version of original master image)

  cv::Mat img_template_;        // original template image
  cv::Mat img_template_PatRec_; // template image used in PatRec (example: binary/post-thresholding version of original template image)

  bool updated_threshold_;
  bool updated_img_master_;
  bool updated_img_master_PatRec_;

  double theta_fine_range_;
  double theta_fine_step_;

  std::vector<double> v_rough_angles_;

  void PatRec(double&, cv::Point&, const cv::Mat&, const cv::Mat&, const double, const int, const std::string& out_dir="") const;

  cv::Point2f RotatePoint(const cv::Point2f&, const double) const;
  cv::Point2f RotatePoint(const cv::Point2f&, const cv::Point2f&, const double) const;

  void draw_RotatedRect(cv::Mat&, const cv::Point&, const double, const double, const double, const cv::Scalar&) const;

 private:
  Q_DISABLE_COPY(AssemblyObjectFinderPatRec)

  mutable QMutex mutex_;

 public slots:

  void update_threshold(const int);

  void acquire_image();

  void update_image_master(const cv::Mat&);
  void update_image_master_PatRec();

  cv::Mat get_image_master_PatRec(const cv::Mat&, const int) const;

  void send_image_master();
  void send_image_master_PatRec();

  void update_rough_angles      (QString);
  void update_angscan_parameters(QString);

  void run_PatRec(const int, const int);
  void run_PatRec_lab_marker(){ this->run_PatRec(1, 0); }

  void template_matching(const cv::Mat&, const cv::Mat&, const cv::Mat&, const int);

 signals:

  void threshold_request();
  void updated_threshold();

  void image_request();

  void updated_image_master(const cv::Mat&);
  void updated_image_master_PatRec(const cv::Mat&);

  void updated_image_master();
  void updated_image_master_PatRec();

  void image_sent(const cv::Mat&);

  void image_path(const int, const QString&);
  void image_mat (const int, const cv::Mat&);

  void updated_rough_angles();
  void updated_angscan_parameters();

  void run_template_matching(const cv::Mat&, const cv::Mat&, const cv::Mat&, const int);

  void reportObjectLocation(int, double, double, double);

  void PatRec_exitcode(const int);
};

#endif // ASSEMBLYOBJECTFINDERPATREC_H
