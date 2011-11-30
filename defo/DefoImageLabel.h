
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


///
/// rubberband with a different color
/// NOT IN USE, FIND OUT HOWTO
///
class DefoImageLabelRubberBand : public QRubberBand {

  Q_OBJECT

  public:
  DefoImageLabelRubberBand( Shape s, QWidget * p = 0 ) : QRubberBand( s, p ) {
    QPalette palette;
    palette.setBrush( QPalette::WindowText, QBrush( Qt::red ) );
    setPalette(palette);
  }
    
 protected:
  
  virtual void paintEvent( QPaintEvent * ) {
    QStylePainter painter(this);
    QStyleOptionFocusRect option;
    option.initFrom( this );
    painter.drawControl(QStyle::CE_FocusFrame, option);
  }

};




///
/// qlabel for displaying images
/// with qrubberband support
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

 signals:
  void areaDefined( DefoArea area );
  
 private:
  void mousePressEvent( QMouseEvent* );
  void mouseMoveEvent( QMouseEvent* );
  void mouseReleaseEvent( QMouseEvent* );
  void init( void );
  void createAndSendArea( void );

  QRubberBand *rubberBand_;
  //  DefoImageLabelRubberBand *rubberBand_;
  QPoint myPoint_;
  bool isRotation_; // rotation by -90 deg?
  bool isDefineArea_;
  QSize originalImageSize_;

};
#endif
