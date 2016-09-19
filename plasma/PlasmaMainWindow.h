#ifndef PLASMAMAINWINDOW_H
#define PLASMAMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>

class PlasmaMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit PlasmaMainWindow(QWidget *parent = 0);

public slots:

  void quit();

signals:

protected slots:

protected:

  QDir currentDir_;

  QToolBar* toolBar_;

  QTabWidget* tabWidget_;
};

#endif // PLASMAMAINWINDOW_H
