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

#ifndef ASSEMBLYREGISTRYVIEW_H
#define ASSEMBLYREGISTRYVIEW_H

#include <AssemblyUEyeView.h>
#include <LStepExpressMotionManager.h>
#include <nqlogger.h>

#include <vector>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <opencv2/opencv.hpp>

class AssemblyRegistryImageWidget;
class AssemblyRegistryPositionWidget;

class AssemblyRegistryView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyRegistryView(const LStepExpressMotionManager*, QWidget* parent=nullptr);
  virtual ~AssemblyRegistryView() {}

  AssemblyRegistryImageWidget* ImageWidget() const { return image_widget_; }

 protected:

  const LStepExpressMotionManager* motion_manager_;

  AssemblyRegistryImageWidget* image_widget_;

  std::vector<AssemblyRegistryPositionWidget*> v_wpos_;

  QLineEdit*   pos_calc_lineed_from_;
  QLineEdit*   pos_calc_lineed_to_;
  QLineEdit*   pos_calc_lineed_res_;
  QPushButton* pos_calc_button_;

  bool load_position_4vector(std::vector<double>&, QString) const;

 public slots:

  void calculate_relative_distance();
};
// ==================================================

class AssemblyRegistryImageWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyRegistryImageWidget(QWidget* parent=nullptr);
  virtual ~AssemblyRegistryImageWidget() {}

  void    connectImageProducer(const QObject* sender, const char* signal);
  void disconnectImageProducer(const QObject* sender, const char* signal);

 protected:

  void keyReleaseEvent(QKeyEvent*);

  AssemblyUEyeView* img_view_;
  QScrollArea*      img_scroll_;
  QPushButton*      img_save_button_;
  QPushButton*      img_celi_button_;

  //!! FIXME: View should not own (copy of) data, add dedicated controller
  cv::Mat image_;
  cv::Mat image_raw_;

  bool image_modified_;

 public slots:

  void update_image(const cv::Mat&, const bool update_image_raw=true);
  void save_image();
  void modify_image_centerlines();

 signals:

  void image_updated(const cv::Mat&);
};
// ==================================================

class AssemblyRegistryPositionWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyRegistryPositionWidget(const QString&, const LStepExpressMotionManager*, QWidget* parent=nullptr);
  virtual ~AssemblyRegistryPositionWidget() {}

  QString position_qstring() const { return pos_lineed_value_->text(); }

 protected:

  const LStepExpressMotionManager* motion_manager_;

  QPushButton* pos_button_;
  QLineEdit*   pos_lineed_value_;
  QLineEdit*   pos_lineed_comme_;

 public slots:

  void update_position();
};
// ==================================================

#endif // ASSEMBLYREGISTRYVIEW_H
