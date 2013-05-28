
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <QHash>

#include "DefoSurface.h"


inline bool operator==(const DefoSplineXYPair &lhs, const DefoSplineXYPair &rhs)
{
  return lhs.x_==rhs.x_ && lhs.y_==rhs.y_;
}

inline uint qHash(const DefoSplineXYPair &key)
{
  return (qHash((uint)key.x_) ^ (qHash((uint)key.y_)<<1));
}

///
///
///
DefoSurface::DefoSurface() {

  isSplineField_ = false;
}

DefoSurface::DefoSurface(const DefoSurface& other) {

  points_ = other.points_;
  splineField_ = other.splineField_;
  pointFields_ = other.pointFields_;

  isSplineField_ = other.isSplineField_;
  isPoints_ = other.isPoints_;

  makeStats();
}

void DefoSurface::makeStats() {

    typedef QHash<DefoSplineXYPair,DefoSplineXYDefoPair>::iterator it_t;
    DefoSplineXYPair key;
    DefoSplineXYDefoPair value;

    // first "along-x" splines
    DefoSplineSetXCollection const& splinesX = splineField_.first;

    // loop set of spline "rows", itC is a DefoSplineSetX
    for( DefoSplineSetXCollection::const_iterator itC = splinesX.begin();
         itC < splinesX.end();
         ++itC ) {

      // loop the points
      for( DefoPointCollection::const_iterator itP = itC->getPoints().begin();
           itP < itC->getPoints().end();
           ++itP ) {

        key.ix_ = itP->getIndex().first;
        key.x_ = itP->getX();
        key.iy_ = itP->getIndex().second;
        key.y_ = itP->getY();
        value.setX(itC->eval(itP->getX()));

        it_t it = defoPointMap_.find(key);
        if (it==defoPointMap_.end()) {
          value.y_ = 0;
          value.hasy_ = false;
          defoPointMap_.insert(key, value);
        } else {
          it.value().setX(itC->eval(itP->getX()));
        }
      }
    }

    // then "along-y" splines
    DefoSplineSetYCollection const& splinesY = splineField_.second;

    // loop set of spline "rows", itC is a DefoSplineSetX
    for( DefoSplineSetYCollection::const_iterator itC = splinesY.begin();
         itC < splinesY.end();
         ++itC ) {

      // loop the points
      for( DefoPointCollection::const_iterator itP = itC->getPoints().begin();
           itP < itC->getPoints().end();
           ++itP ) {

        key.ix_ = itP->getIndex().first;
        key.x_ = itP->getX();
        key.iy_ = itP->getIndex().second;
        key.y_ = itP->getY();
        value.setY(itC->eval(itP->getY()));

        it_t it = defoPointMap_.find(key);
        if (it==defoPointMap_.end()) {
          value.x_ = 0;
          value.hasx_ = false;
          defoPointMap_.insert(key, value);
        } else {
          it.value().setY(itC->eval(itP->getY()));
        }

      }
    }

    stats_.maxZFromXSplines = -1e9;
    stats_.minZFromXSplines = +1e9;

    // "along-x" splines
    DefoSplineSetXCollection const& splinesXminmax = splineField_.first;

    // loop set of spline "rows", itC is a DefoSplineSetX
    for( DefoSplineSetXCollection::const_iterator itC = splinesXminmax.begin();
         itC < splinesXminmax.end();
         ++itC ) {

        // loop the points
        for( DefoPointCollection::const_iterator itP = itC->getPoints().begin();
             itP < itC->getPoints().end();
             ++itP ) {

            double z = itC->eval(itP->getX());
            if (z>stats_.maxZFromXSplines) {
                stats_.maxZFromXSplines = z;
                stats_.posAtMaxZFromXSplines.first = itP->getX();
                stats_.posAtMaxZFromXSplines.second = itP->getY();
            }
            if (z<stats_.minZFromXSplines) {
                stats_.minZFromXSplines = z;
                stats_.posAtMinZFromXSplines.first = itP->getX();
                stats_.posAtMinZFromXSplines.second = itP->getY();
            }
        }
    }

    stats_.maxZFromYSplines = -1e9;
    stats_.minZFromYSplines = +1e9;

    // "along-y" splines
    DefoSplineSetYCollection const& splinesYminmax = splineField_.second;

    // loop set of spline "rows", itC is a DefoSplineSetX
    for( DefoSplineSetYCollection::const_iterator itC = splinesYminmax.begin();
         itC < splinesYminmax.end();
         ++itC ) {

        // loop the points
        for( DefoPointCollection::const_iterator itP = itC->getPoints().begin();
             itP < itC->getPoints().end();
             ++itP ) {

            double z = itC->eval(itP->getY());
            if (z>stats_.maxZFromYSplines) {
                stats_.maxZFromYSplines = z;
                stats_.posAtMaxZFromYSplines.first = itP->getX();
                stats_.posAtMaxZFromYSplines.second = itP->getY();
            }
            if (z<stats_.minZFromYSplines) {
                stats_.minZFromYSplines = z;
                stats_.posAtMinZFromYSplines.first = itP->getX();
                stats_.posAtMinZFromYSplines.second = itP->getY();
            }
        }
    }

}

///
///
///
void DefoSurface::dumpSplineField( std::string& filename ) const {

    typedef QHash<DefoSplineXYPair,DefoSplineXYDefoPair>::const_iterator it_t;

    if (filename.length()==0) return;

    std::ofstream ofile(filename.c_str());
    for (it_t it = defoPointMap_.begin();it!=defoPointMap_.end();++it) {
        ofile << std::setw(8)  << it.key().ix_ << " "
              << std::setw(8)  << it.key().iy_ << " "
              << std::setw(14) << it.key().x_ << " "
              << std::setw(14) << it.key().y_ << " "
              << std::setw(14) << it.value().x_ << " "
              << std::setw(3)  << (int)it.value().hasx_ << " "
              << std::setw(14) << it.value().y_ << " "
              << std::setw(3)  << (int)it.value().hasy_
              << std::endl;
    }
}

///
///
///
void DefoSurface::dumpStats( std::string& filename ) const {

    if (filename.length()==0) return;

    std::ofstream ofile(filename.c_str());

    ofile << std::setw(8)  << stats_.maxZFromXSplines << " "
          << std::setw(8)  << stats_.posAtMaxZFromXSplines.first << " "
          << std::setw(8)  << stats_.posAtMaxZFromXSplines.second << std::endl;
    ofile << std::setw(8)  << stats_.minZFromXSplines << " "
          << std::setw(8)  << stats_.posAtMinZFromXSplines.first << " "
          << std::setw(8)  << stats_.posAtMinZFromXSplines.second << std::endl;

    ofile << std::setw(8)  << stats_.maxZFromYSplines << " "
          << std::setw(8)  << stats_.posAtMaxZFromYSplines.first << " "
          << std::setw(8)  << stats_.posAtMaxZFromYSplines.second << std::endl;
    ofile << std::setw(8)  << stats_.minZFromYSplines << " "
          << std::setw(8)  << stats_.posAtMinZFromYSplines.first << " "
          << std::setw(8)  << stats_.posAtMinZFromYSplines.second << std::endl;
}

///
/// create pair of DefoPointField,
/// one for x-splines, one for y-splines;
///
void DefoSurface::createPointFields( void ) {

  if( !isSplineField_ ) {
    std::cerr << " [DefoSurface::createPointFields] ** ERROR: Spline field not set. Doing nothing." << std::endl;
    return;
  }

  // determine index range
  std::pair<unsigned int, unsigned int> indexRange( 0, 0 );
  for( DefoPointCollection::const_iterator it = points_.begin(); it != points_.end(); ++it ) {
    if( abs( it->getIndex().first  ) > (int)indexRange.first  ) indexRange.first  = abs( it->getIndex().first   );
    if( abs( it->getIndex().second ) > (int)indexRange.second ) indexRange.second = abs( it->getIndex().second  );

    if( !it->isIndexed() ) {
      std::cerr << " [DefoSurface::createPointFields] ** WARNING: Point not indexed at position: x: " 
		<< it->getX() << " y: " << it->getY() << std::endl;
    }

  }


  // create matrix accordingly
  pointFields_.first.resize( indexRange.first * 2 + 1 );
  for( DefoPointField::iterator it = pointFields_.first.begin(); it != pointFields_.first.end(); ++it ) {
    it->resize( indexRange.second * 2 + 1 );
  }

  //  std::cout << " [DefoSurface::createPointFields] =2= matrix dimensions: x: " << indexRange.first << " y: " << indexRange.second << std::endl;

  

  // first "along-x" splines
  DefoSplineSetXCollection const& splinesX = splineField_.first;
  
  // loop set of spline "rows", itC is a DefoSplineSetX
  for( DefoSplineSetXCollection::const_iterator itC = splinesX.begin(); itC < splinesX.end(); ++itC ) {
  
    // loop the points, determine matrix index
    std::vector<DefoPoint> aColumn;
    for( DefoPointCollection::const_iterator itP = itC->getPoints().begin(); itP < itC->getPoints().end(); ++itP ) {
      DefoPoint aPoint( *itP ); // make copy to preserve constness
      aPoint.setHeight( itC->eval( itP->getX() ) );
      aPoint.setValid( true ); // sign for the DefoSurfacePlot to take it
      std::pair<int,int> absIndex = aPoint.getIndex();
      absIndex.first += indexRange.first;
      absIndex.second += indexRange.second;

      pointFields_.first.at( absIndex.first ).at( absIndex.second ) = aPoint; // uuaargh..

    }

  }


  /////////////////////////////////
  /////////////////////////////////
  /////////////////////////////////
  // CLONE FOR THE MOMENT

  pointFields_.second = pointFields_.first; ////////////////////////////////////////////


}

