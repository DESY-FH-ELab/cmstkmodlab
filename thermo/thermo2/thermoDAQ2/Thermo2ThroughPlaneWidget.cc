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

#include <QGroupBox>
#include <cmath>

#include "ThroughPlaneSVG.h"
#include "Thermo2ThroughPlaneWidget.h"

ThroughPlaneSVGWidget::ThroughPlaneSVGWidget(QWidget* parent)
  : QSvgWidget(parent)
{
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  sizePolicy().setHeightForWidth(true);
}

int ThroughPlaneSVGWidget::heightForWidth(int w) const
{
	return w*500/460;
}

int ThroughPlaneSVGWidget::widthForHeight(int h) const
{
  return h*460/500;
}

void ThroughPlaneSVGWidget::resizeEvent(QResizeEvent *event)
{
	QSize s = event->size();
	int h = heightForWidth(s.width());
	if (h<=s.height()) {
		resize(s.width(), h);
	} else {
		int w = widthForHeight(s.height());
		resize(w, s.height());
	}
}

Thermo2ThroughPlaneWidget::Thermo2ThroughPlaneWidget(Thermo2ThroughPlaneModel* model,
		QWidget *parent)
 : QWidget(parent),
   model_(model)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  setLayout(layout);

  svgWidget_ = new ThroughPlaneSVGWidget(this);
  svgWidget_->setMinimumWidth(460);
  svgWidget_->setMinimumHeight(svgWidget_->heightForWidth(460));
  layout->addWidget(svgWidget_);

  // Connect all the signals
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  updateInfo();
}

void Thermo2ThroughPlaneWidget::updateInfo()
{
	QString svg = svgString;

	/*
  double value;
  QString s;

  value = model_->getPT03();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT03§", s);

  value = model_->getPT05();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT05§", s);
  
  value = model_->getPT01CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT01CO2§", s);

  value = model_->getPT02CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT02CO2§", s);

  value = model_->getPT03CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT03CO2§", s);

  value = model_->getPT04CO2();
  s = QString::number(value, 'f', 2);
  svg.replace("§PT04CO2§", s);
  */

  svgWidget_->load(svg.toLocal8Bit());
}
