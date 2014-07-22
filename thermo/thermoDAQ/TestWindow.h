#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>

#include <HuberPetiteFleurModel.h>
#include <HamegModel.h>
#include <KeithleyModel.h>
#include <PfeifferModel.h>
#include <IotaModel.h>

class TestWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit TestWindow(QWidget *parent = 0);

public slots:

protected:
  HuberPetiteFleurModel* huberModel_;
  HamegModel* hamegModel_;
  KeithleyModel* keithleyModel_;
  PfeifferModel* pfeifferModel_;
  IotaModel* iotaModel_;
};

#endif // TESTWINDOW_H
