/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
