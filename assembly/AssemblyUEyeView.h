#ifndef ASSEMBLYUEYEVIEW_H
#define ASSEMBLYUEYEVIEW_H

#include <QLabel>
#include <QImage>

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

public slots:

  void setImage(const QImage&);
};

#endif // ASSEMBLYUEYEWIDGET_H
