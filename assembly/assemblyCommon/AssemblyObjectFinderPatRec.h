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

#include <AssemblyThresholder.h>
#include <ApplicationConfig.h>

#include <QObject>
#include <QString>
#include <QMutex>

#include <opencv2/opencv.hpp>

class AssemblyObjectFinderPatRec : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblyObjectFinderPatRec(AssemblyThresholder* const, const QString&, const QString&, QObject* parent=nullptr);
  virtual ~AssemblyObjectFinderPatRec();

  AssemblyThresholder* thresholder() const { return thresholder_; }

  void delete_image_master();
  void delete_image_master_PatRec();

  void save_subdir_images(const bool b=true){ save_subdir_images_ = b; }

  class Configuration {

   public:
    explicit Configuration() { this->reset(); }
    virtual ~Configuration() {}

    void reset();

    bool is_valid() const;

    QString template_filepath_;

    bool thresholding_useThreshold_;
    int  thresholding_threshold_;

    bool thresholding_useAdaptiveThreshold_;
    int  thresholding_blocksize_;

    std::vector<double> angles_prescan_vec_;
    double              angles_finemax_;
    double              angles_finestep_;
  };

 private:
  Q_DISABLE_COPY(AssemblyObjectFinderPatRec)

 protected:

  mutable QMutex mutex_;

  double mm_per_pixel_row_;
  double mm_per_pixel_col_;

  AssemblyThresholder* const thresholder_;

  const ApplicationConfig* config_;

  static int exe_counter_;

  std::string output_dir_path_;
  std::string output_subdir_name_;

  bool save_subdir_images_;

  cv::Mat img_master_;        // original master image
  cv::Mat img_master_PatRec_; // master image used in PatRec (example: binary/post-thresholding version of original master image)

  cv::Mat img_template_;        // original template image
  cv::Mat img_template_PatRec_; // template image used in PatRec (example: binary/post-thresholding version of original template image)

  bool updated_img_master_;
  bool updated_img_master_PatRec_;

  void PatRec(double&, cv::Point&, const cv::Mat&, const cv::Mat&, const double, const int, const std::string& out_dir="") const;

  cv::Point2f RotatePoint(const cv::Point2f&, const double) const;
  cv::Point2f RotatePoint(const cv::Point2f&, const cv::Point2f&, const double) const;

  void draw_RotatedRect(cv::Mat&, const cv::Point&, const double, const double, const double, const cv::Scalar&) const;

 public slots:

  void update_image_master       (const cv::Mat&);
  void update_image_master_PatRec(const cv::Mat&);

  void launch_PatRec(const AssemblyObjectFinderPatRec::Configuration&);

  void template_matching(const Configuration&, const cv::Mat&, const cv::Mat&, const cv::Mat&);

 signals:

  void updated_image_master(const cv::Mat&);
  void updated_image_master();

  void updated_image_master_PatRec(const cv::Mat&);
  void updated_image_master_PatRec();

  void PatRec_res_image_master_edited(const QString&);
  void PatRec_res_image_master_edited(const cv::Mat&);

  void PatRec_res_image_angscan(const QString&);
  void PatRec_res_image_angscan(const cv::Mat&);

  void PatRec_res_image_master_PatRec(const QString&);
  void PatRec_res_image_master_PatRec(const cv::Mat&);

  void PatRec_res_image_template_PatRec(const QString&);
  void PatRec_res_image_template_PatRec(const cv::Mat&);

  void updated_prescan_angles();
  void updated_angscan_parameters();

  void template_matching_request(const Configuration&, const cv::Mat&, const cv::Mat&, const cv::Mat&);

  void PatRec_results(const double, const double, const double);

  void PatRec_exitcode(const int);

  void DBLogMessage(const QString);
};

#endif // ASSEMBLYOBJECTFINDERPATREC_H
