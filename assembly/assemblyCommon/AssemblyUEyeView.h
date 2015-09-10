#ifndef ASSEMBLYUEYEVIEW_H
#define ASSEMBLYUEYEVIEW_H

#include <opencv2/opencv.hpp>

#include <QLabel>
#include <QImage>
#include <QMutex>

#include <AssemblyVUEyeCamera.h>
#include <AssemblyVMarkerFinder.h>

class AssemblyUEyeView : public QLabel
{
    Q_OBJECT
public:
    explicit AssemblyUEyeView(QWidget *parent = 0);

    void connectImageProducer(const QObject* sender, const char* signal);
    void disconnectImageProducer(const QObject* sender, const char* signal);

    void setZoomFactor(float zoomFactor);
    void increaseZoomFactor();
    void decreaseZoomFactor();

protected:

    void paintEvent(QPaintEvent*);

    QImage image_;
    float zoomFactor_;

    QMutex mutex_;

public slots:

    void setImage(const cv::Mat&);
};

#endif // ASSEMBLYUEYEWIDGET_H
