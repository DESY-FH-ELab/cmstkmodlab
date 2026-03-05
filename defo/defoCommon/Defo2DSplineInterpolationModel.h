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

#ifndef DEFO2DSPLINEINTERPOLATIONMODEL_H
#define DEFO2DSPLINEINTERPOLATIONMODEL_H

#include <QObject>

class Defo2DSplineInterpolationModel : public QObject
{
    Q_OBJECT
public:
  explicit Defo2DSplineInterpolationModel(QObject *parent = 0);

  void setKX(int v);
  void setKY(int v);
  void setSmoothing(double v);
  void setNXY(double v);

  void setDX(int v);
  void setDY(int v);

  int getKX() const { return kX_; }
  int getKY() const { return kY_; }
  double getSmoothing() const { return smoothing_; }
  double getNXY() const { return nxy_; }

  int getDX() const { return dX_; }
  int getDY() const { return dY_; }

  void write(const QString& filename);
  void read(const QString& filename);

public slots:

protected:

  int kX_;
  int kY_;
  double smoothing_;
  double nxy_;

  double dX_;
  double dY_;

signals:

  void interpolationParametersChanged();
};

#endif // DEFO2DSPLINEINTERPOLATIONMODEL_H
