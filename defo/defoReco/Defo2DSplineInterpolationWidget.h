#ifndef DEFO2DSPLINEINTERPOLATIONWIDGET_H
#define DEFO2DSPLINEINTERPOLATIONWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include "Defo2DSplineInterpolationModel.h"

class Defo2DSplineInterpolationWidget : public QWidget
{
  Q_OBJECT
public:
  explicit Defo2DSplineInterpolationWidget(Defo2DSplineInterpolationModel* interpolationModel,
                                           QWidget *parent = 0);

protected:

  Defo2DSplineInterpolationModel* interpolationModel_;

  QSpinBox * kXSpinBox_;
  QSpinBox * kYSpinBox_;
  QDoubleSpinBox * smoothingSpinBox_;

  protected slots:

  void kXChanged(int);
  void kYChanged(int);
  void smoothingChanged(double);
  void parametersChanged();
};

#endif // DEFO2DSPLINEINTERPOLATIONWIDGET_H
