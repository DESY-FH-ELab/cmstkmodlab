#ifndef ASSEMBLYUEYESNAPSHOOTER_H
#define ASSEMBLYUEYESNAPSHOOTER_H

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QScrollArea>

#include "AssemblyVUEyeCamera.h"

#include <AssemblyUEyeView.h>

class AssemblyUEyeSnapShooter : public QWidget
{
    Q_OBJECT
public:

  explicit AssemblyUEyeSnapShooter(QWidget *parent = 0);

    void connectCamera(AssemblyVUEyeCamera *camera);
    void disconnectCamera(AssemblyVUEyeCamera *camera);

protected:

  AssemblyVUEyeCamera* camera_;

  QScrollArea *scrollArea_;
  AssemblyUEyeView *imageView_;

  bool takeSnapShot_;

public slots:

  void snapShot();
  void imageAcquired(const cv::Mat&);
};

#endif // ASSEMBLYUEYEWIDGET_H
