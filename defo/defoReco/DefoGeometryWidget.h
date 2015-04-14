#ifndef DEFOGEOMETRYWIDGET_H
#define DEFOGEOMETRYWIDGET_H

#include <QWidget>
#include <QSvgWidget>
#include <QDoubleSpinBox>
#include <QResizeEvent>

#include "DefoGeometryModel.h"

class DefoSvgWidget : public QSvgWidget
{
    Q_OBJECT
public:
    DefoSvgWidget(QWidget* parent = 0);
    virtual int heightForWidth(int) const;
    virtual int widthForHeight(int) const;

    virtual void resizeEvent(QResizeEvent *event);
};

class DefoGeometryWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoGeometryWidget(DefoGeometryModel* geometryModel,
			      QWidget *parent = 0);

protected:

  DefoGeometryModel* geometryModel_;

  QString sketchSource_;
  DefoSvgWidget * sketch_ ;
  void updateSketch();

protected slots:

  void geometryChanged();
};

#endif // DEFOGEOMETRYWIDGET_H
