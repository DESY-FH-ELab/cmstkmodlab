
#include "DefoHistogramView.h"


///
///
///
DefoHistogramView::DefoHistogramView ( QWidget* p ) : QwtPlot( p ) {

  setWindowFlags( Qt::WindowStaysOnTopHint );
  init();

}



///
///
///
void DefoHistogramView::init( void ) {

  resize( 600, 400 );
  setCanvasBackground( Qt::black );

  redC_   = new QwtPlotCurve("red");
  greenC_ = new QwtPlotCurve("green");
  blueC_  = new QwtPlotCurve("blue");
  grayC_  = new QwtPlotCurve("gray");

  for( unsigned int i = 0; i < _PLOTSIZE; ++i ) {
    redX_[i] = i;   redY_[i] = 0.;
    greenX_[i] = i; greenY_[i] = 0.;
    blueX_[i] = i;  blueY_[i] = 0.;
    grayX_[i] = i;  grayY_[i] = 0.;
  }

  setTitle( "rgb histograms for first area" );

  setAxisTitle( QwtPlot::xBottom, "adc" );
  setAxisScale( QwtPlot::xBottom, 0, _PLOTSIZE+1 );
  
  setAxisTitle( QwtPlot::yLeft, "Pixels" );
  setAxisScale( QwtPlot::yLeft, .1, 100 );

  // set log axis
  setAxisScaleEngine( QwtPlot::yLeft, new QwtLog10ScaleEngine );

}



///
///
///
void DefoHistogramView::addPixel( QRgb const& rgb ) {



  unsigned int red = qRed( rgb );
  redY_[red] += 1.;

  unsigned int green = qGreen( rgb );
  greenY_[green] += 1.;

  unsigned int blue = qBlue( rgb );
  blueY_[blue] += 1.;

  unsigned int gray = qGray( rgb );
  grayY_[gray] += 1.;

}



///
///
///
void DefoHistogramView::scale( void ) {

  double max = 0;

  for( unsigned int i = 0; i <= _PLOTSIZE; ++i ) {
    if( redY_[i] > max )   max = redY_[i];
    if( greenY_[i] > max ) max = greenY_[i];
    if( blueY_[i] > max )  max = blueY_[i];
    if( grayY_[i] > max )  max = grayY_[i];
  }
    
  setAxisScale( QwtPlot::yLeft, 0.1, 10 * max );

}



///
///
///
void DefoHistogramView::clear( void ) {

  init(); // for the time being...

}



///
///
///
void DefoHistogramView::createCurves( void ) {

  // avoid inf spikes with log axis
  for( unsigned int i = 0; i < _PLOTSIZE; ++i ) {
    if( 1. > redY_[i] )   redY_[i] = .01;
    if( 1. > greenY_[i] ) greenY_[i] = .01;
    if( 1. > blueY_[i] )  blueY_[i] = .01;
    if( 1. > grayY_[i] )  grayY_[i] = .01;
  }

  redC_->attach(this);
  redC_->setPen(QPen(Qt::red));
  redC_->setRawData( redX_, redY_, _PLOTSIZE );

  greenC_->attach(this);
  greenC_->setPen(QPen(Qt::green));
  greenC_->setRawData( greenX_, greenY_, _PLOTSIZE );

  blueC_->attach(this);
  blueC_->setPen(QPen(Qt::blue));
  blueC_->setRawData( blueX_, blueY_, _PLOTSIZE );

  grayC_->attach(this);
  QPen grayPen;
  grayPen.setColor( Qt::white );
  grayPen.setStyle( Qt::DotLine );
  grayC_->setPen(grayPen);
  grayC_->setRawData( grayX_, grayY_, _PLOTSIZE );

  scale();

}
