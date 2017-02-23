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

#include <AssemblyVUEyeCamera.h>
#include <AssemblyVMarkerFinder.h>
#include <AssemblyUEyeView.h>

class AssemblyThresholdTuner : public QWidget
{
    Q_OBJECT
public:

    explicit AssemblyThresholdTuner(QWidget *parent = 0);

    void connectImageProducer(const QObject* sender, const char* signal);
    void disconnectImageProducer(const QObject* sender, const char* signal);

protected:

    void keyReleaseEvent(QKeyEvent *event);

    QScrollArea *scrollArea_;
    AssemblyUEyeView *imageView_;

    QPushButton* setThresholdButton;
    QLabel* label;
    QLineEdit* lineEdit;

    cv::Mat image_;

public slots:

    void snapShot();
    void imageAcquired(const cv::Mat&);
    void setNewThreshold();
    //void updateThresholdLabel(int value) { lineEdit -> setText(std::to_string(value)); }
    void updateThresholdLabel(int value) { lineEdit -> setText("123456"); }

 signals:
    
    void setNewThreshold(int);
    void updateThresholdLabel();

};

#endif // ASSEMBLYTHRESHOLDTUNER_H
