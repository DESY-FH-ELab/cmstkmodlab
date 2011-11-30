
#include "DefoImageLabel.h"



///
///
///
void DefoImageLabel::init( void ) {
  
  isRotation_ = 0.;
  isDefineArea_ = false;
  
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
    rotateTransform.rotate( 90. );
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
    //  rubberBand_ = new DefoImageLabelRubberBand(QRubberBand::Rectangle, this);
    rubberBand_ = new QRubberBand( QRubberBand::Rectangle, this );
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

  if( isDefineArea_ ) createAndSendArea();

}



///
///
///
void DefoImageLabel::defineArea( void ) {

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
  
  // get the scale factors from the original size of the image;
  // assume the image is scaled to the size of *this

  double xScale = 0., yScale = 0.;
  if( isRotation_ ) {
    xScale = originalImageSize_.height() / static_cast<double>( width() );
    yScale = originalImageSize_.width() / static_cast<double>( height() );
  }
  else {
    xScale = originalImageSize_.width() / static_cast<double>( width() );
    yScale = originalImageSize_.height() / static_cast<double>( height() );
  }

  // this is the rectangle in the displayed picture
  // which is scaled to the size of *this
  QRect rectangle( myPoint_.x(), 
		   myPoint_.y(), 
		   rubberBand_->size().width(),
		   rubberBand_->size().height() );


  // eventually we must rotate it +90deg
  if( isRotation_ ) {
    rectangle.setRect( rectangle.y(), 
		       width() - rectangle.x(), 
		       rectangle.height(), 
		       rectangle.width()   );
  }

  // scale it according to the original image size
  rectangle.setRect(  static_cast<int>( rectangle.x() * xScale ),
		      static_cast<int>( rectangle.y() * yScale ),
		      static_cast<int>( rectangle.width() * xScale ),
		      static_cast<int>( rectangle.height() * yScale )  );

  // send off the result
  emit( DefoArea( rectangle ) );

  // displaying the rubberbands done by other function
  rubberBand_->hide();

  // exit area selection mode
  isDefineArea_ = false;

}
