#ifndef ASSEMBLYUEYESNAPSHOOTER_H
#define ASSEMBLYUEYESNAPSHOOTER_H

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>

#include <AssemblyVUEyeCamera.h>
#include <AssemblyVMarkerFinder.h>
#include <AssemblyUEyeView.h>

class AssemblyUEyeSnapShooter : public QWidget
{
    Q_OBJECT
public:

    explicit AssemblyUEyeSnapShooter(QWidget *parent = 0);

    void connectImageProducer(const QObject* sender, const char* signal);
    void disconnectImageProducer(const QObject* sender, const char* signal);

protected:

    void keyReleaseEvent(QKeyEvent *event);

    QScrollArea *scrollArea_;
    AssemblyUEyeView *imageView_;

    cv::Mat image_;

public slots:

    void snapShot();
    void imageAcquired(const cv::Mat&);
};

#endif // ASSEMBLYUEYEWIDGET_H
