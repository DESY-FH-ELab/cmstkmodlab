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

 
#ifndef _DEFORECOSURFACE_H
#define _DEFORECOSURFACE_H

#include <utility>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>

#include <QObject>

#include "DefoPoint.h"
#include "DefoSurface.h"
#include "DefoSpline.h"

///
/// perform surface reconstruction
/// based on reco point collection
///
class DefoRecoSurface : public QObject
{
   Q_OBJECT
 public:
  DefoRecoSurface(QObject *parent = 0);
  ~DefoRecoSurface() {}

  const DefoSurface reconstruct( DefoPointCollection&, DefoPointCollection& );

  void setSpacingEstimate( int s ) { spacingEstimate_ = s; }
  void setSearchPathHalfWidth( int s ) { searchPathHalfWidth_ = s; }
  void setNominalGridDistance( double n ) { nominalGridDistance_ = n; }
  void setNominalCameraDistance( double n ) { nominalCameraDistance_ = n; calculateHelpers(); }
  void setNominalViewingAngle( double a ) { nominalViewingAngle_ = a; calculateHelpers(); }
  void setPitchX( double p ) { pitchX_ = p; }
  void setPitchY( double p ) { pitchY_ = p; }
  void setFocalLength( double l ) { focalLength_ = l; }
  void setImageSize(std::pair<double, double> imageSize) { imageSize_ = imageSize; }
  std::vector<DefoPoint> const& getIndexedPoints( void ) { return indexedPoints_; }
  void calculateHelpers(void);

  void setLensParameters(double p0, double p1, double p2, double p3);

  void setCalibX( double v) { calibX_ = v; }
  void setCalibY( double v) { calibY_ = v; }
  void setCalibZx( double v) { calibZx_ = v; }
  void setCalibZy( double v) { calibZy_ = v; }

  void setAngle1(double v) { angle1_ = v; }
  void setAngle2(double v) { angle2_ = v; }
  void setAngle3(double v) { angle3_ = v; }
  void setDistance(double v) { distance_ = v; }
  void setHeight1(double v) { height1_ = v; }
  void setHeight2(double v) { height2_ = v; }

  void dump();

 private:

  void calibrateXYPoints(DefoPointCollection & points);

  const DefoSplineField createZSplines( DefoPointCollection const&, DefoPointCollection const& );
  void mountZSplines( DefoSplineField& ) const;
  const std::pair<bool,DefoPointCollection::const_iterator> findPointByIndex( DefoPointCollection const&, std::pair<int,int> const& ) const;
  void removeGlobalOffset( DefoSplineField& ) const;
  void removeTilt( DefoSplineField& ) const;
  double imageScale(double focalLength) const;

  int spacingEstimate_;
  int searchPathHalfWidth_;
  double nominalGridDistance_;
  double nominalCameraDistance_;
  double nominalViewingAngle_;
  double heightAboveSensor_;
  double horizontalDistanceToSensor_;
  double pitchX_;
  double pitchY_;
  double focalLength_;
  unsigned int debugLevel_;
  std::pair<double, double> imageSize_;
  std::vector<DefoPoint> indexedPoints_;

  double lensP0_, lensP1_, lensP2_, lensP3_;
 
  double calibX_;
  double calibY_;
  double calibZx_;
  double calibZy_;

  double angle1_;
  double angle2_;
  double angle3_;
  double distance_;
  double height1_;
  double height2_;

signals:

  void incrementRecoProgress();
};


#endif
