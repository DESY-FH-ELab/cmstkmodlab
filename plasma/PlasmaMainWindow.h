#ifndef PLASMAMAINWINDOW_H
#define PLASMAMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QDir>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>

#include <NanotecSMCI36Model.h>
#include <NanotecSMCI36Widget.h>

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

  NanotecSMCI36Model *smci36ModelX_;

  QToolBar* toolBar_;

  QTabWidget* tabWidget_;

  NanotecSMCI36Widget *smci36Widget_;
  NanotecSMCI36IOWidget *smci36IOWidget_;
};

#endif // PLASMAMAINWINDOW_H
