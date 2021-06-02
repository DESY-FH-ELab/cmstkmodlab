/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QApplication>
#include <QDateTime>
#include <QXmlStreamWriter>

#include <nqlogger.h>

#include "Thermo2ThroughPlaneModel.h"

Thermo2ThroughPlaneModel::Thermo2ThroughPlaneModel(HuberUnistat525wModel* huberModel,
		RohdeSchwarzNGE103BModel* nge103BModel,
		KeithleyDAQ6510Model* keithleyModel,
		QObject * /* parent */)
 : QObject(),
   huberModel_(huberModel),
   nge103BModel_(nge103BModel),
   keithleyModel_(keithleyModel)
{
  connect(huberModel_, SIGNAL(informationChanged()),
          this, SLOT(huberInfoChanged()));

  connect(nge103BModel_, SIGNAL(informationChanged()),
          this, SLOT(nge103BInfoChanged()));

  connect(keithleyModel_, SIGNAL(informationChanged()),
          this, SLOT(keithleyInfoChanged()));
}

void Thermo2ThroughPlaneModel::huberInfoChanged()
{
  NQLogDebug("Thermo2ThroughPlaneModel") << "huberInfoChanged()";

  QMutexLocker locker(&mutex_);

  /*
  bool changed = false;
  changed |= updateIfChanged<bool>(u525wState_, huberModel_->getDeviceState()==READY ? true : false);
  changed |= updateIfChanged<float>(u525wTemperatureSetPoint_, huberModel_->getTemperatureSetPoint());
  changed |= updateIfChanged<bool>(u525wTemperatureControlMode_, huberModel_->getTemperatureControlMode());
  changed |= updateIfChanged<bool>(u525wTemperatureControlEnabled_, huberModel_->getTemperatureControlEnabled());
  changed |= updateIfChanged<bool>(u525wCirculatorEnabled_, huberModel_->getCirculatorEnabled());
  changed |= updateIfChanged<float>(u525wBathTemperature_, huberModel_->getBathTemperature());
  changed |= updateIfChanged<float>(u525wReturnTemperature_, huberModel_->getReturnTemperature());
  changed |= updateIfChanged<float>(u525wPumpPressure_, huberModel_->getPumpPressure());
  changed |= updateIfChanged<int>(u525wPower_, huberModel_->getPower());
  changed |= updateIfChanged<float>(u525wCWInletTemperature_, huberModel_->getCoolingWaterInletTemperature());
  changed |= updateIfChanged<float>(u525wCWOutletTemperature_, huberModel_->getCoolingWaterOutletTemperature());

  if (changed) {
	emit informationChanged();
  }
  */
}

void Thermo2ThroughPlaneModel::nge103BInfoChanged()
{
  NQLogDebug("Thermo2ThroughPlaneModel") << "nge103BInfoChanged()";

  QMutexLocker locker(&mutex_);

  /*
  for (int i=0;i<3;++i) {

    bool changed = false;
    changed |= updateIfChanged<bool>(nge103BOutputState_[i], nge103BModel_->getOutputState(i+1));
    changed |= updateIfChanged<unsigned int>(nge103BOutputMode_[i], nge103BModel_->getOutputMode(i+1));
    changed |= updateIfChanged<float>(nge103BVoltage_[i], nge103BModel_->getVoltage(i+1));
    changed |= updateIfChanged<float>(nge103BCurrent_[i], nge103BModel_->getCurrent(i+1));
    changed |= updateIfChanged<float>(nge103BMeasuredVoltage_[i], nge103BModel_->getMeasuredVoltage(i+1));
    changed |= updateIfChanged<float>(nge103BMeasuredCurrent_[i], nge103BModel_->getMeasuredCurrent(i+1));
  }

  if (changed) {
  	emit informationChanged();
  }
  */
}

void Thermo2ThroughPlaneModel::keithleyInfoChanged()
{
  NQLogDebug("Thermo2ThroughPlaneModel") << "keithleyInfoChanged()";

  QMutexLocker locker(&mutex_);

  /*
  bool changed = false;
  for (unsigned int card=0;card<2;++card) {
    for (unsigned int channel=0;channel<10;++channel) {
      unsigned int sensor = (card+1)*100 + channel + 1;

      changed |= updateIfChanged<bool>(keithleyState_[card][channel], keithleyModel_->getSensorState(sensor)==READY ? "1" : "0");
      changed |= updateIfChanged<float>(keithleyTemperature_[card][channel], keithleyModel_->getTemperature(sensor));
    }
  }

  if (changed) {
  	emit informationChanged();
  }
  */
}
