/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
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

#include "Defo2DSplineInterpolationModel.h"

Defo2DSplineInterpolationModel::Defo2DSplineInterpolationModel(QObject *parent)
: QObject(parent)
{
  kX_ = ApplicationConfig::instance()->getValue<int>("KX", 3);
  kY_ = ApplicationConfig::instance()->getValue<int>("KY", 3);
  smoothing_ = ApplicationConfig::instance()->getValue<double>("SMOOTHING", 0.0);
  nxy_ = ApplicationConfig::instance()->getValue<double>("NXY", 0.75);
  dX_ = ApplicationConfig::instance()->getValue<int>("DX", 5.0);
  dY_ = ApplicationConfig::instance()->getValue<int>("DY", 5.0);
}

void Defo2DSplineInterpolationModel::setKX(int v) 
{
  bool valueChanged = !(v==kX_);
  kX_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<int>("KX", kX_);
    emit interpolationParametersChanged();
  }
}

void Defo2DSplineInterpolationModel::setKY(int v) {
  bool valueChanged = !(v==kY_);
  kY_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<int>("KY", kY_);
    emit interpolationParametersChanged();
  }
}

void Defo2DSplineInterpolationModel::setSmoothing(double v)
{
  bool valueChanged = !(v==smoothing_);
  smoothing_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("SMOOTHING", smoothing_);
    emit interpolationParametersChanged();
  }
}

void Defo2DSplineInterpolationModel::setNXY(double v)
{
  bool valueChanged = !(v==nxy_);
  nxy_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<double>("NXY", nxy_);
    emit interpolationParametersChanged();
  }
}

void Defo2DSplineInterpolationModel::setDX(int v)
{
  bool valueChanged = !(v==dX_);
  dX_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<int>("DX", dX_);
    emit interpolationParametersChanged();
  }
}

void Defo2DSplineInterpolationModel::setDY(int v)
{
  bool valueChanged = !(v==dY_);
  dY_ = v;
  if (valueChanged) {
    ApplicationConfig::instance()->setValue<int>("DY", dY_);
    emit interpolationParametersChanged();
  }
}

void Defo2DSplineInterpolationModel::write(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("Defo2DSplineInterpolation");

  stream.writeAttribute("kx", QString().setNum(kX_));
  stream.writeAttribute("ky", QString().setNum(kY_));
  stream.writeAttribute("s", QString().setNum(smoothing_, 'e', 6));
  stream.writeAttribute("nxy", QString().setNum(nxy_, 'e', 6));
  stream.writeAttribute("dx", QString().setNum(dX_));
  stream.writeAttribute("dy", QString().setNum(dY_));

  stream.writeEndElement();

  stream.writeEndDocument();
}

void Defo2DSplineInterpolationModel::read(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);

  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoCalibration") {
      kX_ = stream.attributes().value("kx").toString().toInt();
      kY_ = stream.attributes().value("ky").toString().toInt();
      smoothing_ = stream.attributes().value("s").toString().toDouble();
      nxy_ = stream.attributes().value("nxy").toString().toDouble();
      dX_ = stream.attributes().value("dx").toString().toInt();
      dY_ = stream.attributes().value("dy").toString().toInt();
     }
  }

  emit interpolationParametersChanged();
}
