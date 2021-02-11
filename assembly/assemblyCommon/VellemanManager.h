/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//        Modeled on cmstkmodlab/assembly/assemblyCommon/ConradManager.h       //
//                        Last Updated September 5, 2019                       //
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

  int channelNumber_;
  QTimer* liveTimer_;

 protected:

  const int togglingVacuumDelay = 3000;

 public slots:

   void toggleVacuum(const int);
   void vacuumToggled();

   void transmit_vacuumChannelState(const int);

   void enableVacuum(const int);
   void disableVacuum(const int);
   
 signals:

   void vacuumChannelState(int, bool);

   void enableVacuumButton();
   void disableVacuumButton();

   void vacuum_toggled();

   void vacuum_enabled();
   void vacuum_disabled();
   void vacuum_error();
  
}; // end of VellemanManager class declarations

#endif // VELLEMANMANAGER_H

