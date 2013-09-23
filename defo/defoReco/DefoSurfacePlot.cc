#include <vector>

#include "DefoSurfacePlot.h"

///
/// plot initialization
///
void DefoSurfacePlot::init( void )
{
    vs_ = &dvs_;

    // by default, we display the points from the x-splines
    pointSet_ = DefoSurfacePlot::POINTS_X;

    // valid data? default is false until setData is called
    isValid_ = false;
}

///
/// a wrapper for show()
///
void DefoSurfacePlot::draw( void )
{
    if( !isValid_ ) {
        std::cerr << " [DefoSurfacePlot::draw] ** WARNING: no data set" << std::endl;
        return;
    }

    std::vector<Qwt3D::AXIS> Xaxis;
    Xaxis.push_back(Qwt3D::X1);
    Xaxis.push_back(Qwt3D::X2);
    Xaxis.push_back(Qwt3D::X3);
    Xaxis.push_back(Qwt3D::X4);

    std::vector<Qwt3D::AXIS> Yaxis;
    Yaxis.push_back(Qwt3D::Y1);
    Yaxis.push_back(Qwt3D::Y2);
    Yaxis.push_back(Qwt3D::Y3);
    Yaxis.push_back(Qwt3D::Y4);

    std::vector<Qwt3D::AXIS> Zaxis;
    Zaxis.push_back(Qwt3D::Z1);
    Zaxis.push_back(Qwt3D::Z2);
    Zaxis.push_back(Qwt3D::Z3);
    Zaxis.push_back(Qwt3D::Z4);

    setTitle( title_ );

    // scaling & display geom

    // for the scaling we need the z amplitude (in micron)
    const double amplitude = ( amplitudeRange_.second - amplitudeRange_.first ); // this amplitude is already in µm

//    zScale_ = 1e6; // starting value (experimental..)
//    xyScale_ = 15000.; // experimental..

//    scalefactor_ = amplitude / 1500. * ( xyScale_ / DEFORSURFACEPLOT_XY_SCALING ); // experimental..
//    //  setScale( scalefactor_, scalefactor_, zScale_ / DEFORSURFACEPLOT_Z_SCALING ); // z-axis is in µm, all others in m

//    zScale_ = 1.5;
//    xyScale_ = 0.9;
//    scalefactor_ = zScale_;

    // currently not to be changed
    setBackgroundColor(Qwt3D::RGBA(1,1,1));

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

    for (std::vector<Qwt3D::AXIS>::const_iterator it = Zaxis.begin();
         it!=Zaxis.end();
         ++it) {
        //coordinates()->axes[a].limits(zAxisRange.first, zAxisRange.second);
        coordinates()->axes[*it].setMajors(10);
        coordinates()->axes[*it].setMinors(5);
        coordinates()->axes[*it].setLabelString(QString( "Z [" ) + QChar(0x00B5) + QString( "m]" ));
        coordinates()->axes[*it].setLabel(true);
    }

    coordinates()->axes[Qwt3D::X1].setLabelString( "X [mm]" );
    coordinates()->axes[Qwt3D::X2].setLabelString( "X [mm]" );
    coordinates()->axes[Qwt3D::X3].setLabelString( "X [mm]" );
    coordinates()->axes[Qwt3D::X4].setLabelString( "X [mm]" );

    coordinates()->axes[Qwt3D::Y1].setLabelString( "Y [mm]" );
    coordinates()->axes[Qwt3D::Y2].setLabelString( "Y [mm]" );
    coordinates()->axes[Qwt3D::Y3].setLabelString( "Y [mm]" );
    coordinates()->axes[Qwt3D::Y4].setLabelString( "Y [mm]" );

    coordinates()->axes[Qwt3D::Z1].setLabelString( QString( "Z [" ) + QChar(0x00B5) + QString( "m]" ) ); // QChar makes the µ
    coordinates()->axes[Qwt3D::Z2].setLabelString( QString( "Z [" ) + QChar(0x00B5) + QString( "m]" ) );
    coordinates()->axes[Qwt3D::Z3].setLabelString( QString( "Z [" ) + QChar(0x00B5) + QString( "m]" ) );
    coordinates()->axes[Qwt3D::Z4].setLabelString( QString( "Z [" ) + QChar(0x00B5) + QString( "m]" ) );

    makeCurrent(); // QGLWidget::makeCurrent
    updateData();
    updateGL();

    // sync the buttons & spin box
    emit meshEnabled( vs_->isMesh_ );
    emit axesEnabled( vs_->isAxes_ );
    emit shadeEnabled( vs_->isShade_ );
    emit legendEnabled( vs_->isLegend_ );
    emit isolinesEnabled( vs_->isIsolines_ );
    emit nIsolines( vs_->nIsolines_ );

    show();
    toggleView( true ); // true = 3D
}

///
/// import data from a DefoSurface object
///
void DefoSurfacePlot::setData(DefoSurface const& surface)
{
    // for the max defo amplitude in µm
    amplitudeRange_ = std::pair<double,double>( 0., 0. );

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
    indexRange.second = field.at(0).size();
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
                data[i][j] = Qwt3D::Triple(field[i][j].getX() * DEFORSURFACEPLOT_XY_SCALING,
                                           field[i][j].getY() * DEFORSURFACEPLOT_XY_SCALING,
                                           field[i][j].getHeight() * DEFORSURFACEPLOT_Z_SCALING /* to µm*/ );
            }
            else { // use valid neighbour
                DefoPoint const& aNeighbour = neighbour( std::pair<unsigned int, unsigned int>( i, j ), field );
                data[i][j] = Qwt3D::Triple( aNeighbour.getX() * DEFORSURFACEPLOT_XY_SCALING,
                                            aNeighbour.getY() * DEFORSURFACEPLOT_XY_SCALING,
                                            aNeighbour.getHeight() * DEFORSURFACEPLOT_Z_SCALING /* to µm*/ );
            }

            if( data[i][j].z < amplitudeRange_.first )  amplitudeRange_.first  = data[i][j].z;
            if( data[i][j].z > amplitudeRange_.second ) amplitudeRange_.second = data[i][j].z;
        }
    }

    loadFromData(data, indexRange.first, indexRange.second);

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
DefoPoint const& DefoSurfacePlot::neighbour( std::pair<unsigned int, unsigned int> const& matrixPos, DefoPointField const& field )
{
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
void DefoSurfacePlot::toggleView( bool is3D )
{
    // save 3d settings when we switch from 3d to 2d
    // and restore them when switching back;
    // this is neccessary since 2d mode toggles some buttons automatically

    if( !is3D && vs_->is3D_ ) { // switching from 3d to 2d

        meshAndShadeCarry_ = std::pair<bool,bool>( vs_->isMesh_, vs_->isShade_ );

    } else if( is3D && !vs_->is3D_ ) { // switching from 2d to 3d

        vs_->isMesh_ = meshAndShadeCarry_.first;
        vs_->isShade_ = meshAndShadeCarry_.second;
    }

    vs_->is3D_ = is3D;

    if( vs_->is3D_ ) {
        setRotation(vs_->xRot_, vs_->yRot_, vs_->zRot_);
    } else {
        setRotation( 90.1, 0., 180. );
    }

    repaint();
}

///
///
///
void DefoSurfacePlot::setIsShade( bool isShade )
{
    vs_->isShade_ = isShade;
    repaint();
};

///
///
///
void DefoSurfacePlot::setIsMesh( bool isMesh )
{
    vs_->isMesh_ = isMesh;
    repaint();
}

///
///
///
void DefoSurfacePlot::setIsAxes( bool isAxes )
{
    vs_->isAxes_ = isAxes;
    repaint();
}

///
///
///
void DefoSurfacePlot::setIsLegend( bool isLegend )
{
    vs_->isLegend_ = isLegend;
    repaint();
}

///
///
///
void DefoSurfacePlot::setIsIsolines( bool isIsolines )
{
    vs_->isIsolines_ = isIsolines;
    repaint();
}

///
///
///
void DefoSurfacePlot::setNIsolines(int nIsolines)
{
    // avoid signal bouncing
    if( nIsolines != vs_->nIsolines_ ) {
        vs_->nIsolines_ = nIsolines;
        repaint();
    }
}

///
/// increase length of z axis by 5%,
/// keep axis limits
///
void DefoSurfacePlot::increaseZScale( void )
{
    vs_->zScale_ *= 1.05;
    repaint();
}

///
/// decrease length of z axis by 5%,
/// keep axis limits
///
void DefoSurfacePlot::decreaseZScale( void )
{
    vs_->zScale_ *= 0.95;
    repaint();
}

///
/// common repainting procedure
/// for 2D and 3D modes
///
void DefoSurfacePlot::repaint( void ) {


    if( vs_->is3D_ ) { // for 3D mode

        // determine plot style
        if( vs_->isMesh_ ) {
            if( vs_->isShade_ ) {
                setPlotStyle( Qwt3D::FILLEDMESH );
            } else {
                setPlotStyle( Qwt3D::WIREFRAME );
            }
        } else { // no mesh
            if( vs_->isShade_ ) {
                setPlotStyle( Qwt3D::FILLED );
            } else {
                setPlotStyle( Qwt3D::NOPLOT );
            }
        }

        // isolines
        if ( vs_->isIsolines_ ) setFloorStyle( Qwt3D::FLOORISO );
        else setFloorStyle( Qwt3D::NOFLOOR );

        //setScale( scalefactor_, scalefactor_, zScale_ / DEFORSURFACEPLOT_Z_SCALING );
        setScale(vs_->xyScale_, vs_->xyScale_, vs_->zScale_);

        emit shadeEnabled( vs_->isShade_ );
        emit meshEnabled( vs_->isMesh_ );

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

        if ( vs_->isIsolines_ ) {
            setFloorStyle( Qwt3D::FLOORISO );
            emit shadeEnabled( false );
        } else {
            setFloorStyle( Qwt3D::FLOORDATA );
            emit shadeEnabled( true );
        }

        emit meshEnabled( false );
        setPlotStyle(Qwt3D::NOPLOT);
        setScale(vs_->xyScale_, vs_->xyScale_, 0.); // flat in z

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

    if ( vs_->isAxes_ ) setCoordinateStyle( Qwt3D::BOX );
    else setCoordinateStyle( Qwt3D::NOCOORD );

    setIsolines( vs_->nIsolines_ );

    showColorLegend( vs_->isLegend_ );

    // redo graphics
    updateData();
    updateGL();
}

void DefoSurfacePlot::setViewSettings(DefoSurfacePlotViewSettings* vs)
{
    if (vs_) {
        vs_->xRot_ = xRotation();
        vs_->yRot_ = yRotation();
        vs_->zRot_ = zRotation();
    }

    vs_ = vs;

    if( vs_->is3D_ ) {
        setRotation(vs_->xRot_, vs_->yRot_, vs_->zRot_);
    } else {
        setRotation( 90.1, 0., 180. );
    }
}
