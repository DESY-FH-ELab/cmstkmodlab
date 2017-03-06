#ifndef ASSEMBLYTHRESHOLDTUNER_H
#define ASSEMBLYTHRESHOLDTUNER_H

#include <opencv2/opencv.hpp>

#include <string>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QString>

#include <AssemblyVUEyeCamera.h>
#include <AssemblyVMarkerFinder.h>
#include <AssemblyUEyeView.h>
#include "nqlogger.h"



class AssemblyThresholdTuner : public QWidget
{
    Q_OBJECT
public:

    explicit AssemblyThresholdTuner(QWidget *parent = 0);

    void connectImageProducer(const QObject* sender, const char* signal);
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
