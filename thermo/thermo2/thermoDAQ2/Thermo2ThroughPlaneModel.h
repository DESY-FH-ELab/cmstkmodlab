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

#ifndef THERMO2THROUGHPLANEMODEL_H
#define THERMO2THROUGHPLANEMODEL_H

#include <vector>
#include <array>

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "HuberUnistat525wModel.h"
#include "RohdeSchwarzNGE103BModel.h"
#include "KeithleyDAQ6510Model.h"

class Thermo2ThroughPlaneModel : public QObject
{
  Q_OBJECT
public:

  explicit Thermo2ThroughPlaneModel(HuberUnistat525wModel* huberModel,
		  RohdeSchwarzNGE103BModel* nge103BModel,
		  KeithleyDAQ6510Model* keithleyModel,
		  QObject *parent = 0);

  bool getHuberState() const { return huberState_; }
  double getSinkTemperature() const { return huberTemperatureSetPoint_; }

  bool getNGE103BState() const { return nge103BState_; }
  bool getNGE103BChannelState() const { return nge103BChannelState_; }
  double getSourcePower() const { return sourcePower_; }

  unsigned int getKeithleyTopChannel(unsigned int channel) const { return keithleyTopChannels_[channel]; }
  unsigned int getKeithleyBottomChannel(unsigned int channel) const { return keithleyBottomChannels_[channel]; }
  bool getKeithleyState() const { return keithleyState_; }
  bool getKeithleyTopChannelState(unsigned int channel) const { return keithleyTopChannelStates_[channel]; }
  bool getKeithleyBottomChannelState(unsigned int channel) const { return keithleyBottomChannelStates_[channel]; }
  double getKeithleyTopTemperature(unsigned int channel) const { return keithleyTopTemperatures_[channel]; }
  double getKeithleyBottomTemperature(unsigned int channel) const { return keithleyBottomTemperatures_[channel]; }

  bool getCalculationState() const { return calculationState_; }
  double getGradientTop() const { return gradientTop_; }
  double getPowerTop() const { return powerTop_; }
  double getSampleTemperatureTop() const { return sampleTTop_; }
  double getSampleTemperatureMiddle() const { return sampleTMiddle_; }
  double getSampleTemperatureBottom() const { return sampleTBottom_; }
  double getGradientBottom() const { return gradientBottom_; }
  double getPowerBottom() const { return powerBottom_; }

public slots:

protected slots:

  void huberInfoChanged();
  void nge103BInfoChanged();
  void keithleyInfoChanged();

protected:

  HuberUnistat525wModel* huberModel_;
  RohdeSchwarzNGE103BModel* nge103BModel_;
  KeithleyDAQ6510Model* keithleyModel_;

  QMutex mutex_;

  QDateTime currentTime_;

  template <typename T> bool updateIfChanged(T &variable, T newValue) {
    if (variable==newValue) return false;
    variable = newValue;
    return true;
  }

  double resistance_;
  double cableResistance_;
  double kBlock_;

  bool huberState_;
  double huberTemperatureSetPoint_;

  unsigned int nge103BChannel_;
  bool nge103BState_;
  bool nge103BChannelState_;
  double nge103BCurrent_;

  std::array<unsigned int,6> keithleyTopChannels_;
  std::array<double,6> keithleyTopPositions_;
  std::array<unsigned int,6> keithleyBottomChannels_;
  std::array<double,6> keithleyBottomPositions_;
  bool keithleyState_;
  std::array<bool,6> keithleyTopChannelStates_;
  std::array<bool,6> keithleyBottomChannelStates_;
  std::array<double,6> keithleyTopTemperatures_;
  std::array<double,6> keithleyBottomTemperatures_;

  double sourcePower_;

  bool calculationState_;
  double gradientTop_;
  double powerTop_;
  double sampleTTop_;
  double sampleTMiddle_;
  double sampleTBottom_;
  double gradientBottom_;
  double powerBottom_;

signals:

  void informationChanged();
};

#endif // THERMO2THROUGHPLANEMODEL_H
