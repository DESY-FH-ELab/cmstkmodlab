
#ifndef _DEFOIMAGELABEL_H
#define _DEFOIMAGELABEL_H

#include <iostream>
#include <utility>

#include <QLabel>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QPalette>
#include <QStylePainter>
#include <QStyleOptionFocusRect>

#include "DefoPoint.h"
#include "DefoArea.h"
#include "DefoConfigReader.h"
#include "DefoHistogramView.h"



///
/// rubberband with a different color
///
class DefoImageLabelRubberBand : public QRubberBand {

  Q_OBJECT

  public:
  DefoImageLabelRubberBand( Shape s, QWidget * p = 0 ) : QRubberBand( s, p ) {}
  void setName( QString name ) { name_ = name; }
  QString& getName( void ) { return name_; }

 private:
  QString name_;

 protected:
  
  void paintEvent(QPaintEvent *pe ) {
    //    QRect r( pe->rect().x(), pe->rect().y()-20, pe->rect().width()+40, pe->rect().height()+40 );
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
/// qlabel for displaying images
/// with special qrubberband support
///
class DefoImageLabel : public QLabel {

  Q_OBJECT

 public:
  explicit DefoImageLabel( QWidget * parent=0, Qt::WindowFlags f=0 ) : QLabel( parent, f ) { init(); }
  explicit DefoImageLabel( const QString & text, QWidget * parent=0, Qt::WindowFlags f=0 ) : QLabel( text, parent, f ) { init(); }
  ~DefoImageLabel() {}
  void setRotation( bool rot ) { isRotation_ = rot; }
  void displayImageToSize( QImage& );

 public slots:
  void defineArea( void );
  void displayAreas( bool );
  void refreshAreas( std::vector<DefoArea> area );
  void showHistogram( void );

 signals:
  void areaDefined( DefoArea area );
  
 private:
  void mousePressEvent( QMouseEvent* );
  void mouseMoveEvent( QMouseEvent* );
  void mouseReleaseEvent( QMouseEvent* );
  void init( void );
  void createAndSendArea( void );
  void transformToOriginal( QRect& );
  void transformToLocal( QRect& );

  QImage origImage_;
  DefoImageLabelRubberBand *rubberBand_; // for later
  std::vector<DefoImageLabelRubberBand*> areaRubberBands_;
  QPoint myPoint_;
  DefoHistogramView view_;
  bool isView_;
  bool isRotation_; // rotation by -90 deg?
  bool isDefineArea_;
  QSize originalImageSize_;
  unsigned int debugLevel_;
  bool isDisplayAreas_;

};
#endif
