#ifndef DEFOGEOMETRYPARAMETERWIDGET_H
#define DEFOGEOMETRYPARAMETERWIDGET_H

#include <QWidget>
#include <QSvgWidget>
#include <QDoubleSpinBox>
#include <QResizeEvent>

#include "DefoGeometryModel.h"

class DefoGeometryParameterWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoGeometryParameterWidget(DefoGeometryModel* geometryModel,
				       QWidget *parent = 0);

protected:

  DefoGeometryModel* geometryModel_;

  QDoubleSpinBox * angle1SpinBox_;
  QDoubleSpinBox * angle2SpinBox_;
  QDoubleSpinBox * angle3SpinBox_;
  QDoubleSpinBox * distanceSpinBox_;
  QDoubleSpinBox * height1SpinBox_;
  QDoubleSpinBox * height2SpinBox_;

protected slots:

  void angle1Changed(double);
  void angle2Changed(double);
  void angle3Changed(double);
  void distanceChanged(double);
  void height1Changed(double);
  void height2Changed(double);
  void geometryChanged();
};

#endif // DEFOGEOMETRYPARAMETERWIDGET_H
