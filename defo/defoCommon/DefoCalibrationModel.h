#ifndef DEFOCALIBRATIONMODEL_H
#define DEFOCALIBRATIONMODEL_H

#include <QObject>

class DefoCalibrationModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoCalibrationModel(QObject *parent = 0);

  void setCalibX(double v);
  void setCalibY(double v);
  void setCalibZx(double v);
  void setCalibZy(double v);

  double getCalibX() const { return calibX_; }
  double getCalibY() const { return calibY_; }
  double getCalibZx() const { return calibZx_; }
  double getCalibZy() const { return calibZy_; }

  void write(const QString& filename);
  void read(const QString& filename);

public slots:

protected:

  double calibX_;
  double calibY_;
  double calibZx_;
  double calibZy_;

signals:

  void calibrationChanged();
};

#endif // DEFOCALIBRATIONMODEL_H
