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

    void    connectImageProducer(const QObject* sender, const char* signal);
    void disconnectImageProducer(const QObject* sender, const char* signal);

    QPushButton* button;

 protected:

    void keyReleaseEvent(QKeyEvent *event);

    QScrollArea *scrollArea1_;
    AssemblyUEyeView *imageView1_;

    QScrollArea *scrollArea2_;
    AssemblyUEyeView *imageView2_;

    QPushButton* setThresholdButton;
    QLabel* label;
    QLineEdit* lineEdit;

    cv::Mat image_;

 public slots:

    void snapShot();
    //void imageAcquired(const cv::Mat&);
    void imageAcquired(cv::Mat);
    void setNewThreshold();
    void updateThresholdLabelSlot(int);
    void enableThresholdButton();
    void disableThresholdButton();
    void updateThresholdImage(QString);

 signals:

    void setNewThreshold(int, cv::Mat);
    void updateThresholdLabel();
    void updateThresholdImagePicture();

};

#endif // ASSEMBLYTHRESHOLDTUNER_H
