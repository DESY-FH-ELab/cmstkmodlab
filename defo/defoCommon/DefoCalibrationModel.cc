/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <fstream>

#include <QFile>
#include <QXmlStreamWriter>

#include "ApplicationConfig.h"

#include "DefoCalibrationModel.h"

DefoCalibrationModel::DefoCalibrationModel(QObject *parent)
: QObject(parent)
{
  calibX_ = ApplicationConfig::instance()->getValue<double>("CALIBX", 1.0);
  calibY_ = ApplicationConfig::instance()->getValue<double>("CALIBY", 1.0);
  calibZx_ = ApplicationConfig::instance()->getValue<double>("CALIBZX", 1.0);
  calibZy_ = ApplicationConfig::instance()->getValue<double>("CALIBZY", 1.0);
}

void DefoCalibrationModel::setCalibX(double v) {
  bool valueChanged = !(v==calibX_);
  calibX_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("CALIBX", calibX_);
    emit calibrationChanged();
  }
}

void DefoCalibrationModel::setCalibY(double v) {
  bool valueChanged = !(v==calibY_);
  calibY_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("CALIBY", calibY_);
    emit calibrationChanged();
  }
}

void DefoCalibrationModel::setCalibZx(double v) {
  bool valueChanged = !(v==calibZx_);
  calibZx_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("CALIBZX", calibZx_);
    emit calibrationChanged();
  }
}

void DefoCalibrationModel::setCalibZy(double v) {
  bool valueChanged = !(v==calibZy_);
  calibZy_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("CALIBZY", calibZy_);
    emit calibrationChanged();
  }
}

void DefoCalibrationModel::write(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoCalibration");

  stream.writeAttribute("x", QString().setNum(calibX_, 'e', 6));
  stream.writeAttribute("y", QString().setNum(calibY_, 'e', 6));
  stream.writeAttribute("zx", QString().setNum(calibZx_, 'e', 6));
  stream.writeAttribute("zy", QString().setNum(calibZy_, 'e', 6));

  stream.writeEndElement();

  stream.writeEndDocument();
}

void DefoCalibrationModel::read(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);

  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoCalibration") {
      calibX_ = stream.attributes().value("x").toString().toDouble();
      calibY_ = stream.attributes().value("y").toString().toDouble();
      calibZx_ = stream.attributes().value("zx").toString().toDouble();
      calibZy_ = stream.attributes().value("zy").toString().toDouble();
    }
  }

  emit calibrationChanged();
}
