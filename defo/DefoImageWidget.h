#ifndef DEFOIMAGEWIDGET_H
#define DEFOIMAGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDateTime>
#include <QSize>
#include "DefoSurfaceModel.h"

class DefoImageWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoImageWidget(DefoSurfaceModel* model, QWidget *parent = 0);

protected:
  DefoSurfaceModel* model_;
  void paintEvent(QPaintEvent *event);

  static QSize MINIMUM_SIZE;

signals:

public slots:
  void referenceChanged();

};

#endif // DEFOIMAGEWIDGET_H
