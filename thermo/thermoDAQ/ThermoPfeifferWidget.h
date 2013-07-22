#ifndef THERMOPFEIFFERWIDGET_H
#define THERMOPFEIFFERWIDGET_H

#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QWidget>
#include <QSvgWidget>
#include <QResizeEvent>

#include "PfeifferModel.h"
#include "DeviceState.h"

class ThermoPfeifferSvgWidget : public QSvgWidget
{
    Q_OBJECT
public:
    ThermoPfeifferSvgWidget(QWidget* parent = 0);
    virtual int heightForWidth(int) const;
    virtual int widthForHeight(int) const;

    virtual void resizeEvent(QResizeEvent *event);
};

class ThermoPfeifferWidget : public QWidget
{
    Q_OBJECT
public:
  explicit ThermoPfeifferWidget(PfeifferModel* model, QWidget *parent = 0);

protected:
  const static int LCD_SIZE = 32;
  
  PfeifferModel* model_;

  QCheckBox* pfeifferCheckBox_;

  QWidget* operationPanel_;

  QString sketchSource_;
  ThermoPfeifferSvgWidget * sketch_;
  void updateSketch();

  double p1_;
  double p2_;

public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // THERMOPFEIFFERWIDGET_H
