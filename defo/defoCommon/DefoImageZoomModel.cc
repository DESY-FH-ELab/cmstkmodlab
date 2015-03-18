#include <iostream>

#include "DefoImageZoomModel.h"

DefoImageZoomModel::DefoImageZoomModel(QObject *parent)
  : QObject(parent)
{
  zoomFactor_ = 0.0;
}

QSize DefoImageZoomModel::getZoomedSize(const QSize& size, const QImage& image)
{
  float width = image.width();
  float height = image.height();    

  if (zoomFactor_==0.0) {
    float aspectRatio = width/height;
    //std::cout << width << ", " << height << ", " << aspectRatio << std::endl;
    //std::cout << size.width() << ", " << size.height() << ", " << size.width()/aspectRatio << std::endl;
    
    if (size.width()/aspectRatio > size.height()) {
      //std::cout << "1 " << size.height()*aspectRatio << ", " << size.height() << std::endl;
      scaling_ = size.height()*aspectRatio/width;
      return QSize(size.height()*aspectRatio,
		   size.height());
    } else {
      //std::cout << "2 "  << size.width() << ", " << size.width()/aspectRatio << std::endl;
      scaling_ = size.width()/width;
      return QSize(size.width(),
		   size.width()/aspectRatio);
    }
  }

  scaling_ = zoomFactor_;

  return QSize(width*zoomFactor_, height*zoomFactor_);
}

void DefoImageZoomModel::setZoomFactor(float factor)
{
  if (factor<0.0 || factor>1.0) return;

  if (zoomFactor_!=factor) {
    zoomFactor_ = factor;
    float oldScaling = scaling_;
    scaling_ = zoomFactor_;
    emit zoomFactorChanged(scaling_, oldScaling);
  }
}

void DefoImageZoomModel::increaseZoomFactor()
{
  if (zoomFactor_==1.0) return;
  zoomFactor_ += 0.25;
  float oldScaling = scaling_;
  scaling_ = zoomFactor_;
  emit zoomFactorChanged(scaling_, oldScaling);
}

void DefoImageZoomModel::decreaseZoomFactor()
{
  if (zoomFactor_==0.0) return;
  zoomFactor_ -= 0.25;
  float oldScaling = scaling_;
  scaling_ = zoomFactor_;
  emit zoomFactorChanged(scaling_, oldScaling);
}
