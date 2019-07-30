/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//         Modeled on cmstkmodlab/devices/Conrad/ConradControllerFake.h        //
//                           Last Updated August 8, 2019                       //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef VellemanControllerFake_h
#define VellemanControllerFake_h

#include <vector>

#include "IVellemanController.h"

/// \brief class dealing with ?? (readout of Velleman multimeter OR use of software capabilities without real relay card)
class VellemanControllerFake : public IVellemanController {

 public:

  VellemanControllerFake(const std::string&);
  virtual ~VellemanControllerFake();

  bool initialize();

  std::vector<bool> queryRelayStatus() const;
  bool setRelay(unsigned relay, unsigned relayCMD) const;
  bool setSingleRelay(unsigned relay, unsigned relayCMD) const;
  bool setMultiRelays(std::vector<unsigned> relays) const;

 private:

  mutable bool status_[8];

};

#endif // ends #ifndef VellemanControllerFake_h



