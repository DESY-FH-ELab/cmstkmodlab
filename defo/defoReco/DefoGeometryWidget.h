#ifndef DEFOGEOMETRYWIDGET_H
#define DEFOGEOMETRYWIDGET_H

#include <QWidget>
#include <QSvgWidget>
#include <QDoubleSpinBox>

#include "DefoGeometryModel.h"

class DefoSvgWidget : public QSvgWidget
{
    Q_OBJECT
public:
    DefoSvgWidget(QWidget* parent = 0);
    int heightForWidth(int w) const;
};

class DefoGeometryWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoGeometryWidget(
      DefoGeometryModel* geometryModel
    , QWidget *parent = 0
  );

protected:

  DefoGeometryModel* geometryModel_;

  QString sketchSource_;
  DefoSvgWidget * sketch_ ;
  void updateSketch();

  QDoubleSpinBox * angle1SpinBox_;
  QDoubleSpinBox * angle2SpinBox_;
  QDoubleSpinBox * distanceSpinBox_;
  QDoubleSpinBox * height1SpinBox_;
  QDoubleSpinBox * height2SpinBox_;

protected slots:

  void angle1Changed(double);
  void angle2Changed(double);
  void distanceChanged(double);
  void height1Changed(double);
  void height2Changed(double);
  void geometryChanged();
};

#endif // DEFOGEOMETRYWIDGET_H
