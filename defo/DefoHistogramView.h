
#ifndef _DEFOHISTOGRAMVIEW_H
#define _DEFOHISTOGRAMVIEW_H

#include <iostream>

#include <QWidget>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_engine.h>

#define _PLOTSIZE 256


///
/// histogram plot widget
/// 
class DefoHistogramView : public QwtPlot {

 Q_OBJECT

 public:
  DefoHistogramView ( QWidget* w = 0 );
  void addPixel( QRgb const&  );
  void createCurves( void );
  void clear( void );

 private:
  void init( void );
  void scale( void );

  QwtPlotCurve *redC_;
  QwtPlotCurve *greenC_;
  QwtPlotCurve *blueC_;
  QwtPlotCurve *grayC_;

  double redX_[_PLOTSIZE], redY_[_PLOTSIZE];
  double greenX_[_PLOTSIZE], greenY_[_PLOTSIZE];
  double blueX_[_PLOTSIZE], blueY_[_PLOTSIZE];
  double grayX_[_PLOTSIZE], grayY_[_PLOTSIZE];

};
#endif
