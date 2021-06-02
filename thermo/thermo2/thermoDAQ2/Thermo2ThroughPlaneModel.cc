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
#include <cmath>

#include <gsl/gsl_multifit.h>

#include <QApplication>
#include <QDateTime>
#include <QXmlStreamWriter>

#include <nqlogger.h>

#include <ApplicationConfig.h>

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
  ApplicationConfig* config = ApplicationConfig::instance();

  resistance_ = config->getValue<double>("ThroughPlaneResistance");
  cableResistance_ = config->getValue<double>("ThroughPlaneCableResistance");
  kBlock_ = config->getValue<double>("ThroughPlaneKBlock");
  ABlock_ = config->getValue<double>("ThroughPlaneABlock");
  nge103BChannel_ = config->getValue<unsigned int>("ThroughPlaneNGE103BChannel");
  keithleyTopChannels_ = config->getValueArray<unsigned int,6>("ThroughPlaneKeithleyTopChannels");
  keithleyTopPositions_ = config->getValueArray<double,6>("ThroughPlaneKeithleyTopPositions");
  keithleyBottomChannels_ = config->getValueArray<unsigned int,6>("ThroughPlaneKeithleyBottomChannels");
  keithleyBottomPositions_ = config->getValueArray<double,6>("ThroughPlaneKeithleyBottomPositions");

  huberState_ = false;
  huberTemperatureSetPoint_ = 0;

  nge103BState_ = false;
  nge103BChannelState_ = false;
  nge103BCurrent_ = 0;
  sourcePower_ = 0;

  keithleyState_ = false;
  for (unsigned int c=0;c<6;++c) {
  	keithleyTopChannelStates_[c] = false;
  	keithleyBottomChannelStates_[c] = false;
  	keithleyTopTemperatures_[c] = 0;
  	keithleyBottomTemperatures_[c] = 0;
  }

  calculationState_ = false;
  gradientTop_ = 0;
  powerTop_ = 0;
  sampleTTop_ = 0;
  sampleTMiddle_ = 0;
  sampleTBottom_ = 0;
  gradientBottom_ = 0;
  powerBottom_ = 0;

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

  bool changed = false;
  changed |= updateIfChanged<bool>(huberState_, huberModel_->getDeviceState()==READY ? true : false);
  changed |= updateIfChanged<double>(huberTemperatureSetPoint_, huberModel_->getTemperatureSetPoint());

  if (changed) {
  	emit informationChanged();
  }
}

void Thermo2ThroughPlaneModel::nge103BInfoChanged()
{
  NQLogDebug("Thermo2ThroughPlaneModel") << "nge103BInfoChanged()";

  QMutexLocker locker(&mutex_);

  bool changed = false;
  changed |= updateIfChanged<bool>(nge103BState_, nge103BModel_->getDeviceState()==READY ? true : false);
  changed |= updateIfChanged<bool>(nge103BChannelState_, nge103BModel_->getOutputState(nge103BChannel_));
  changed |= updateIfChanged<double>(nge103BCurrent_, nge103BModel_->getCurrent(nge103BChannel_));

  if (changed) {

  	sourcePower_ = resistance_ * std::pow(nge103BCurrent_, 2);

  	emit informationChanged();
  }
}

void Thermo2ThroughPlaneModel::keithleyInfoChanged()
{
  NQLogDebug("Thermo2ThroughPlaneModel") << "keithleyInfoChanged()";

  QMutexLocker locker(&mutex_);

  bool changed = false;
  unsigned int countTop = 0;
  unsigned int countBottom = 0;
  changed |= updateIfChanged<bool>(keithleyState_, keithleyModel_->getDeviceState()==READY ? true : false);
  for (unsigned int i=0;i<6;++i) {
  	changed |= updateIfChanged<bool>(keithleyTopChannelStates_[i], keithleyModel_->getSensorState(keithleyTopChannels_[i])==READY ? true : false);
  	changed |= updateIfChanged<double>(keithleyTopTemperatures_[i], keithleyModel_->getTemperature(keithleyTopChannels_[i]));
  	if (keithleyTopChannelStates_[i]) countTop++;

  	changed |= updateIfChanged<bool>(keithleyBottomChannelStates_[i], keithleyModel_->getSensorState(keithleyBottomChannels_[i])==READY ? true : false);
  	changed |= updateIfChanged<double>(keithleyBottomTemperatures_[i], keithleyModel_->getTemperature(keithleyBottomChannels_[i]));
  	if (keithleyBottomChannelStates_[i]) countBottom++;
  }

  if (changed) {

  	if (countTop>=2 && countBottom>=2) {
  		calculationState_ = true;

  		gsl_matrix *X, *cov;
  		gsl_vector *x, *y, *c;
  		gsl_multifit_robust_workspace * work;
  		unsigned int p;
  		double pos;

  		if (countTop<4) {
  			p = 2;
  		} else {
  			p = 3;
  		}

  		X = gsl_matrix_alloc(countTop, p);
  		x = gsl_vector_alloc(countTop);
  		y = gsl_vector_alloc(countTop);
  		c = gsl_vector_alloc(p);
  		cov = gsl_matrix_alloc(p, p);

  		for (unsigned int i=0;i<countTop;i++) {
  			if (keithleyTopChannelStates_[i]) {
  				pos = keithleyTopPositions_[i];
  				gsl_vector_set(x, i, pos);
  				gsl_vector_set(y, i, keithleyTopTemperatures_[i]);

  				gsl_matrix_set(X, i, 0, 1.0);
  				gsl_matrix_set(X, i, 1, pos);
  				if (p==3) gsl_matrix_set(X, i, 2, pos*pos);
  			}
  		}

  	  work = gsl_multifit_robust_alloc(gsl_multifit_robust_bisquare, X->size1, X->size2);
  	  gsl_multifit_robust(X, y, c, cov, work);
  	  gsl_multifit_robust_free(work);

  	  sampleTTop_ = gsl_vector_get(c, 0);
  	  gradientTop_ = -1.0*gsl_vector_get(c, 1);
  	  powerTop_ = gradientTop_ * kBlock_ * ABlock_;

  	  gsl_matrix_free(X);
  	  gsl_vector_free(x);
  	  gsl_vector_free(y);
  	  gsl_vector_free(c);
  	  gsl_matrix_free(cov);


  		if (countBottom<4) {
  			p = 2;
  		} else {
  			p = 3;
  		}

  		X = gsl_matrix_alloc(countBottom, p);
  		x = gsl_vector_alloc(countBottom);
  		y = gsl_vector_alloc(countBottom);
  		c = gsl_vector_alloc(p);
  		cov = gsl_matrix_alloc(p, p);

  		for (unsigned int i=0;i<countBottom;i++) {
  			if (keithleyBottomChannelStates_[i]) {
  				pos = keithleyBottomPositions_[i];
  				gsl_vector_set(x, i, pos);
  				gsl_vector_set(y, i, keithleyBottomTemperatures_[i]);

  				gsl_matrix_set(X, i, 0, 1.0);
  				gsl_matrix_set(X, i, 1, pos);
  				if (p==3) gsl_matrix_set(X, i, 2, pos*pos);
  			}
  		}

  	  work = gsl_multifit_robust_alloc(gsl_multifit_robust_bisquare, X->size1, X->size2);
  	  gsl_multifit_robust(X, y, c, cov, work);
  	  gsl_multifit_robust_free(work);

  	  sampleTBottom_ = gsl_vector_get(c, 0);
  	  gradientBottom_ = gsl_vector_get(c, 1);
  	  powerBottom_ = gradientBottom_ * kBlock_ * ABlock_;

  	  gsl_matrix_free(X);
  	  gsl_vector_free(x);
  	  gsl_vector_free(y);
  	  gsl_vector_free(c);
  	  gsl_matrix_free(cov);

  	  /*
  		double xsum, x2sum, ysum, xysum;
  		double a, b;

  		xsum=0;
  		x2sum=0;
  		ysum=0;
  		xysum=0;
  		for (unsigned int i=0;i<countTop;i++) {
  			if (keithleyTopChannelStates_[i]) {
  				xsum += -1.0*keithleyTopPositions_[i]; //calculate sigma(xi)
  				ysum += keithleyTopTemperatures_[i]; //calculate sigma(yi)
  				x2sum += std::pow(-1.0*keithleyTopPositions_[i], 2); //calculate sigma(x^2i)
  				xysum += -1.0*keithleyTopPositions_[i] * keithleyTopTemperatures_[i]; //calculate sigma(xi*yi)
  			}
  		}
  		a = (countTop*xysum-xsum*ysum)/(countTop*x2sum-xsum*xsum); //slope
  		b = (x2sum*ysum-xsum*xysum)/(x2sum*countTop-xsum*xsum); // offset

  		gradientTop_ = a;
  		powerTop_ = gradientTop_ * kBlock_ * ABlock_;
  		sampleTTop_ = b;

  		xsum=0;
  		x2sum=0;
  		ysum=0;
  		xysum=0;
  		for (unsigned int i=0;i<countBottom;i++) {
  			if (keithleyBottomChannelStates_[i]) {
  				xsum += -1.0*keithleyBottomPositions_[i]; //calculate sigma(xi)
  				ysum += keithleyBottomTemperatures_[i]; //calculate sigma(yi)
  				x2sum += std::pow(-1.0*keithleyBottomPositions_[i], 2); //calculate sigma(x^2i)
  				xysum += -1.0*keithleyBottomPositions_[i] * keithleyBottomTemperatures_[i]; //calculate sigma(xi*yi)
  			}
  		}
  		a = (countBottom*xysum-xsum*ysum)/(countBottom*x2sum-xsum*xsum); //slope
  		b = (x2sum*ysum-xsum*xysum)/(x2sum*countBottom-xsum*xsum); // offset

  		gradientBottom_ = a;
  		powerBottom_ = gradientBottom_ * kBlock_ * ABlock_;
  		sampleTBottom_ = b;
			*/

  	  sampleTMiddle_ = 0.5*(sampleTTop_ + sampleTBottom_);
  	} else {
  		calculationState_ = false;
  	}

  	emit informationChanged();
  }
}
