#ifndef CONRADMANAGER_H
#define CONRADMANAGER_H

#include <QObject>
#include <QTimer>
#include "ConradModel.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif


class ConradManager : public QObject
{
  Q_OBJECT

 protected:
  ConradModel* ConradModel_;
  int channelNumber;
  QTimer* liveTimer_;

 private:
  const int togglingVacuumDelay = 3000;
  
 public:
  //add constructor
  explicit ConradManager(ConradModel* ConradModel_);

public slots:

    void toggleVacuum(int);
    void vacuumToggled();
    void updateVacuumChannelsStatus();

    //signal to say when vacuum state has changed (needs timer)
 signals:
    void updateVacuumChannelState(int, bool);
    void enableVacuumButton();
    void disableVacuumButton();
};

#endif // CONRADMANAGER_H



/*

questions:
1) conradModel is not initialized in MainWindow but is sent to AssemblyModuleAssembler
2) qt timer in ConradManager SLOT
3) qt timer constractor
4) ComradManager thread safe version, channelNumber, state
5) debugSlot
6) add checking to all USB ports for every device.

 */
