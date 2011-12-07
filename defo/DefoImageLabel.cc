
#include "DefoImageLabel.h"



///
///
///
void DefoImageLabel::init( void ) {
  
  isRotation_ = 0.;
  isDefineArea_ = false;
  isDisplayAreas_ = false;

  // read parameters
  DefoConfigReader cfgReader( "defo.cfg" );
  debugLevel_ = cfgReader.getValue<unsigned int>( "DEBUG_LEVEL" );
  
}



///
///
///
void DefoImageLabel::displayImageToSize( QImage& originalImage ) {

  originalImageSize_ = originalImage.size();
  
  QSize mySize = size();
  if( isRotation_ ) mySize.transpose(); // if rotated, swap width,height

  QImage *transformedImage = new QImage( originalImage.scaled( mySize ) );

  // rotate if required
  if( isRotation_ ) {
    QTransform rotateTransform;
    rotateTransform.rotate( -90. );
    *transformedImage = transformedImage->transformed( rotateTransform );
  }

  setPixmap( QPixmap::fromImage( *transformedImage ) );

}



///
///
///
void DefoImageLabel::mousePressEvent( QMouseEvent *event ) {
  
  if( isDefineArea_ ) {
    myPoint_ = event->pos();
    rubberBand_ = new DefoImageLabelRubberBand(QRubberBand::Rectangle, this);
    rubberBand_->setGeometry( QRect( myPoint_, QSize() ) );
    rubberBand_->show();
  }

}
 

///
///
///
void DefoImageLabel::mouseMoveEvent( QMouseEvent *event ) {

  if( isDefineArea_ ) {
    rubberBand_->setGeometry( QRect( myPoint_, event->pos() ).normalized() ); //Area Bounding
  }

}



///
///
///
void DefoImageLabel::mouseReleaseEvent( QMouseEvent *event ) {

  Q_UNUSED(event);

  if( isDefineArea_ ) createAndSendArea();

}



///
///
///
void DefoImageLabel::defineArea( void ) {

  setCursor( Qt::CrossCursor );
  isDefineArea_ = true;

}



///
///
///
void DefoImageLabel::createAndSendArea( void ) {

  if( !pixmap() ) {
    std::cerr << " [DefoImageLabel::mouseReleaseEvent] ** ERROR: no pixmap loaded." << std::endl;
    throw;
  }
  


  // this is the rectangle in the displayed picture
  // which is scaled to the size of *this
  QRect rectangle( myPoint_.x(), 
		   myPoint_.y(), 
		   rubberBand_->size().width(),
		   rubberBand_->size().height() );

  if( debugLevel_ >= 3 ) std::cout << " [DefoImageLabel::createAndSendArea] =3= Created area: x: " 
				   << rectangle.x() << " y: " << rectangle.y()
				   << " w: " << rectangle.width() << " h: " << rectangle.height() 
				   << " (in local " << (isRotation_?"rotated ":"") << "coords)"
				   << std::endl << std::flush;

  transformToOriginal( rectangle );

  // send off the result
  DefoArea area( rectangle );
  emit( areaDefined( area ) );

  // displaying the rubberbands done by other function
  rubberBand_->hide();

  // exit area selection mode
  isDefineArea_ = false;
  setCursor( Qt::ArrowCursor );


  if( debugLevel_ >= 3 ) std::cout << " [DefoImageLabel::createAndSendArea] =3= Emitted area: x: " 
				   << rectangle.x() << " y: " << rectangle.y()
				   << " w: " << rectangle.width() << " h: " << rectangle.height() << " (in original coords)"
				   << std::endl << std::flush;
}



///
///
///
void DefoImageLabel::displayAreas( bool isDisplay ) {

  isDisplayAreas_ = isDisplay;

  for( std::vector<DefoImageLabelRubberBand*>::iterator it = areaRubberBands_.begin(); it < areaRubberBands_.end(); ++it ) {
    isDisplayAreas_ ? (*it)->show() : (*it)->hide();
  }
      
}



///
///
///
void DefoImageLabel::refreshAreas( std::vector<DefoArea> areas ) {

  // reset the vector
  for( std::vector<DefoImageLabelRubberBand*>::iterator it = areaRubberBands_.begin(); it < areaRubberBands_.end(); ++it ) {
    (*it)->hide();
    delete( *it );
  }
  areaRubberBands_.resize( 0 );
  
  for( std::vector<DefoArea>::iterator it = areas.begin(); it < areas.end(); ++it ) {
    
    DefoImageLabelRubberBand* aRubberBand = new DefoImageLabelRubberBand( QRubberBand::Rectangle, this );
    QRect r = it->getRectangle();
    transformToLocal( r );
    aRubberBand->setGeometry( r );
    aRubberBand->setName( it->getName() );
    areaRubberBands_.push_back( aRubberBand );
    if( isDisplayAreas_ ) aRubberBand->show();
    
  }

}



///
/// transform a QRect
/// from the coordinate system of the displayed image
/// (which is scaled and eventually rotated)
/// to the coordinate system of the original raw image
/// that was passed through displayImageToSize()
///
void DefoImageLabel::transformToOriginal( QRect& rect ) {

  // get the scale factors from the original size of the image;
  // assume the image is scaled to the size of *this
  // and might be rotated -90deg

  double xScale = 0., yScale = 0.;

  if( isRotation_ ) {

    xScale = originalImageSize_.height() / static_cast<double>( width() );
    yScale = originalImageSize_.width() / static_cast<double>( height() );

    // the rotated upper-left and lower-right will be the current:
    QPoint lowerLeft( rect.x(), rect.y()+rect.height() );
    QPoint upperRight( rect.x()+rect.width(), rect.y() );

    QPoint newUpperLeft( yScale * ( height() - lowerLeft.y() ), xScale * lowerLeft.x() );
    QPoint newLowerRight( yScale * ( height() - upperRight.y() ), xScale * upperRight.x() );

    rect = QRect( newUpperLeft.x(), newUpperLeft.y(), (newLowerRight-newUpperLeft).x(), (newLowerRight - newUpperLeft).y() );
    
  }
  else {

    xScale = originalImageSize_.width() / static_cast<double>( width() );
    yScale = originalImageSize_.height() / static_cast<double>( height() );

    // no rotation, so scale the points directly
    QPoint upperLeft( xScale * rect.x(), yScale * rect.y() );
    QPoint lowerRight( xScale * ( rect.x() + rect.width() ), yScale * ( rect.y() + rect.height() ) );

    rect = QRect( upperLeft.x(), upperLeft.y(), (lowerRight-upperLeft).x(), (lowerRight - upperLeft).y() );

  }

}



///
/// transform a QRect
/// from the coordinate system of the original raw image
/// that was passed through displayImageToSize()
/// to the coordinate system of the displayed image
/// (which is scaled and eventually rotated)
///
void DefoImageLabel::transformToLocal( QRect& rect ) {

  // get the scale factors from the original size of the image;
  // assume the image is scaled to the size of *this
  // and might be rotated -90deg
  double xScale = 0., yScale = 0.;
  if( isRotation_ ) {

    xScale = height() / static_cast<double>( originalImageSize_.width() );
    yScale = width() / static_cast<double>( originalImageSize_.height() );

    // the rotated upper-left and lower right will be the current:
    QPoint upperRight( rect.x()+rect.width(), rect.y() );
    QPoint lowerLeft( rect.x(), rect.y()+rect.height() );

    QPoint newUpperLeft( yScale * upperRight.y(), xScale * ( originalImageSize_.width() - upperRight.x() ) );
    QPoint newLowerRight( yScale * lowerLeft.y(), xScale * ( originalImageSize_.width() - lowerLeft.x() ) );

    rect = QRect( newUpperLeft.x(), newUpperLeft.y(), (newLowerRight-newUpperLeft).x(), (newLowerRight - newUpperLeft).y() );

  }
  else {

    xScale = width() / static_cast<double>( originalImageSize_.width() );
    yScale = height() / static_cast<double>( originalImageSize_.height() );

    // no rotation, so scale the points directly
    QPoint upperLeft( xScale * rect.x(), yScale * rect.y() );
    QPoint lowerRight( xScale * ( rect.x() + rect.width() ), yScale * ( rect.y() + rect.height() ) );

    rect = QRect( upperLeft.x(), upperLeft.y(), (lowerRight-upperLeft).x(), (lowerRight - upperLeft).y() );

  }

}
