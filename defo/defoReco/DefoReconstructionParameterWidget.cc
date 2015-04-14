#include <QBoxLayout>
#include <QGridLayout>
#include <iostream>

#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "ApplicationConfig.h"

#include "DefoReconstructionParameterWidget.h"

DefoReconstructionParameterWidget::DefoReconstructionParameterWidget(DefoGeometryModel* geometryModel,
								     DefoCalibrationModel* calibrationModel,
								     Defo2DSplineInterpolationModel* interpolationModel,
								     QWidget *parent)
  : QWidget(parent),
    geometryModel_(geometryModel),
    calibrationModel_(calibrationModel),
    interpolationModel_(interpolationModel)
{

}
