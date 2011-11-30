

#ifndef _DEFOAREA_H
#define _DEFOAREA_H

#include <utility>

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

 private:
  QRect rectangle_;

};


#endif
