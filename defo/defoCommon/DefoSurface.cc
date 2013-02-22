
#include <iostream>
#include <fstream>
#include <iomanip>

#include <QHash>

#include "DefoSurface.h"

class DefoSplineXYPair
{
public:
  DefoSplineXYPair(const double& x = 0, const double& y = 0)
    :x_(x), y_(y) {}
  double x_, y_;
};

inline bool operator==(const DefoSplineXYPair &lhs, const DefoSplineXYPair &rhs)
{
  return lhs.x_==rhs.x_ && lhs.y_==rhs.y_;
}

inline uint qHash(const DefoSplineXYPair &key)
{
  return (qHash((uint)key.x_) ^ (qHash((uint)key.y_)<<1));
}

class DefoSplineXYDefoPair
{
public:
  DefoSplineXYDefoPair()
    :x_(0), y_(0), hasx_(false), hasy_(false) {}
  void setX(const double &x) { x_ = x; hasx_ = true; }
  void setY(const double &y) { y_ = y; hasy_ = true; }
  double x_, y_;
  bool hasx_, hasy_;
};

///
///
///
DefoSurface::DefoSurface() {

  isSplineField_ = false;
}

///
///
///
void DefoSurface::dumpSplineField( void ) const {

  QHash<DefoSplineXYPair,DefoSplineXYDefoPair> hmap;
  typedef QHash<DefoSplineXYPair,DefoSplineXYDefoPair>::iterator it_t;
  DefoSplineXYPair key;
  DefoSplineXYDefoPair value;

  // first "along-x" splines
  DefoSplineSetXCollection const& splinesX = splineField_.first;
  
  // loop set of spline "rows", itC is a DefoSplineSetX
  for( DefoSplineSetXCollection::const_iterator itC = splinesX.begin(); itC < splinesX.end(); ++itC ) {
    
    // loop the points
    for( DefoPointCollection::const_iterator itP = itC->getPoints().begin(); itP < itC->getPoints().end(); ++itP ) {
      
      std::cout << "HX: "
		<< std::setw( 14 ) << itP->getX() 
		<< std::setw( 14 ) << itP->getY() 
		<< std::setw( 14 ) << itC->eval( itP->getX() )
		<< std::endl;

      key.x_ = itP->getX();
      key.y_ = itP->getY();
      value.setX(itC->eval(itP->getX()));

      it_t it = hmap.find(key);
      if (it==hmap.end()) {
        value.y_ = 0;
        value.hasy_ = false;
        hmap.insert(key, value);
      } else {
        it.value().setX(itC->eval(itP->getX()));
      }
    }

  }

  // then "along-y" splines
  DefoSplineSetYCollection const& splinesY = splineField_.second;

  // loop set of spline "rows", itC is a DefoSplineSetX
  for( DefoSplineSetYCollection::const_iterator itC = splinesY.begin(); itC < splinesY.end(); ++itC ) {
    
    // loop the points
    for( DefoPointCollection::const_iterator itP = itC->getPoints().begin(); itP < itC->getPoints().end(); ++itP ) {
      
      std::cout << "HY: "
		<< std::setw( 14 ) << itP->getX() 
		<< std::setw( 14 ) << itP->getY() 
		<< std::setw( 14 ) << itC->eval( itP->getY() )
		<< std::endl;

      key.x_ = itP->getX();
      key.y_ = itP->getY();
      value.setY(itC->eval(itP->getY()));

      it_t it = hmap.find(key);
      if (it==hmap.end()) {
        value.x_ = 0;
        value.hasx_ = false;
        hmap.insert(key, value);
      } else {
        it.value().setY(itC->eval(itP->getY()));
      }

    }

  }

  std::ofstream ofile("defoDump.txt");
  for (it_t it = hmap.begin();it!=hmap.end();++it) {\
    ofile << it.key().x_ << "\t"
          << it.key().y_ << "\t"
          << it.value().x_ << "\t"
          << (int)it.value().hasx_ << "\t"
          << it.value().y_ << "\t"
          << (int)it.value().hasy_
          << std::endl;
  }
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

