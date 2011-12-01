

#ifndef _DEFOAREA_H
#define _DEFOAREA_H

#include <QRect>

///
///
///
class DefoArea {

 public:
  DefoArea() {}
  DefoArea( QRect& r ) { rectangle_ = r; }
  ~DefoArea() {}
  
  void setRectangle( QRect& r ) { rectangle_ = r; }
  void setName( QString& name ) { name_ = name; }
  QRect& getRectangle( void ) { return rectangle_; }
  QString& getName( void ) { return name_; }

 private:
  QRect rectangle_;
  QString name_;

};


#endif
