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

#ifndef ASSEMBLYAUTOFOCUSVIEW_H
#define ASSEMBLYAUTOFOCUSVIEW_H

#include <AssemblyUEyeView.h>

#include <QWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QKeyEvent>
#include <QString>

#include <opencv2/opencv.hpp>

class AssemblyAutoFocusView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyAutoFocusView(QWidget* parent=nullptr);
  virtual ~AssemblyAutoFocusView();

  void    connectImageProducer(const QObject* sender, const char* signal);
  void disconnectImageProducer(const QObject* sender, const char* signal);

  void update_scan_config(const double, const int);

 protected:

  void keyReleaseEvent(QKeyEvent*);

  AssemblyUEyeView* imageView_1_;
  AssemblyUEyeView* imageView_2_;

  QScrollArea* scrollArea_1_;
  QScrollArea* scrollArea_2_;

  QPushButton* button_1_;

  QLineEdit * lineed_1_;
  QLineEdit * lineed_2_;

 public slots:

  void configure_scan();
  void read_graph(const QString&);
  void updateText(double);

 signals:

  void scan_config(const double, const int);

  void graph_found(const cv::Mat&);
};

#endif // ASSEMBLYAUTOFOCUSVIEW_H
