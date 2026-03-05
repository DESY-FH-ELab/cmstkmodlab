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

#include <iostream>
#include <cmath>

#include <QFile>
#include <QXmlStreamWriter>

#include <nqlogger.h>

#include "DefoLensModel.h"

DefoLens::DefoLens(const QString& name,
		   double p0, double p1, double p2, double p3)
  : name_(name),
    p0_(p0),
    p1_(p1),
    p2_(p2),
    p3_(p3)
{
  
}

double DefoLens::getImageScale(double f) const
{
  return p0_ + p1_ * f + p2_ * f * f + p3_ * f * f * f;
}

void DefoLens::getParameters(double &p0, double &p1, double &p2, double &p3)
{
  p0 = p0_;
  p1 = p1_;
  p2 = p2_;
  p3 = p3_;
}

DefoLensModel::DefoLensModel(QObject *parent)
  : QObject(parent)
{
  lens_ = new DefoLens("50 mm lens", 0.0366916);
  lenses_[lens_->getName()] = lens_;

  lens_ = new DefoLens("15 - 85 mm zoom", -0.0240888, 0.00148648, -6.91494e-06);
  lenses_[lens_->getName()] = lens_;
}

double DefoLensModel::getImageScale(double focalLength) const
{
  return lens_->getImageScale(focalLength);
}

void DefoLensModel::getCurrentParameters(double &p0, double &p1, double &p2, double &p3)
{
  lens_->getParameters(p0, p1, p2, p3);
}

void DefoLensModel::setCurrentName(const QString& name)
{
  NQLogSpam("DefoLensModel") << "lens changed to " << name;

  DefoLens * lens = lenses_[name];
  lens_ = lens;

  emit lensChanged();
}

void DefoLensModel::write(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;

  QXmlStreamWriter stream(&file);
  stream.setAutoFormatting(true);
  stream.writeStartDocument();

  stream.writeStartElement("DefoLens");

  stream.writeAttribute("name", lens_->getName());

  stream.writeEndElement();

  stream.writeEndDocument();
}

void DefoLensModel::read(const QString& filename) {

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QXmlStreamReader stream(&file);

  while (!stream.atEnd()) {
    stream.readNextStartElement();

    if (stream.isStartElement() && stream.name()=="DefoLens") {
      QString name = stream.attributes().value("name").toString();

      lens_ = lenses_[name];

      emit lensChanged();
    }
  }
}
