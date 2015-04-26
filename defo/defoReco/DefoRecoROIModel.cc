#include <iostream>
#include <cmath>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoRecoMeasurement.h"
#include "DefoRecoROIModel.h"

DefoRecoROIModel::DefoRecoROIModel(DefoMeasurementSelectionModel *selectionModel,
                                   QObject *parent)
: DefoROIModel(parent),
  selectionModel_(selectionModel)
{
  connect(selectionModel_, SIGNAL(selectionChanged(DefoMeasurement*)),
          this, SLOT(selectionChanged(DefoMeasurement*)));
}

void DefoRecoROIModel::selectionChanged(DefoMeasurement* selection)
{
  DefoRecoMeasurement* recoMeasurement = dynamic_cast<DefoRecoMeasurement*>(selection);
  roi_ = recoMeasurement->roi();
  emit roiChanged();
}
