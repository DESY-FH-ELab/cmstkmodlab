#ifndef ASSEMBLYUEYEVIEW_H
#define ASSEMBLYUEYEVIEW_H

#include <QLabel>
#include <QImage>

#ifdef USE_FAKEIO
#include "AssemblyUEyeFakeCamera.h"
typedef AssemblyUEyeFakeCamera AssemblyUEyeCamera_t;
#else
#include "AssemblyUEyeCamera.h"
typedef AssemblyUEyeCamera AssemblyUEyeCamera_t;
#endif

class AssemblyUEyeView : public QLabel
{
    Q_OBJECT
public:
  explicit AssemblyUEyeView(QWidget *parent = 0);

protected:

    void paintEvent(QPaintEvent*);

  AssemblyUEyeCamera_t* camera_;

  QImage image_;

public slots:

  void setImage(const QImage&);
};

#endif // ASSEMBLYUEYEWIDGET_H
