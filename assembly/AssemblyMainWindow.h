#ifndef ASSEMBLYMAINWINDOW_H
#define ASSEMBLYMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>

#include "AssemblyUEyeModel.h"
#include "AssemblyUEyeWidget.h"

class AssemblyMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit AssemblyMainWindow(QWidget *parent = 0);

public slots:

  void quit();

protected:

  QDir currentDir_;

  QTabWidget* tabWidget_;

  AssemblyUEyeModel* uEyeModel_;
  AssemblyUEyeWidget* uEyeWidget_;
};

#endif // ASSEMBLYMAINWINDOW_H
