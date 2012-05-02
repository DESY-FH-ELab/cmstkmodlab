
#ifndef _DEFOIMAGELABEL_H
#define _DEFOIMAGELABEL_H

#include <iostream>
#include <utility>

#include <QLabel>
//#include <QGraphicsView>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QPalette>
#include <QStylePainter>
#include <QStyleOptionFocusRect>

#include "DefoPoint.h"
#include "DefoSquare.h"
#include "DefoArea.h"
#include "DefoConfigReader.h"
#include "DefoHistogramView.h"



///
/// rubberband with a different color,
/// for displaying selected areas
///
class DefoImageLabelAreaRubberBand : public QRubberBand {

  Q_OBJECT

 public:
  DefoImageLabelAreaRubberBand( Shape s, QWidget* p = 0 ) : QRubberBand( s, p ) {}
  void setName( QString name ) { name_ = name; }
  QString& getName( void ) { return name_; }

 private:
  QString name_;

 protected:

  virtual void paintEvent(QPaintEvent *pe ) {
    QRegion reg( pe->rect() );
    setMask( reg ); // need to replace the mask to fill the entire rect
    QPainter painter(this);
    QPen pen( Qt::yellow, 2 );
    pen.setStyle( Qt::DashLine );
    painter.setPen( pen );
    painter.drawRect( pe->rect() );
    painter.drawText( 3,13, name_ );
  }

  
  
};



///
/// QLabel for displaying images with special QRubberband support
/// for displaying areas and scaled/rotated display
///
/// Objects of this class keep a copy of the the original image (origImage_)
/// as well as a second "local" image for display which is scaled to the
/// size of the DefoImageLabel and - if wanted - rotated wrt. the original.
/// The transform methods do the transformations of a QRect (e.g. a DefoArea)
/// between both frames.
///
/// Additionally, this class knows how to display areas, point indices and
/// the point indicator which it receives from DefoMainWindow
///
class DefoImageLabel : public QLabel {

  Q_OBJECT

 public:
  explicit DefoImageLabel( QWidget * parent=0, Qt::WindowFlags f=0 ) : QLabel( parent, f ) { init(); }
  explicit DefoImageLabel( const QString & text, QWidget * parent=0, Qt::WindowFlags f=0 ) : QLabel( text, parent, f ) { init(); }
  ~DefoImageLabel() {}
  void setRotation( bool rot ) { isRotation_ = rot; }
  void displayImageToSize( QImage& );
  bool isImage( void ) { return isImage_; }
  QImage& getOriginalImage( void ) { return origImage_; }

 public slots:
  void defineArea( void );
  void displayAreas( bool );
  void refreshAreas( std::vector<DefoArea> area );
  void displayIndices( bool );
  void refreshIndices( std::vector<DefoPoint> points );
  void showHistogram( void );
  void displayPointSquares( bool );
  void refreshPointSquares( std::vector<DefoSquare> squares );
  void displayCoords( bool );

 signals:
  void areaDefined( DefoArea area );
  
 private:
  virtual void paintEvent(QPaintEvent* );
  void mousePressEvent( QMouseEvent* );
  void mouseMoveEvent( QMouseEvent* );
  void mouseReleaseEvent( QMouseEvent* );
  void init( void );
  void createAndSendArea( void );
  void transformToOriginal( QRect& );
  void transformToLocal( QRect& );

  QImage origImage_;
  DefoImageLabelAreaRubberBand *rubberBand_;
  std::vector<DefoArea> areas_;
  std::vector<DefoPoint> indexPoints_;
  std::vector<DefoSquare> pointSquares_;
  bool isImage_;
  QPoint myPoint_;
  DefoHistogramView view_;
  bool isView_;
  bool isRotation_; // rotation by -90 deg?
  bool isDefineArea_;
  QSize originalImageSize_;
  unsigned int debugLevel_;
  bool isDisplayAreas_;
  bool isDisplayIndices_;
  bool isDisplayPointSquares_;
  bool isDisplayCoords_;

};
#endif
