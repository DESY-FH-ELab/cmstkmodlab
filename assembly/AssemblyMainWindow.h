#ifndef ASSEMBLYMAINWINDOW_H
#define ASSEMBLYMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>

#include "AssemblyUEyeModel.h"

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
};

#endif // ASSEMBLYMAINWINDOW_H
