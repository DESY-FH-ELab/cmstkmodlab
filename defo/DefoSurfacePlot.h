
#ifndef _DEFOSURFACEPLOT_H
#define _DEFOSURFACEPLOT_H

#include <QString>
#include <Qt>

#include <qwt3d_surfaceplot.h>
#include <qwt3d_surfaceplot.h>
#include <qwt3d_types.h>
#include <qwt3d_portability.h>

#include "DefoSurface.h"

// scaling for the z axis (m -> µm)
#define DEFORSURFACEPLOT_Z_SCALING 1000000.

///
/// a container/manager for the QwtPlot3D
/// reco surface output plot on the GUI
///
class DefoSurfacePlot : public Qwt3D::SurfacePlot {

  Q_OBJECT

 public:

  enum PointSet { POINTS_X, POINTS_Y };

  DefoSurfacePlot( QWidget* parent ) : Qwt3D::SurfacePlot( parent ) { init(); }
  ~DefoSurfacePlot() {}
  void setData( DefoSurface const& );
  void setDisplayTitle( QString const& title ) { title_ = title; }
  void draw( void );

 public slots:

  void toggleView( bool );
  void setIsShade( bool );
  void setIsMesh( bool );
  void setIsAxes( bool );
  void setIsLegend( bool );
  void setIsIsolines( bool );
  void setNIsolines( int );
  void increaseZScale( void );
  void decreaseZScale( void );
	
 signals:

  void shadeEnabled( bool );
  void meshEnabled( bool );
  void axesEnabled( bool );
  void legendEnabled( bool );
  void isolinesEnabled( bool );
  void nIsolines( int );

 private:
  
  void init( void );
  DefoPoint const& neighbour( std::pair<unsigned int, unsigned int> const&,  DefoPointField const& );
  void repaint( void );
  DefoSurfacePlot::PointSet pointSet_;
  std::pair<double,double> amplitudeRange_;
  bool isValid_;
  QString title_;
  double scalefactor_;
  double xyScale_;
  double zScale_;
  std::pair<bool,bool> meshAndShadeCarry_;
  
  bool is3D_;
  bool isMesh_;
  bool isAxes_;
  bool isShade_;
  bool isIsolines_;
  bool isLegend_;
  int nIsolines_;

};


#endif
