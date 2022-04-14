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

  mattermostStatus_ = new QCheckBox("Post status updates on Mattermost");
  mattermostStatus_->setChecked(model_->getMattermostStatus());
  layout->addWidget(mattermostStatus_);

  // Connect all the signals
  connect(mattermostStatus_, SIGNAL(clicked()),
          this, SLOT(mattermostChanged()));

  svgWidget_ = new ThroughPlaneSVGWidget(this);
  svgWidget_->setMinimumWidth(460);
  svgWidget_->setMinimumHeight(svgWidget_->heightForWidth(460));
  layout->addWidget(svgWidget_);

  // Connect all the signals
  connect(model_, SIGNAL(informationChanged()),
          this, SLOT(updateInfo()));

  updateInfo();
}

void Thermo2ThroughPlaneWidget::mattermostChanged()
{
  model_->setMattermostStatus(mattermostStatus_->isChecked());
}

void Thermo2ThroughPlaneWidget::updateInfo()
{
  QString svg = ThroughPlaneSVGString;

  double value;
  QString s, p;

  if (model_->getHuberState()) {
    value = model_->getProcessTemperature();
    s = QString::number(value, 'f', 2);
    svg.replace("§TSink§", s);
  } else {
    svg.replace("§TSink§", "--.--");
  }

  if (model_->getNGE103BState() && model_->getNGE103BChannelState()) {
    value = model_->getSourcePower();
    s = QString::number(value, 'f', 2);
    svg.replace("§PSource§", s);
  } else {
    svg.replace("§PSource§", "--.--");
  }

  for (unsigned int c=0;c<6;++c) {
    s = QString::number(model_->getKeithleyTopSensor(c));
    p = "§CTop" + QString::number(c+1) + "§";
    svg.replace(p, s);

    s = QString::number(model_->getKeithleyBottomSensor(c));
    p = "§CBot" + QString::number(c+1) + "§";
    svg.replace(p, s);
  }
  if (model_->getKeithleyState()) {
    for (unsigned int c=0;c<6;++c) {
      p = "§TTop" + QString::number(c+1) + "§";
      if (model_->getKeithleyTopSensorState(c)) {
        s = QString::number(model_->getKeithleyTopTemperature(c),'f', 4);
        svg.replace(p, s);
      } else {
        svg.replace(p, "--.--");
      }

      p = "§TBot" + QString::number(c+1) + "§";
      if (model_->getKeithleyBottomSensorState(c)) {
        s = QString::number(model_->getKeithleyBottomTemperature(c),'f', 4);
        svg.replace(p, s);
      } else {
        svg.replace(p, "--.--");
      }
    }

    p = "§TAmb§";
    if (model_->getKeithleyAmbientSensorState()) {
      s = QString::number(model_->getKeithleyAmbientTemperature(),'f', 4);
      svg.replace(p, s);
    } else {
      svg.replace(p, "--.--");
    }
  } else {
    for (unsigned int c=0;c<6;++c) {
      p = "§TTop" + QString::number(c+1) + "§";
      svg.replace(p, "--.--");

      p = "§TBot" + QString::number(c+1) + "§";
      svg.replace(p, "--.--");
    }

    p = "§TAmb§";
    svg.replace(p, "--.--");
  }

  if (model_->getCalculationState()) {
    value = model_->getGradientTop();
    s = QString::number(value, 'f', 4);
    svg.replace("§GradTop§", s);

    value = model_->getPowerTop();
    s = QString::number(value, 'f', 4);
    svg.replace("§PTop§", s);

    value = model_->getSampleTemperatureTop();
    s = QString::number(value, 'f', 4);
    svg.replace("§TTopSample§", s);

    value = model_->getSampleTemperatureMiddle();
    s = QString::number(value, 'f', 4);
    svg.replace("§TMidSample§", s);

    value = model_->getSampleTemperatureBottom();
    s = QString::number(value, 'f', 4);
    svg.replace("§TBotSample§", s);

    value = model_->getGradientBottom();
    s = QString::number(value, 'f', 4);
    svg.replace("§GradBot§", s);

    value = model_->getPowerBottom();
    s = QString::number(value, 'f', 4);
    svg.replace("§PBot§", s);

  } else {
    svg.replace("§GradTop§", "--.--");
    svg.replace("§PTop§", "--.--");
    svg.replace("§TTopSample§", "--.--");
    svg.replace("§TMidSample§", "--.--");
    svg.replace("§TBotSample§", "--.--");
    svg.replace("§GradBot§", "--.--");
    svg.replace("§PBot§", "--.--");
  }

  svgWidget_->load(svg.toLocal8Bit());
}
