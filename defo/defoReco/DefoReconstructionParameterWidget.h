#ifndef DEFORECONSTRUCTIONPARAMETERWIDGET_H
#define DEFORECONSTRUCTIONPARAMETERWIDGET_H

#include <QWidget>
#include <QSvgWidget>
#include <QDoubleSpinBox>
#include <QResizeEvent>

#include "DefoGeometryModel.h"
#include "DefoCalibrationModel.h"
#include "Defo2DSplineInterpolationModel.h"

class DefoReconstructionParameterWidget : public QWidget
{
  Q_OBJECT
public:

  explicit DefoReconstructionParameterWidget(DefoGeometryModel* geometryModel,
					     DefoCalibrationModel* calibrationModel,
					     Defo2DSplineInterpolationModel* interpolationModel,
					     QWidget *parent = 0);

protected:

  DefoGeometryModel* geometryModel_;
  DefoCalibrationModel* calibrationModel_;
  Defo2DSplineInterpolationModel* interpolationModel_;

  QDoubleSpinBox * angle1SpinBox_;
  QDoubleSpinBox * angle2SpinBox_;
  QDoubleSpinBox * angle3SpinBox_;
  QDoubleSpinBox * distanceSpinBox_;
  QDoubleSpinBox * height1SpinBox_;
  QDoubleSpinBox * height2SpinBox_;

  QDoubleSpinBox * calibXSpinBox_;
  QDoubleSpinBox * calibYSpinBox_;
  QDoubleSpinBox * calibZxSpinBox_;
  QDoubleSpinBox * calibZySpinBox_;

  QSpinBox * kXSpinBox_;
  QSpinBox * kYSpinBox_;
  QDoubleSpinBox * smoothingSpinBox_;
  QDoubleSpinBox * nxySpinBox_;
  QDoubleSpinBox * dXSpinBox_;
  QDoubleSpinBox * dYSpinBox_;

protected slots:

  void angle1Changed(double);
  void angle2Changed(double);
  void angle3Changed(double);
  void distanceChanged(double);
  void height1Changed(double);
  void height2Changed(double);
  
  void geometryChanged();

  void calibXChanged(double);
  void calibYChanged(double);
  void calibZxChanged(double);
  void calibZyChanged(double);

  void calibrationChanged();

  void kXChanged(int);
  void kYChanged(int);
  void smoothingChanged(double);
  void nxyChanged(double);
  void dXChanged(double);
  void dYChanged(double);

  void interpolationParametersChanged();
};

#endif // DEFORECONSTRUCTIONPARAMETERWIDGET_H
