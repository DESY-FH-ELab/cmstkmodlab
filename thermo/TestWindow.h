#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>

#include <HamegModel.h>

class TestWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit TestWindow(QWidget *parent = 0);

public slots:

protected:
  HamegModel* hamegModel_;
};

#endif // TESTWINDOW_H
