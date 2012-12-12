#ifndef DEFORECOIMAGEZOOMMODEL_H
#define DEFORECOIMAGEZOOMMODEL_H

#include <QObject>
#include <QImage>
#include <QSize>

class DefoRecoImageZoomModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoRecoImageZoomModel(QObject *parent = 0);
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

#endif // DEFORECOIMAGEZOOMMODEL_H
