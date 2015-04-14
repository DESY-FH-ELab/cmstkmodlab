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

protected slots:

};

#endif // DEFORECONSTRUCTIONPARAMETERWIDGET_H
