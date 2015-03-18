#ifndef DEFOIMAGEZOOMMODEL_H
#define DEFOIMAGEZOOMMODEL_H

#include <QObject>
#include <QImage>
#include <QSize>

class DefoImageZoomModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoImageZoomModel(QObject *parent = 0);
  QSize getZoomedSize(const QSize& size, const QImage& image);
  float getScaling() const { return scaling_; }

public slots:
  void setZoomFactor(float factor);
  void increaseZoomFactor();
  void decreaseZoomFactor();

protected:
  float zoomFactor_;
  float scaling_;

signals:
  void zoomFactorChanged(float newScaling, float oldScaling);
};

#endif // DEFOIMAGEZOOMMODEL_H
