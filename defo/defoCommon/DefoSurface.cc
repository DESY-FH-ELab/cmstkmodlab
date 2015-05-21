
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <QHash>

#include <nqlogger.h>

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
DefoSurface::DefoSurface()
{
  isSplineField_ = false;
}

DefoSurface::DefoSurface(const DefoSurface& other)
{
  points_ = other.points_;
  splineField_ = other.splineField_;
  pointFields_ = other.pointFields_;

  isSplineField_ = other.isSplineField_;
  isPoints_ = other.isPoints_;

  summary_ = other.summary_;
}

void DefoSurface::makeSummary()
{
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
        key.x_ = itP->getCalibratedX();
        key.iy_ = itP->getIndex().second;
        key.y_ = itP->getCalibratedY();

        value.setX(itC->eval(itP->getCalibratedX()));

        it_t it = defoPointMap_.find(key);
        if (it==defoPointMap_.end()) {
          value.y_ = 0;
          value.hasy_ = false;
          defoPointMap_.insert(key, value);
        } else {
          it.value().setX(itC->eval(itP->getCalibratedX()));
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
        key.x_ = itP->getCalibratedX();
        key.iy_ = itP->getIndex().second;
        key.y_ = itP->getCalibratedY();

        value.setY(itC->eval(itP->getCalibratedY()));

        it_t it = defoPointMap_.find(key);
        if (it==defoPointMap_.end()) {
          value.x_ = 0;
          value.hasx_ = false;
          defoPointMap_.insert(key, value);
        } else {
          it.value().setY(itC->eval(itP->getCalibratedY()));
        }

      }
    }

    summary_.maxZFromXSplines = -1e9;
    summary_.minZFromXSplines = +1e9;

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

            double z = itC->eval(itP->getCalibratedX());
            if (z>summary_.maxZFromXSplines) {
                summary_.maxZFromXSplines = z;
                summary_.posAtMaxZFromXSplines.first = itP->getCalibratedX();
                summary_.posAtMaxZFromXSplines.second = itP->getCalibratedY();
            }
            if (z<summary_.minZFromXSplines) {
                summary_.minZFromXSplines = z;
                summary_.posAtMinZFromXSplines.first = itP->getCalibratedX();
                summary_.posAtMinZFromXSplines.second = itP->getCalibratedY();
            }
        }
    }

    summary_.maxZFromYSplines = -1e9;
    summary_.minZFromYSplines = +1e9;

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

            double z = itC->eval(itP->getCalibratedY());
            if (z>summary_.maxZFromYSplines) {
                summary_.maxZFromYSplines = z;
                summary_.posAtMaxZFromYSplines.first = itP->getCalibratedX();
                summary_.posAtMaxZFromYSplines.second = itP->getCalibratedY();
            }
            if (z<summary_.minZFromYSplines) {
                summary_.minZFromYSplines = z;
                summary_.posAtMinZFromYSplines.first = itP->getCalibratedX();
                summary_.posAtMinZFromYSplines.second = itP->getCalibratedY();
            }
        }
    }
}

const DefoSurfaceSummary& DefoSurface::getSummary() const
{
    return summary_;
}

///
///
///
void DefoSurface::dumpSplineField(std::string filename) const
{
  typedef QHash<DefoSplineXYPair,DefoSplineXYDefoPair>::const_iterator it_t;

  if (filename.length()==0) return;

  std::ofstream ofile(filename.c_str());
  for (it_t it = defoPointMap_.begin();it!=defoPointMap_.end();++it) {
    ofile << std::setw(8)  << it.key().ix_ << " "
          << std::setw(8)  << it.key().iy_ << " "
          << std::setw(14) << std::scientific << it.key().x_ << " "
          << std::setw(14) << std::scientific << it.key().y_ << " "
          << std::setw(14) << std::scientific << it.value().x_ << " "
          << std::setw(3)  << (int)it.value().hasx_ << " "
          << std::setw(14) << std::scientific << it.value().y_ << " "
          << std::setw(3)  << (int)it.value().hasy_
          << std::endl;
  }
}

///
///
///
void DefoSurface::dumpSpline1DFieldX(std::string filename, double dx)
{
  typedef QHash<DefoSplineXYPair,DefoSplineXYDefoPair>::const_iterator it_t;

  if (filename.length()==0) return;

  int ixmin = defoPointMap_.begin().key().ix_;
  int ixmax = ixmin;
  int iymin = defoPointMap_.begin().key().iy_;
  int iymax = iymin;

  for (it_t it = defoPointMap_.begin();it!=defoPointMap_.end();++it) {
    ixmin = std::min(ixmin, it.key().ix_);
    ixmax = std::max(ixmax, it.key().ix_);
    iymin = std::min(iymin, it.key().iy_);
    iymax = std::max(iymax, it.key().iy_);
  }

  double xmin = -1e9;
  double xmax = 1e9;
  double ymin = -1e9;
  double ymax = 1e9;

  for (it_t it = defoPointMap_.begin();it!=defoPointMap_.end();++it) {
    if (it.key().ix_==ixmin) xmin = std::max(xmin, it.key().x_);
    if (it.key().ix_==ixmax) xmax = std::min(xmax, it.key().x_);
    if (it.key().iy_==iymin) ymin = std::max(ymin, it.key().y_);
    if (it.key().iy_==iymax) ymax = std::min(ymax, it.key().y_);
  }

  int stepsX = std::ceil((xmax-xmin)/dx);

  double theDX = (xmax-xmin)/stepsX;

  const DefoSplineSetXCollection& splineSets = splineField_.first;

  for (auto splineSet : splineSets) {

    for (auto point : splineSet.getPoints()) {

    }

    break;
  }

  /*
  typedef QHash<DefoSplineXYPair,DefoSplineXYDefoPair>::const_iterator it_t;

  if (filename.length()==0) return;


  std::vector<double> x, y, zx, zy;
  std::vector<int> vix, viy;

  int ix = 0, iy = 0;
  for (int rx=xmin;rx<=xmax;rx+=theDX) {
    iy = 0;
    for (int ry=ymin;ry<=ymax;ry+=theDY) {
      vix.push_back(ix);
      viy.push_back(iy);
      x.push_back(rx);
      y.push_back(ry);
      zx.push_back(0.0);
      zy.push_back(0.0);
      iy++;
    }
    ix++;
  }

  spline2Dx_.evaluate(x, y, zx);
  spline2Dy_.evaluate(x, y, zy);

  std::ofstream ofile(filename.c_str());
  std::vector<int>::iterator itix = vix.begin();
  std::vector<int>::iterator itiy = viy.begin();
  std::vector<double>::iterator itx = x.begin();
  std::vector<double>::iterator ity = y.begin();
  std::vector<double>::iterator itzx = zx.begin();
  std::vector<double>::iterator itzy = zy.begin();

  for (;itx!=x.end();++itix,++itiy,++itx,++ity,++itzx,++itzy) {
    ofile << std::setw(8)  << *itix << " "
          << std::setw(8)  << *itiy  << " "
      << std::setw(14) << std::scientific << *itx << " "
          << std::setw(14) << std::scientific << *ity << " "
          << std::setw(14) << std::scientific << *itzx << "   1 "
          << std::setw(14) << std::scientific << *itzy << "   1"
          << std::endl;
  }
  */
}

///
///
///
void DefoSurface::dumpSpline2DField(std::string filename,
                                    double dx, double dy)
{
  typedef QHash<DefoSplineXYPair,DefoSplineXYDefoPair>::const_iterator it_t;

  if (filename.length()==0) return;

  int ixmin = defoPointMap_.begin().key().ix_;
  int ixmax = ixmin;
  int iymin = defoPointMap_.begin().key().iy_;
  int iymax = iymin;
  
  for (it_t it = defoPointMap_.begin();it!=defoPointMap_.end();++it) {
    ixmin = std::min(ixmin, it.key().ix_);
    ixmax = std::max(ixmax, it.key().ix_);
    iymin = std::min(iymin, it.key().iy_);
    iymax = std::max(iymax, it.key().iy_);
  }

  double xmin = -1e9;
  double xmax = 1e9;
  double ymin = -1e9;
  double ymax = 1e9;
  
  for (it_t it = defoPointMap_.begin();it!=defoPointMap_.end();++it) {
    if (it.key().ix_==ixmin) xmin = std::max(xmin, it.key().x_);
    if (it.key().ix_==ixmax) xmax = std::min(xmax, it.key().x_);
    if (it.key().iy_==iymin) ymin = std::max(ymin, it.key().y_);
    if (it.key().iy_==iymax) ymax = std::min(ymax, it.key().y_);
  }
  
  int stepsX = std::ceil((xmax-xmin)/dx);
  int stepsY = std::ceil((ymax-ymin)/dy);

  double theDX = (xmax-xmin)/stepsX;
  double theDY = (ymax-ymin)/stepsY;

  std::vector<double> x, y, zx, zy;
  std::vector<int> vix, viy;

  int ix = 0, iy = 0;
  for (int rx=xmin;rx<=xmax;rx+=theDX) {
    iy = 0;
    for (int ry=ymin;ry<=ymax;ry+=theDY) {
      vix.push_back(ix);
      viy.push_back(iy);
      x.push_back(rx);
      y.push_back(ry);
      zx.push_back(0.0);
      zy.push_back(0.0);
      iy++;
    }
    ix++;
  }

  spline2Dx_.evaluate(x, y, zx);
  spline2Dy_.evaluate(x, y, zy);

  std::ofstream ofile(filename.c_str());
  std::vector<int>::iterator itix = vix.begin();
  std::vector<int>::iterator itiy = viy.begin();
  std::vector<double>::iterator itx = x.begin();
  std::vector<double>::iterator ity = y.begin();
  std::vector<double>::iterator itzx = zx.begin();
  std::vector<double>::iterator itzy = zy.begin();
  
  for (;itx!=x.end();++itix,++itiy,++itx,++ity,++itzx,++itzy) {
    ofile << std::setw(8)  << *itix << " "
          << std::setw(8)  << *itiy  << " "
	  << std::setw(14) << std::scientific << *itx << " "
          << std::setw(14) << std::scientific << *ity << " "
          << std::setw(14) << std::scientific << *itzx << "   1 "
          << std::setw(14) << std::scientific << *itzy << "   1"
          << std::endl;
  }
}

///
///
///
void DefoSurface::dumpSummary(std::string& filename) const
{
  if (filename.length()==0) return;

  std::ofstream ofile(filename.c_str());

  ofile << std::setw(8)  << summary_.maxZFromXSplines << " "
        << std::setw(8)  << summary_.posAtMaxZFromXSplines.first << " "
        << std::setw(8)  << summary_.posAtMaxZFromXSplines.second << std::endl;
  ofile << std::setw(8)  << summary_.minZFromXSplines << " "
        << std::setw(8)  << summary_.posAtMinZFromXSplines.first << " "
        << std::setw(8)  << summary_.posAtMinZFromXSplines.second << std::endl;

  ofile << std::setw(8)  << summary_.maxZFromYSplines << " "
        << std::setw(8)  << summary_.posAtMaxZFromYSplines.first << " "
        << std::setw(8)  << summary_.posAtMaxZFromYSplines.second << std::endl;
  ofile << std::setw(8)  << summary_.minZFromYSplines << " "
        << std::setw(8)  << summary_.posAtMinZFromYSplines.first << " "
        << std::setw(8)  << summary_.posAtMinZFromYSplines.second << std::endl;
}

///
/// create pair of DefoPointField,
/// one for x-splines, one for y-splines;
///
void DefoSurface::createPointFields( void ) {

  if( !isSplineField_ ) {
    NQLogCritical("DefoSurface::createPointFields()")
        << "Spline field not set. Doing nothing.";
    return;
  }

  // determine index range
  std::pair<unsigned int, unsigned int> indexRange( 0, 0 );
  for( DefoPointCollection::const_iterator it = points_.begin(); it != points_.end(); ++it ) {
    if( abs( it->getIndex().first  ) > (int)indexRange.first  ) indexRange.first  = abs( it->getIndex().first   );
    if( abs( it->getIndex().second ) > (int)indexRange.second ) indexRange.second = abs( it->getIndex().second  );

    if( !it->isIndexed() ) {
      NQLogWarning("DefoSurface::createPointFields()")
          << "Point not indexed at position: x: "
		  << it->getCalibratedX() << " y: " << it->getCalibratedY();
    }
  }


  // create matrix accordingly
  pointFields_.first.resize( indexRange.first * 2 + 1 );
  for( DefoPointField::iterator it = pointFields_.first.begin(); it != pointFields_.first.end(); ++it ) {
    it->resize( indexRange.second * 2 + 1 );
  }

  // first "along-x" splines
  DefoSplineSetXCollection const& splinesX = splineField_.first;
  
  // loop set of spline "rows", itC is a DefoSplineSetX
  for( DefoSplineSetXCollection::const_iterator itC = splinesX.begin(); itC < splinesX.end(); ++itC ) {
  
    // loop the points, determine matrix index
    std::vector<DefoPoint> aColumn;
    for( DefoPointCollection::const_iterator itP = itC->getPoints().begin(); itP < itC->getPoints().end(); ++itP ) {
      DefoPoint aPoint( *itP ); // make copy to preserve constness
      aPoint.setHeight( itC->eval( itP->getCalibratedX() ) );
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

void DefoSurface::calibrateZ(double calibZx, double calibZy)
{
  typedef QHash<DefoSplineXYPair,DefoSplineXYDefoPair>::iterator it_t;

  for (it_t it = defoPointMap_.begin();it!=defoPointMap_.end();++it) {
    if (it.value().hasx_ && it.value().hasy_) {
      it.value().x_ = it.value().x_ * calibZx;
      it.value().y_ = it.value().y_ * calibZy;
    }
  }
}

void DefoSurface::fitSpline2D(int kx, int ky, double s, double nxy)
{
  typedef QHash<DefoSplineXYPair,DefoSplineXYDefoPair>::const_iterator it_t;

  std::vector<double> x, y, zx, zy;

  for (it_t it = defoPointMap_.begin();it!=defoPointMap_.end();++it) {
    if (it.value().hasx_ && it.value().hasy_) {
      x.push_back(it.key().x_);
      y.push_back(it.key().y_);
      zx.push_back(it.value().x_);
      zy.push_back(it.value().y_);
    }
  }

  spline2Dx_.surfit(x, y, zx, kx, ky, s, nxy);
  spline2Dy_.surfit(x, y, zy, kx, ky, s, nxy);
}
