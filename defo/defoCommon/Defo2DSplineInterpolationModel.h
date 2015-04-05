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

  void setDX(int v);
  void setDY(int v);

  int getKX() const { return kX_; }
  int getKY() const { return kY_; }
  double getSmoothing() const { return smoothing_; }
  int getDX() const { return dX_; }
  int getDY() const { return dY_; }

  void write(const QString& filename);
  void read(const QString& filename);

public slots:

protected:

  double kX_;
  double kY_;
  double smoothing_;

  double dX_;
  double dY_;

signals:

  void interpolationParametersChanged();
};

#endif // DEFO2DSPLINEINTERPOLATIONMODEL_H
