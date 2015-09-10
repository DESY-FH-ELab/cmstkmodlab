#include <iostream>
#include <cmath>

#include <QFile>
#include <QXmlStreamWriter>

#include <nqlogger.h>

#include "DefoRecoMeasurement.h"
#include "DefoRecoROIModel.h"

DefoRecoROIModel::DefoRecoROIModel(DefoMeasurementSelectionModel *selectionModel,
                                   QObject *parent)
: DefoROIModel(parent),
  selectionModel_(selectionModel),
  otherROIModel_(0)
{
  connect(selectionModel_, SIGNAL(selectionChanged(DefoMeasurement*)),
          this, SLOT(selectionChanged(DefoMeasurement*)));
}

DefoRecoROIModel::DefoRecoROIModel(DefoMeasurementSelectionModel *selectionModel,
				   DefoRecoROIModel *other,
                                   QObject *parent)
: DefoROIModel(parent),
  selectionModel_(selectionModel),
  otherROIModel_(other)
{
  connect(selectionModel_, SIGNAL(selectionChanged(DefoMeasurement*)),
          this, SLOT(selectionChanged(DefoMeasurement*)));

  connect(otherROIModel_, SIGNAL(roiChanged(bool)),
	  this, SLOT(otherROIChanged(bool)));
}

void DefoRecoROIModel::selectionChanged(DefoMeasurement* selection)
{
  DefoRecoMeasurement* recoMeasurement = dynamic_cast<DefoRecoMeasurement*>(selection);
  roi_ = recoMeasurement->roi();
  emit roiChanged(true);
}

void DefoRecoROIModel::otherROIChanged(bool nu)
{
  if (otherROIModel_->isEqualTo(this)) {
    // NQLogMessage("DefoRecoROIModel") << "otherROIChanged(" << (int)nu << ")";
    emit roiChanged(nu);
  }
}
