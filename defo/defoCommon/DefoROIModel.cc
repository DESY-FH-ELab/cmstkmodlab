/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>

#include <QFile>
#include <QXmlStreamWriter>

#include <nqlogger.h>

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
  if (roi_->setPoint(index, newX, newY)) {
    // NQLogMessage("DefoROIModel") << "roiChanged(false) " << roi_;
    emit roiChanged(false);
  }
}

int DefoROIModel::getSelectedPoint() const
{
  if (!roi_) return -1;
  return roi_->getSelectedPoint();
}

void DefoROIModel::selectPoint(int index)
{
  if (roi_) {
    if (roi_->getSelectedPoint()!=-1) {
      // NQLogMessage("DefoROIModel") << "roiChanged(true) " << roi_;
      emit roiChanged(true);
    }
    roi_->selectPoint(index);
  }
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
  if (roi_->insertPointBefore()) emit roiChanged(true);
}

void DefoROIModel::insertPointAfter()
{
  if (!roi_) return;
  if (roi_->insertPointAfter()) emit roiChanged(true);
}

void DefoROIModel::removePoint()
{
  if (!roi_) return;
  if (roi_->removePoint()) emit roiChanged(true);
}

void DefoROIModel::assignFrom(DefoROI* roi)
{
  if (!roi_ || !roi) return;
  if (roi_->assignFrom(roi)) emit roiChanged(true);
}

void DefoROIModel::assignTo(DefoROI* roi)
{
  if (!roi_ || !roi) return;
  roi_->assignTo(roi);
}

bool DefoROIModel::isEqualTo(DefoROIModel* other)
{
  if (!this->roi_) return false;
  if (this->roi_==other->roi_) return true;
  return false;
}

bool DefoROIModel::write(const QString& filename)
{
  if (!this->roi_) return false;
  this->roi_->write(filename);
  return true;
}

bool DefoROIModel::read(const QString& filename)
{
  if (!this->roi_) return false;
  this->roi_->read(filename);
  emit roiChanged(true);
  return true;
}
