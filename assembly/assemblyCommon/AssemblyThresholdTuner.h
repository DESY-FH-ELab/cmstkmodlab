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
public slots:

    void snapShot();
    void imageAcquired(const cv::Mat&);
    void setNewThreshold();
    void updateThresholdLabel(int value) { 
    NQLog("AssemblyThresholdTuner") << " : INFO!! : threshold received. Value = " << value; lineEdit -> setText(QString::number(value)); }

 signals:
    
    void setNewThreshold(int);
    void updateThresholdLabel();

};

#endif // ASSEMBLYTHRESHOLDTUNER_H
