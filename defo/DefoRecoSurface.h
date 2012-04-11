 
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

  void setSpacingEstimate( int s ) { spacingEstimate_ = s; }
  void setSearchPathHalfWidth( int s ) { searchPathHalfWidth_ = s; }
  void setNominalGridDistance( double n ) { nominalGridDistance_ = n; }
  void setNominalCameraDistance( double n ) { nominalCameraDistance_ = n; calculateHelpers(); }
  void setNominalViewingAngle( double a ) { nominalViewingAngle_ = a; calculateHelpers(); }
  void setPitchX( double p ) { pitchX_ = p; }
  void setPitchY( double p ) { pitchY_ = p; }
  void setFocalLength( double l ) { focalLength_ = l; }
  std::pair<unsigned int, unsigned int> indexPoints( DefoPointCollection& );
  std::vector<DefoPoint> const& getIndexedPoints( void ) { return indexedPoints_; }

 private:
  const DefoSplineField createXYSplines( DefoPointCollection const& );
  const DefoSplineField createZSplines( DefoPointCollection const&, DefoPointCollection const& );
  const DefoSplineField createZSplinesOld( DefoPointCollection const&, DefoPointCollection const& );
  const std::pair<std::vector<DefoPointCollection>,std::vector<DefoPointCollection> > groupPointsSorted( DefoPointCollection const& );
  void mountZSplines( DefoSplineField& ) const;
  void mountZSplinesOld( DefoSplineField& ) const;
  const std::pair<double,double> determineAverageSpacing( DefoPointCollection const& ) const;
  std::pair<bool,DefoPointCollection::iterator> findClosestPoint( DefoPoint const&, DefoPointCollection& ) const;
  std::pair<bool,DefoPointCollection::iterator> findClosestPointExcluded( DefoPoint const&, DefoPointCollection&, DefoPoint const& ) const;
  const std::pair<bool,DefoPointCollection::const_iterator> findPointByIndex( DefoPointCollection const&, std::pair<int,int> const& ) const;
  void removeGlobalOffset( DefoSplineField& ) const;
  void removeTilt( DefoSplineField& ) const;
  void calculateHelpers( void );

/*   std::pair<unsigned int, unsigned int> refPointIndexRange_; */
/*   std::pair<unsigned int, unsigned int> recoPointIndexRange_; */
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
  std::vector<DefoPoint> indexedPoints_;

};


#endif
