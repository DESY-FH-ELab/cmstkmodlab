
#ifndef _DEFORECOSURFACE_H
#define _DEFORECOSURFACE_H

#include <utility>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>

#include "DefoPoint.h"
#include "DefoSurface.h"
#include "DefoSpline.h"
#include "DefoConfigReader.h"


///
/// perform surface reconstruction
/// based on reco point collection
///
class DefoRecoSurface {

 public:
  DefoRecoSurface();
  const DefoSurface reconstruct( DefoPointCollection&, DefoPointCollection& );

 private:
  const DefoSplineField createXYSplines( DefoPointCollection const& );
  const DefoSplineField createZSplines( DefoPointCollection const&, DefoPointCollection const& );
  const DefoSplineField createZSplinesOld( DefoPointCollection const&, DefoPointCollection const& );
  const std::pair<std::vector<DefoPointCollection>,std::vector<DefoPointCollection> > groupPointsSorted( DefoPointCollection const& );
  void mountZSplines( DefoSplineField& ) const;
  void mountZSplinesOld( DefoSplineField& ) const;
  const std::pair<double,double> determineAverageSpacing( DefoPointCollection const& ) const;
  std::pair<unsigned int, unsigned int> indexPoints( DefoPointCollection& ) const;
  std::pair<bool,DefoPointCollection::iterator> findClosestPoint( DefoPoint const&, DefoPointCollection& ) const;
  const std::pair<bool,DefoPointCollection::const_iterator> findPointByIndex( DefoPointCollection const&, std::pair<int,int> const& ) const;
  void removeGlobalOffset( DefoSplineField& ) const;
  void removeTilt( DefoSplineField& ) const;

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

};


#endif
