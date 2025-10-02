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

#ifndef DEFOLENSMODEL_H
#define DEFOLENSMODEL_H

#include <QObject>
#include <QString>
#include <QMap>

class DefoLens
{
 public:
  
  DefoLens(const QString& name,
	   double p0, double p1 = 0.0, double p2 = 0.0, double p3 = 0.0);
  
  const QString& getName() const { return name_; }
  double getImageScale(double focalLength) const;
  void getParameters(double &p0, double &p1, double &p2, double &p3);

 protected:
  
  QString name_;
  double p0_, p1_, p2_, p3_;
};

class DefoLensModel : public QObject
{
    Q_OBJECT

public:

  explicit DefoLensModel(QObject *parent = 0);

  const QList<QString> getNames() const { return lenses_.keys(); }
  const QString& getCurrentName() const { return lens_->getName(); }
  void getCurrentParameters(double &p0, double &p1, double &p2, double &p3);
  double getImageScale(double focalLength) const;

  void write(const QString& filename);
  void read(const QString& filename);

  public slots:

  void setCurrentName(const QString&);

protected:

  QMap<QString,DefoLens*> lenses_;
  DefoLens* lens_;

protected slots:

signals:

  void lensChanged();
};

#endif // DEFOLENSMODEL_H
