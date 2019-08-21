/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//        Modeled on cmstkmodlab/assembly/assemblyCommon/ConradManager.h       //
//                         Last Updated August 26, 2019                        //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef VELLEMANMANAGER_H
#define VELLEMANMANAGER_H

#include <VellemanModel.h>

#include <QObject>
#include <QTimer>

class VellemanManager : public QObject
{
  Q_OBJECT


 public:

  // add constructor
  explicit VellemanManager(VellemanModel* const);

  VellemanModel* vellemanModel() const;

 protected:

  VellemanModel* const vellemanModel_;

  int relayNumber_;
  QTimer* liveTimer_;

 protected:

  const int togglingVacuumDelay = 3000;

 public slots:

   void toggleVacuum(const int);
   void vacuumToggled();

   void transmit_vacuumRelayState(const int);

   void enableVacuum(const int);
   void disableVacuum(const int);
   
 signals:

   void vacuumRelayState(int, bool);

   void enableVacuumButton();
   void disableVacuumButton();

   void vacuum_toggled();

   void vacuum_enabled();
   void vacuum_disabled();
   void vacuum_error();
  
}; // end of VellemanManager class declarations

#endif // VELLEMANMANAGER_H

/*

QUESTIONS (from ConradManager):
1) conradModel() is not initialized in MainWindow but is sent to AssemblyModuleAssembler
2) qt timer in ConradManager SLOT
3) qt timer constructor
4) ConradManager thread safe version, relayNumber, state
5) debugSlot
6) add checking to all USB ports for every device
7) add try-catch in setNewThreshold slot.

Link to uEye datasheet
https://www.ids-imaging.us/manuals/uEye_SDK/EN/uEye_Manual_4.92.2/index.html
*/
