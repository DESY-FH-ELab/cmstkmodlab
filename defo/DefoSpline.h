
#ifndef _DEFOSPLINE_H
#define _DEFOSPLINE_H

#include <vector>
#include <utility>
#include <cmath>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/base_object.hpp>

#include "DefoPoint.h"


///
/// class for holding a single spline
/// of the form: y = ax^2 + bx + c
///
class DefoSpline {

  friend class boost::serialization::access;

 public:
  DefoSpline() {}
  DefoSpline( double const& a, double const& b, double const& c ) { a_ = a; b_ = b; c_ = c; }
  void setParameters( double const& a, double const& b, double const& c ) { a_ = a; b_ = b; c_ = c; }
  void setA( double const& a ) { a_ = a; }
  void setB( double const& b ) { b_ = b; }
  void setC( double const& c ) { c_ = c; }
  void setValidityRange( double const& from, double const& to ) { validityRange_ = std::make_pair<double,double>( from, to ); }
  std::pair<double,double> const& getValidityRange( void ) const { return validityRange_; }
  bool isInRange( double pos ) const { return( pos >= validityRange_.first && pos <= validityRange_.second ); }
  double const& a() const { return a_; }
  double const& b() const { return b_; }
  double const& c() const { return c_; }
  double eval( double pos ) const { return ( a_ * pos * pos + b_ * pos + c_ ); }

 private:
  double a_, b_, c_;
  std::pair<double,double> validityRange_;

  template<class Archive>
  void serialize( Archive & ar, const unsigned int version ) { 
    ar & a_; 
    ar & b_;
    ar & c_;
    ar & validityRange_;
    if( version ) {}; // empty
  }

};

BOOST_CLASS_VERSION( DefoSpline, 0 )



///
/// base class for a spline set
/// (one row or column of points/splines)
///
class DefoSplineSetBase {

  friend class boost::serialization::access;

 public:
  DefoSplineSetBase() {}
  virtual ~DefoSplineSetBase() {}
  virtual bool doFitXY( void ) = 0;
  virtual bool doFitZ( void ) = 0;
  std::vector<DefoSpline> const& getSplines( void ) const { return splines_; }
  DefoPointCollection const& getPoints( void ) const { return points_; }
  void addPoint( DefoPoint const& point ) { points_.push_back( point ); }
  void clear( void ) { splines_.resize( 0 ); points_.resize( 0 ); }
  void offset( double );
  double eval( double ) const;

 protected:
  std::vector<DefoSpline> splines_;
  DefoPointCollection points_;

 private:
  template<class Archive>
  void serialize( Archive & ar, const unsigned int version ) { 
    ar & splines_;
    ar & points_;
    if( version ) {}; // empty
  }

};

BOOST_CLASS_VERSION( DefoSplineSetBase, 0 )



///
/// specialization for splines along X
///
class DefoSplineSetX : public DefoSplineSetBase {

  friend class boost::serialization::access;

 public:
  DefoSplineSetX() {}
  virtual ~DefoSplineSetX() { clear(); }
  virtual bool doFitXY( void );
  virtual bool doFitZ( void );
  
 private:
  template<class Archive>
  void serialize( Archive & ar, const unsigned int version ) { 
    ar & boost::serialization::base_object<DefoSplineSetBase>( *this );
    if( version ) {}; // empty
  }

};

BOOST_CLASS_VERSION( DefoSplineSetX, 0 )



///
/// specialization for splines along Y
///
class DefoSplineSetY : public DefoSplineSetBase {

  friend class boost::serialization::access;

 public:
  DefoSplineSetY() {}
  virtual ~DefoSplineSetY() { clear(); }
  virtual bool doFitXY( void );
  virtual bool doFitZ( void );

 private:
  template<class Archive>
  void serialize( Archive & ar, const unsigned int version ) { 
    ar & boost::serialization::base_object<DefoSplineSetBase>( *this );
    if( version ) {}; // empty
  }

};

BOOST_CLASS_VERSION( DefoSplineSetY, 0 )



// containers with splinesets
typedef std::vector<DefoSplineSetX> DefoSplineSetXCollection;
typedef std::vector<DefoSplineSetY> DefoSplineSetYCollection;
typedef std::pair<DefoSplineSetXCollection,DefoSplineSetYCollection> DefoSplineField;

// helper definitions
typedef std::pair<std::vector<double>,std::vector<double> > DefoSplineFieldOffsets;
typedef std::pair<std::vector<unsigned int>,std::vector<unsigned int> > DefoSplineFieldCounters;

// operations on containers
//void DefoAddSplineFieldNormalization( DefoSplineField const&, DefoSplineField const& );


#endif
