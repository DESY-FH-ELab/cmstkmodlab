/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef CONRADMANAGER_H
#define CONRADMANAGER_H

#include <ConradModel.h>

#include <QObject>
#include <QTimer>

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
  void  enableVacuumButton();
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
7) add try-catch in setNewThreshold slot.


Link to uEye datasheet
http://www.physimetrics.com/content/PDF_Content/Cameras/IDS/UI-3480CP-M-GL.pdf

*/
