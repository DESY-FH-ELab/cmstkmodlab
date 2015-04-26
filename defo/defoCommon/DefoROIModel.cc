#include <iostream>
#include <cmath>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoROIModel.h"

DefoROIModel::DefoROIModel(QObject *parent)
: QObject(parent),
  roi_(0)
{

}

QRectF DefoROIModel::boundingRect() const
{
  if (!roi_) return QRectF(0.05, 0.05, 0.90, 0.90);
  return roi_->boundingRect();
}

int DefoROIModel::size() const
{
  if (!roi_) return 0;
  return roi_->size();
}

const QPointF DefoROIModel::at(int i) const
{
  if (!roi_) return QPointF();
  return roi_->at(i);
}

const QPointF DefoROIModel::first() const
{
  if (!roi_) return QPointF();
  return roi_->first();
}

const QPointF DefoROIModel::last() const
{
  if (!roi_) return QPointF();
  return roi_->last();
}

float DefoROIModel::getClosestPoint(int & index,
                                    float width, float height,
                                    float x, float y)
{
  if (!roi_) return 1.e9;
  return roi_->getClosestPoint(index, width, height, x, y);
}

void DefoROIModel::setPoint(int index, float newX, float newY)
{
  if (!roi_) return;
  if (roi_->setPoint(index, newX, newY)) emit roiChanged();
}

int DefoROIModel::getSelectedPoint() const
{
  if (!roi_) return -1;
  return roi_->getSelectedPoint();
}

void DefoROIModel::selectPoint(int index)
{
  if (roi_) roi_->selectPoint(index);
}

int DefoROIModel::numberOfPoints() const
{
  if (!roi_) return -1;
  return roi_->numberOfPoints();
}

bool DefoROIModel::containsPoint(float width, float height,
                                 float x, float y)
{
  if (!roi_) return false;
  return roi_->containsPoint(width, height, x, y);
}

void DefoROIModel::insertPointBefore()
{
  if (!roi_) return;
  if (roi_->insertPointBefore()) emit roiChanged();
}

void DefoROIModel::insertPointAfter()
{
  if (!roi_) return;
  if (roi_->insertPointAfter()) emit roiChanged();
}

void DefoROIModel::removePoint()
{
  if (!roi_) return;
  if (roi_->removePoint()) emit roiChanged();
}

void DefoROIModel::assignFrom(DefoROI* roi)
{
  if (!roi_ || !roi) return;
  if (roi_->assignFrom(roi)) emit roiChanged();
}
}
