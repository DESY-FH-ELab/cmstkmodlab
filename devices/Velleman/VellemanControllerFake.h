/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//         Modeled on cmstkmodlab/devices/Conrad/ConradControllerFake.h        //
//                          Last Updated August 16, 2019                       //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef VellemanControllerFake_h
#define VellemanControllerFake_h

#include <vector>


#include "VVellemanController.h"

/// \brief class dealing with use of software without relay card
class VellemanControllerFake : public VVellemanController {



 public:

  VellemanControllerFake(const std::string&);
  virtual ~VellemanControllerFake();

  bool initialize();

  std::vector<bool> queryRelayStatus() const;
  bool setRelay(unsigned relay, unsigned char relayCMD) const;
  bool setSingleRelay(unsigned relay, unsigned char relayCMD) const;
  bool setMultiRelays(std::vector<unsigned> relay, unsigned char relayCMD) const;

 private:

  mutable bool status_[8];

};

#endif // ends #ifndef VellemanControllerFake_h



