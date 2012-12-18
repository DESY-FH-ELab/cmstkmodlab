
#ifndef _DEFOSPLINE_H
#define _DEFOSPLINE_H

#include <vector>
#include <utility>
#include <cmath>

#include "DefoPoint.h"
#include "DefoConfigReader.h"

///
/// class for holding a single spline
/// of the form: y = ax^2 + bx + c
///
class DefoSpline {

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

};



///
/// base class for a spline set
/// (one row or column of points/splines)
///
class DefoSplineSetBase {

 public:
  DefoSplineSetBase( DefoPoint::Axis axis );
  virtual ~DefoSplineSetBase() {}
  virtual bool doFitXY( void );
  virtual bool doFitZ( void );
  std::vector<DefoSpline> const& getSplines( void ) const { return splines_; }
  DefoPointCollection const& getPoints( void ) const { return points_; }
  std::pair<double,double> const validityRange( void ) const;
  size_t getNPoints( void ) const { return points_.size(); }
  void addPoint( DefoPoint const& point ) { points_.push_back( point ); }
  void clear( void ) { splines_.resize( 0 ); points_.resize( 0 ); }
  void offset( double );
  double eval( double ) const;

 protected:
  DefoPoint::Axis axis_;
  std::vector<DefoSpline> splines_;
  DefoPointCollection points_;
  int debugLevel_;

};



///
/// specialization for splines along X
///
class DefoSplineSetX : public DefoSplineSetBase {

 public:
  DefoSplineSetX() : DefoSplineSetBase( DefoPoint::X ) {}
  virtual ~DefoSplineSetX() { clear(); }
//  virtual bool doFitXY( void );
//  virtual bool doFitZ( void );

};



///
/// specialization for splines along Y
///
class DefoSplineSetY : public DefoSplineSetBase {

 public:
  DefoSplineSetY() : DefoSplineSetBase( DefoPoint::Y ) {}
  virtual ~DefoSplineSetY() { clear(); }
//  virtual bool doFitXY( void );
//  virtual bool doFitZ( void );

};



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
