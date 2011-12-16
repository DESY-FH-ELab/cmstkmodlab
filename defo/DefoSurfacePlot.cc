
#include "DefoSurfacePlot.h"


///
/// plot initialization
///
void DefoSurfacePlot::init( void ) {
  
  // by default, we display the points from the x-splines
  pointSet_ = DefoSurfacePlot::POINTS_X;

  // valid data? default is false until setData is called
  isValid_ = false;
  
  scalefactor_ = 0.;

  // default values
  is3D_ = true;
  isMesh_ = true;
  isAxes_ = true;
  isShade_ = true;
  isLegend_ = true;
  isIsolines_ = false;
  nIsolines_ = 10;

}



///
/// a wrapper for show()
///
void DefoSurfacePlot::draw( void ) {

  if( !isValid_ ) {
    std::cerr << " [DefoSurfacePlot::draw] ** WARNING: no data set" << std::endl;
    return;
  }
  
  
  setTitle( title_ );

  // scaling & display geom
  
  // for the scaling we need the z amplitude (in micron)
  const double amplitude = ( amplitudeRange_.second - amplitudeRange_.first ); // this amplitude is already in µm

  zScale_ = 700. * 400.; // starting value (experimental..)
  xyScale_ = 1600.; // experimental..

  scalefactor_ = amplitude / 160. * ( xyScale_ / DEFORSURFACEPLOT_XY_SCALING ); // experimental..
  //  setScale( scalefactor_, scalefactor_, zScale_ / DEFORSURFACEPLOT_Z_SCALING ); // z-axis is in µm, all others in m

  // currently not to be changed
  setBackgroundColor(Qwt3D::RGBA(1,1,1));


//   // tick marks
//   for (unsigned i=0; i!=coordinates()->axes.size(); ++i) {
//     coordinates()->axes[i].setMajors(7);
//     coordinates()->axes[i].setMinors(4);
//     coordinates()->axes[i].setTicLength( 0.001 * DEFORSURFACEPLOT_XY_SCALING, 0.0005 * DEFORSURFACEPLOT_XY_SCALING );
//   }
  

  // axes
  std::pair<double,double> zAxisRange; // min,max
  coordinates()->axes[2].limits( zAxisRange.first, zAxisRange.second );
  legend()->setLimits( zAxisRange.first, zAxisRange.second );
  legend()->setTitleString( QString( "Z / " ) + QChar(0x00B5) + QString( "m" ) );

  for (unsigned i=0; i!= coordinates()->axes.size(); ++i) {
    coordinates()->axes[i].setTicLength( 0.002 * DEFORSURFACEPLOT_XY_SCALING, 0.001 * DEFORSURFACEPLOT_XY_SCALING );
    coordinates()->axes[i].setMajors(10);
    coordinates()->axes[i].setMinors(5);
  }

  coordinates()->axes[Qwt3D::X1].setLabelString( "X / cm" );
  coordinates()->axes[Qwt3D::X2].setLabelString( "X / cm" );
  coordinates()->axes[Qwt3D::X3].setLabelString( "X / cm" );
  coordinates()->axes[Qwt3D::X4].setLabelString( "X / cm" );

  coordinates()->axes[Qwt3D::Y1].setLabelString( "Y / cm" );
  coordinates()->axes[Qwt3D::Y2].setLabelString( "Y / cm" );
  coordinates()->axes[Qwt3D::Y3].setLabelString( "Y / cm" );
  coordinates()->axes[Qwt3D::Y4].setLabelString( "Y / cm" );

  coordinates()->axes[Qwt3D::Z1].setLabelString( QString( "Z / " ) + QChar(0x00B5) + QString( "m" ) ); // QChar makes the µ
  coordinates()->axes[Qwt3D::Z2].setLabelString( QString( "Z / " ) + QChar(0x00B5) + QString( "m" ) );
  coordinates()->axes[Qwt3D::Z3].setLabelString( QString( "Z / " ) + QChar(0x00B5) + QString( "m" ) );
  coordinates()->axes[Qwt3D::Z4].setLabelString( QString( "Z / " ) + QChar(0x00B5) + QString( "m" ) );

  makeCurrent(); // QGLWidget::makeCurrent
  updateData();
  updateGL();

  // sync the buttons & spin box
  emit meshEnabled( isMesh_ );
  emit axesEnabled( isAxes_ );
  emit shadeEnabled( isShade_ );
  emit legendEnabled( isLegend_ );
  emit isolinesEnabled( isIsolines_ );
  emit nIsolines( nIsolines_ );

  show();
  toggleView( true ); // true = 3D

}



///
/// import data from a DefoSurface object
///
void DefoSurfacePlot::setData( DefoSurface const& surface ) {

  // for the max defo amplitude in µm
  amplitudeRange_ = std::make_pair<double,double>( 0., 0. );

  // set x or y field, according to what has been chosen
  DefoPointField const& field = POINTS_X==pointSet_ ? surface.getPointFields().first : surface.getPointFields().second;

  // determine its dimensions
  std::pair<unsigned int, unsigned int> indexRange;

  indexRange.first = field.size();
  if( 0 == indexRange.first ) {
    std::cerr << " [DefoSurfacePlot::setData] ** WARNING: received empty point field" << std::endl;
    isValid_ = false;
  }

  // look at first column for y size
  indexRange.second = field.at( 0 ).size();
  if( 0 == indexRange.second ) {
    std::cerr << " [DefoSurfacePlot::setData] ** WARNING: point field has zero column dimension" << std::endl;
    isValid_ = false;
  }
  
  // Qwt3D::SurfacePlot wants an array:
  // one entry for each point, holding a (x,y,z) tuple
  Qwt3D::Triple **data = new Qwt3D::Triple *[indexRange.first] ;

  for( unsigned int i = 0; i < indexRange.first; ++i ) {
    data[i] = new Qwt3D::Triple[indexRange.second] ;
    for( unsigned int j = 0; j < field[i].size(); ++j ) { // the rest of the column should be zero!!

      if(field[i][j].isValid() ) {
	data[i][j] = Qwt3D::Triple( field[i][j].getX() * DEFORSURFACEPLOT_XY_SCALING, 
				    field[i][j].getY() * DEFORSURFACEPLOT_XY_SCALING,
				    field[i][j].getHeight() * DEFORSURFACEPLOT_Z_SCALING /* to µm*/ );
      }
      else { // use valid neighbour
	DefoPoint const& aNeighbour = neighbour( std::make_pair<unsigned int, unsigned int>( i, j ), field );
	data[i][j] = Qwt3D::Triple( aNeighbour.getX() * DEFORSURFACEPLOT_XY_SCALING, 
				    aNeighbour.getY() * DEFORSURFACEPLOT_XY_SCALING, 
				    aNeighbour.getHeight() * DEFORSURFACEPLOT_Z_SCALING /* to µm*/ );
      }
      
      if( data[i][j].z < amplitudeRange_.first )  amplitudeRange_.first  = data[i][j].z;
      if( data[i][j].z > amplitudeRange_.second ) amplitudeRange_.second = data[i][j].z;

    }
  }  

  // import data in mother
  loadFromData( data, indexRange.first, indexRange.second );
  
  isValid_ = true;

}


///
/// find a suitable neighbour for aPoint
/// from which to take coordinates for display
/// when aPoint is invalid (i.e. contains no height data)
///
/// all this fiddling with neighbours is due to the fact
/// that stupid Qwt3D::SurfacePlot needs a square array
/// as data input and allows no gaps or invalid points,
/// so we have to clone an invalid point from its nearest neighbour
/// in which case it becomes "invisible"
///
DefoPoint const& DefoSurfacePlot::neighbour( std::pair<unsigned int, unsigned int> const& matrixPos, DefoPointField const& field ) {

  // determine index range
  std::pair<int, int> indexRange;
  indexRange.first = field.size();
  if( 0 != field.size() ) indexRange.second = field.at( 0 ).size();

  // widen the search "circle"
  for( int offset = 1; offset < std::max( indexRange.first, indexRange.second ); ++offset ) {

    for( int i = (int)matrixPos.first - offset; i < (int)matrixPos.first + offset; ++i ) {
      for( int j = (int)matrixPos.second - offset; j < (int)matrixPos.second + offset; ++j ) {
	if( i >= 0 && i < indexRange.first && j >= 0 && j < indexRange.second ) {
	  if( field[i][j].isValid() ) {
	    return field[i][j];
	  }
	}
      }
    }

  }

  // failure
  std::cerr << " [DefoSurfacePlot::neighbour] ** WARNING: no valid neighbour found for pos: " 
	    << matrixPos.first << ", " << matrixPos.second << std::endl;

  return field[matrixPos.first][matrixPos.second];

}



///
///
///
void DefoSurfacePlot::toggleView( bool is3D ) {


  // save 3d settings when we switch from 3d to 2d
  // and restore them when switching back;
  // this is neccessary since 2d mode toggles some buttons automatically

  if( !is3D && is3D_ ) { // switching from 3d to 2d
    meshAndShadeCarry_ = std::make_pair<bool,bool>( isMesh_, isShade_ );
  }
  else if( is3D && !is3D_ ) { // switching from 2d to 3d
    isMesh_ = meshAndShadeCarry_.first;
    isShade_ = meshAndShadeCarry_.second;
  }

  is3D_ = is3D;

  if( is3D_ ) {
    setRotation( 20., 0., 45. );
  }
  else {
    setRotation( 90.1, 0., 180. );
  }

  repaint();

}



///
///
///
void DefoSurfacePlot::setIsShade( bool isShade ) {

  isShade_ = isShade; 
  repaint();

};



///
///
///
void DefoSurfacePlot::setIsMesh( bool isMesh ) { 

  isMesh_ = isMesh; 
  repaint();

}



///
///
///
void DefoSurfacePlot::setIsAxes( bool isAxes ) { 

  isAxes_ = isAxes;
  repaint();

}



///
///
///
void DefoSurfacePlot::setIsLegend( bool isLegend ) { 

  isLegend_ = isLegend; 
  repaint();

}



///
///
///
void DefoSurfacePlot::setIsIsolines( bool isIsolines ) {

  isIsolines_ = isIsolines;
  repaint();

}



///
///
///
void DefoSurfacePlot::setNIsolines( int nIsolines ) {

  // avoid signal bouncing
  if( nIsolines != nIsolines_ ) {
    nIsolines_ = nIsolines; 
    repaint();
  }

}



///
/// increase length of z axis by 5%,
/// keep axis limits
///
void DefoSurfacePlot::increaseZScale( void ) {

  zScale_ *= 1.05;
  repaint();
  
}



///
/// decrease length of z axis by 5%,
/// keep axis limits
///
void DefoSurfacePlot::decreaseZScale( void ) {

  zScale_ *= 0.95;
  repaint();

}



///
/// common repainting procedure
/// for 2D and 3D modes
///
void DefoSurfacePlot::repaint( void ) {


  if( is3D_ ) { // for 3D mode

    // determine plot style
    if( isMesh_ ) {
      if( isShade_ ) {
	setPlotStyle( Qwt3D::FILLEDMESH );
      }
      else {
	setPlotStyle( Qwt3D::WIREFRAME );
      }
    }
    else { // no mesh
      if( isShade_ ) {
	setPlotStyle( Qwt3D::FILLED );
      }
      else {
	setPlotStyle( Qwt3D::NOPLOT );
      }
    }

    // isolines
    if( isIsolines_ ) setFloorStyle( Qwt3D::FLOORISO );
    else setFloorStyle( Qwt3D::NOFLOOR );

    setScale( scalefactor_, scalefactor_, zScale_ / DEFORSURFACEPLOT_Z_SCALING ); 

    emit shadeEnabled( isShade_ );
    emit meshEnabled( isMesh_ );

    assignMouse(
		 Qwt3D::MouseState( Qt::LeftButton ), // xrot
		 Qwt3D::MouseState( Qt::LeftButton ), // yrot
		 Qwt3D::MouseState( Qt::LeftButton ), // zrot
		 Qwt3D::MouseState( Qt::NoButton ), // x scale
		 Qwt3D::MouseState( Qt::NoButton ), // y scale
		 Qwt3D::MouseState( Qt::NoButton ), // z scale
		 Qwt3D::MouseState( Qt::MidButton ), // zoom
		 Qwt3D::MouseState( Qt::RightButton ), // shift x
		 Qwt3D::MouseState( Qt::RightButton ) // shift right
	       );
  }

  else { // 2D mode

    if( isIsolines_ ) {
      setFloorStyle( Qwt3D::FLOORISO );
      emit shadeEnabled( false );
    }
    else {
      setFloorStyle( Qwt3D::FLOORDATA );
      emit shadeEnabled( true );
    }

    emit meshEnabled( false );
    setPlotStyle(Qwt3D::NOPLOT);
    setScale( scalefactor_, scalefactor_, 0. ); // flat in z

    assignMouse( // limit mouse actions here
		 Qwt3D::MouseState( Qt::NoButton ), // xrot
		 Qwt3D::MouseState( Qt::NoButton ), // yrot
		 Qwt3D::MouseState( Qt::NoButton ), // zrot
		 Qwt3D::MouseState( Qt::NoButton ), // x scale
		 Qwt3D::MouseState( Qt::NoButton ), // y scale
		 Qwt3D::MouseState( Qt::NoButton ), // z scale
		 Qwt3D::MouseState( Qt::MidButton ), // zoom
		 Qwt3D::MouseState( Qt::RightButton ), // shift x
		 Qwt3D::MouseState( Qt::RightButton ) // shift right
	       );
  }

  // common for both modes

  if( isAxes_ ) setCoordinateStyle( Qwt3D::BOX );
  else setCoordinateStyle( Qwt3D::NOCOORD );

  setIsolines( nIsolines_ );

  showColorLegend( isLegend_ );

  // redo graphics
  updateData();
  updateGL(); 

}
