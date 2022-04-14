/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
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
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include <HistoryFifo.h>

#include "Thermo2ThroughPlaneFitter.h"

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

  void setMattermostStatus(bool value) { mattermostStatus_ = value; }
  bool getMattermostStatus() const { return mattermostStatus_; }

  bool getHuberState() const { return huberState_; }
  double getTemperatureSetPoint() const { return huberTemperatureSetPoint_; }
  double getInternalTemperature() const { return huberInternalTemperature_; }
  double getProcessTemperature() const { return huberProcessTemperature_; }

  bool getNGE103BState() const { return nge103BState_; }
  bool getNGE103BChannelState() const { return nge103BChannelState_; }
  double getSourcePower() const { return sourcePower_; }

  unsigned int getKeithleyTopSensor(unsigned int position) const { return keithleyTopSensors_[position]; }
  unsigned int getKeithleyBottomSensor(unsigned int position) const { return keithleyBottomSensors_[position]; }
  unsigned int getKeithleyAmbientSensor() const { return keithleyAmbientSensor_; }
  bool getKeithleyState() const { return keithleyState_; }
  bool getKeithleyTopSensorState(unsigned int position) const { return keithleyTopSensorStates_[position]; }
  bool getKeithleyBottomSensorState(unsigned int position) const { return keithleyBottomSensorStates_[position]; }
  bool getKeithleyAmbientSensorState() const { return keithleyAmbientSensorState_; }
  double getKeithleyTopTemperature(unsigned int position) const { return keithleyTopTemperatures_[position]; }
  const HistoryFifo<double>& getKeithleyTopTemperatureHistory(unsigned int position) const { return keithleyTopTemperatureHistory_[position]; }
  double getKeithleyBottomTemperature(unsigned int position) const { return keithleyBottomTemperatures_[position]; }
  const HistoryFifo<double>& getKeithleyBottomTemperatureHistory(unsigned int position) const { return keithleyBottomTemperatureHistory_[position]; }
  double getKeithleyAmbientTemperature() const { return keithleyAmbientTemperature_; }

  bool getCalculationState() const { return calculationState_; }
  double getGradientTop() const { return gradientTop_; }
  double getPowerTop() const { return powerTop_; }
  double getSampleTemperatureTop() const { return sampleTTop_; }
  double getSampleTemperatureMiddle() const { return sampleTMiddle_; }
  double getSampleTemperatureBottom() const { return sampleTBottom_; }
  double getGradientBottom() const { return gradientBottom_; }
  double getPowerBottom() const { return powerBottom_; }

  void statusMessage(const QString & text);

public slots:

  void setTemperatureSetPoint(double temperature);
  void setSourcePower(double power);

protected slots:

  void configurationChanged();

  void huberInfoChanged();
  void nge103BInfoChanged();
  void keithleyInfoChanged();
  void sendMattermostStatus();

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

  bool mattermostStatus_;
  int mattermostInterval_;
  QTimer *mattermostTimer_;
  QDateTime mattermostStatusTime_;

  double resistance_;
  double cableResistance_;
  double kBlock_;
  double ABlock_;

  bool huberState_;
  double huberTemperatureSetPoint_;
  double huberInternalTemperature_;
  double huberProcessTemperature_;

  unsigned int nge103BChannel_;
  bool nge103BState_;
  bool nge103BChannelState_;
  double nge103BCurrent_;

  std::array<unsigned int,6> keithleyTopSensors_;
  VKeithleyDAQ6510::ChannelMode_t keithleyTopSensorTypes_;
  std::array<double,6> keithleyTopPositions_;
  std::array<double,6> keithleyTopCor0_;
  std::array<double,6> keithleyTopCor1_;
  std::array<double,6> keithleyTopCor2_;
  std::array<double,6> keithleyTopCor3_;
  std::array<unsigned int,6> keithleyBottomSensors_;
  VKeithleyDAQ6510::ChannelMode_t keithleyBottomSensorTypes_;
  std::array<double,6> keithleyBottomPositions_;
  std::array<double,6> keithleyBottomCor0_;
  std::array<double,6> keithleyBottomCor1_;
  std::array<double,6> keithleyBottomCor2_;
  std::array<double,6> keithleyBottomCor3_;
  unsigned int keithleyAmbientSensor_;
  VKeithleyDAQ6510::ChannelMode_t keithleyAmbientSensorType_;
  bool keithleyState_;
  std::array<bool,6> keithleyTopSensorStates_;
  std::array<bool,6> keithleyBottomSensorStates_;
  bool keithleyAmbientSensorState_;
  std::array<double,6> keithleyTopTemperatures_;
  std::array<HistoryFifo<double>,6> keithleyTopTemperatureHistory_;
  std::array<double,6> keithleyBottomTemperatures_;
  std::array<HistoryFifo<double>,6> keithleyBottomTemperatureHistory_;
  double keithleyAmbientTemperature_;

  double sourcePower_;

  Thermo2ThroughPlaneFitter fitter_;

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
  void message(const QString & text);
  void log(const QString & text);
};

#endif // THERMO2THROUGHPLANEMODEL_H
