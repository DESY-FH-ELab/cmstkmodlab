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

#ifndef ASSEMBLYTHRESHOLDTUNER_H
#define ASSEMBLYTHRESHOLDTUNER_H

#include <AssemblyVUEyeCamera.h>
#include <AssemblyUEyeView.h>
#include <nqlogger.h>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QString>

#include <opencv2/opencv.hpp>

class AssemblyThresholdTuner : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyThresholdTuner(QWidget* parent=0);
  virtual ~AssemblyThresholdTuner() {}

  int get_threshold() const;

  void    connectImageProducer_1(const QObject* sender, const char* signal);
  void disconnectImageProducer_1(const QObject* sender, const char* signal);

  void    connectImageProducer_2(const QObject* sender, const char* signal);
  void disconnectImageProducer_2(const QObject* sender, const char* signal);

 protected:

  void keyReleaseEvent(QKeyEvent *event);

  AssemblyUEyeView* imageView_1_;
  AssemblyUEyeView* imageView_2_;

  QScrollArea* scrollArea_1_;
  QScrollArea* scrollArea_2_;

  QPushButton* thre_button_;
  QLabel*      thre_label_;
  QLineEdit*   thre_linee_;

 public slots:

  void read_threshold();

 signals:

  void threshold_value(const int);
};

#endif // ASSEMBLYTHRESHOLDTUNER_H
