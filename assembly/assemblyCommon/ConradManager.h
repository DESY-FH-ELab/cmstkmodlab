#ifndef CONRADMANAGER_H
#define CONRADMANAGER_H

#include <QObject>
#include <ConradModel.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif


class ConradManager : public QObject
{
  Q_OBJECT

 public:
  ConradModel* ConradModel_;

  //add constructor
  explicit ConradManager(ConradModel* ConradModel_);

public slots:

    void toggleVacuum(int);

    //signal to say when vacuum state has changed (needs timer)

};

#endif // CONRADMANAGER_H
