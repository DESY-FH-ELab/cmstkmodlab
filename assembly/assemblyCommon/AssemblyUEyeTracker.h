#ifndef ASSEMBLYUEYETRACKER_H
#define ASSEMBLYUEYETRACKER_H

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>

#include <AssemblyVUEyeCamera.h>
#include <AssemblyVMarkerFinder.h>
#include <AssemblyUEyeView.h>

class AssemblyUEyeTracker : public QWidget
{
    Q_OBJECT
public:
    explicit AssemblyUEyeTracker(QWidget *parent = 0);

    cv::Vec3f mean(std::vector<cv::Vec3f> vec_circles);
    void connectImageProducer_tracker(const QObject* sender, const char* signal);
    void disconnectImageProducer(const QObject* sender, const char* signal);

protected:
    int _step = 0;
    void keyReleaseEvent(QKeyEvent *event);

    QScrollArea *scrollArea_;
    AssemblyUEyeView *imageView_;

    cv::Mat image_;

public slots:

    void snapShot();
    void stream();
    void track();

    void imageAcquired(const cv::Mat&);

signals:
    void getframe();

};

#endif // ASSEMBLYUEYEWIDGET_H
