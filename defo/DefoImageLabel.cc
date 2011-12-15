
#include "DefoImageLabel.h"



///
///
///
void DefoImageLabel::init( void ) {
  
  isRotation_ = 0.;
  isDefineArea_ = false;
  isDisplayAreas_ = false;
  isDisplayIndices_ = false;

  // read parameters
  DefoConfigReader cfgReader( "defo.cfg" );
  debugLevel_ = cfgReader.getValue<unsigned int>( "DEBUG_LEVEL" );
  
  isView_ = false;
  
}



///
///
///
void DefoImageLabel::displayImageToSize( QImage& originalImage ) {

  origImage_ = originalImage; // make a copy

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
    rubberBand_ = new DefoImageLabelAreaRubberBand(QRubberBand::Rectangle, this);
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
  update(); // call paintEvent
  
}



///
///
///
void DefoImageLabel::refreshAreas( std::vector<DefoArea> areas ) {

  // reset vector
  areas_.resize( 0 );
  areas_ = areas;
  update(); // call paintEvent

}



///
///
///
void DefoImageLabel::displayIndices( bool isDisplay ) {

  isDisplayIndices_ = isDisplay;
  update(); // call paintEvent

}



///
///
///
void DefoImageLabel::refreshIndices( std::vector<DefoPoint> points ) {

  indexPoints_.resize( 0 );
  indexPoints_ = points;
  update(); // call paintEvent

}



///
///
///
void DefoImageLabel::displayPointSquares( bool isDisplay ) {

  isDisplayPointSquares_ = isDisplay;
  update(); // call paintEvent

}



///
///
///
void DefoImageLabel::refreshPointSquares( std::vector<DefoSquare> squares ) {

  pointSquares_.resize( 0 );
  pointSquares_ = squares;
  update(); // call paintEvent

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



///
///
///
void DefoImageLabel::showHistogram( void ) {

  // if it exists already, we shut it down
  if( isView_ ) {
    view_.hide();
    isView_ = false;
  }
  
  // otherwise, we create and show it
  else {
    
    view_.clear();
    isView_ = true;
    QRect r;

    // if there's an area defined, we display info from it,
    // otherwise from the whole image
    if( areas_.size() ) {
      r = areas_.at( 0 ).getRectangle();
      transformToOriginal( r );
    }
    else {
      r = origImage_.rect();
    }
    
    for( int x = r.x(); x < r.x() + r.width(); ++x ) {
      for( int y = r.y(); y < r.y() + r.height(); ++y ) {
	view_.addPixel( origImage_.pixel( x, y ) );
      }
    }
    
    view_.createCurves();
    view_.show();
    view_.replot();

  }

}



///
///
///
void DefoImageLabel::paintEvent( QPaintEvent* e ) {

  // first draw the label
  QLabel:: paintEvent( e );

  // then additional items:



  // areas
  if( isDisplayAreas_ ) {

    for( std::vector<DefoArea>::iterator it = areas_.begin(); it < areas_.end(); ++it ) {

      QRect r = it->getRectangle();
      transformToLocal( r );
      QPainter painter(this);
      QPen pen( Qt::yellow, 1 );
      pen.setStyle( Qt::DotLine );
      painter.setPen( pen );
      painter.drawRect( r );
      painter.drawText( r.x() + 3, r.y() + 13, it->getName() );

    }

  }


  // points and HW sqaures around them
  if( isDisplayPointSquares_ ) {

    for( std::vector<DefoSquare>::iterator it = pointSquares_.begin(); it < pointSquares_.end(); ++it ) {

      QPainter painter(this);

      // blue point is marked differently
      if( it->getCenter().isBlue() ) painter.setPen( QPen( Qt::red, 1 ) );
      else painter.setPen( QPen( Qt::blue, 1 ) );

      QRect r( it->getCenter().getPixX(), it->getCenter().getPixY(), 0, 0 ); // we pack it in a QRect so we can use the transform methods
      transformToLocal( r );
      QPoint p = r.topLeft(); // this is the reco point position

      // first the cross

      for( int px = -1; px <= 1; ++px )
	painter.drawPoint( p + QPoint( px, 0 ) );
      for( int py = -1; py <= 1; ++py )
	painter.drawPoint( p + QPoint( 0, py ) );

      // then the square
      r = QRect( it->getCenter().getPixX() - it->getHalfWidth(),
		 it->getCenter().getPixY() - it->getHalfWidth(),
		 it->getHalfWidth() * 2, it->getHalfWidth() * 2  );
      transformToLocal( r );
      painter.drawRect( r );

    }

  }



  // indices
  if( isDisplayIndices_ ) {

    // move the indices text a bit right/up
    const std::pair<int,int> offset( 3, -3 );

    for( std::vector<DefoPoint>::const_iterator it = indexPoints_.begin(); it < indexPoints_.end(); ++it ) {
      QRect r( it->getX(), it->getY(), 0, 0 ); // we pack it in a QRect so we can use the transform methods
      transformToLocal( r );
      QPainter painter( this );
      QPen pen( Qt::magenta, 2 );
      painter.setPen( pen );
      QFont font;
      font.setPointSize( 7 );
      painter.setFont( font );
      painter.drawText( r.x() + offset.first, r.y() + offset.second, 
        QString::number( it->getIndex().first ) + "," + QString::number( it->getIndex().second ) );
    }

  }


}
