#ifndef ASSEMBLYUEYEVIEW_H
#define ASSEMBLYUEYEVIEW_H

#include <opencv2/opencv.hpp>

#include <QLabel>
#include <QImage>
#include <QMutex>

#include "AssemblyVUEyeCamera.h"

class AssemblyUEyeView : public QLabel
{
    Q_OBJECT
public:
  explicit AssemblyUEyeView(QWidget *parent = 0);

    void connectCamera(AssemblyVUEyeCamera* camera);
    void disconnectCamera(AssemblyVUEyeCamera* camera);

protected:

    void paintEvent(QPaintEvent*);

  AssemblyVUEyeCamera *camera_;

  QImage image_;

  QMutex mutex_;

public slots:

  void setImage(const cv::Mat&);
};

#endif // ASSEMBLYUEYEWIDGET_H
