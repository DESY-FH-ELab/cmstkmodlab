/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//            Modeled on cmstkmodlab/devices/Conrad/ConradController.h         //
//                           Last Updated August 6, 2019                       //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef VellemanController_h
#define VellemanController_h

#include <vector>

#include "IVellemanController.h"

class VellemanCommunication;

class VellemanController : public IVellemanController {

public:

  VellemanController(const std::string&);
  ~VellemanController();

  bool initialize();

  std::vector<bool> queryRelayStatus() const;
  bool setRelay(unsigned relay, unsigned relayCMD) const;
  bool setSingleRelay(unsigned relay, unsigned relayCMD) const;
  bool setMultiRelays(std::vector<unsigned> relays) const;
  
  std::string comPort() const; 
  
private:

  bool queryRawStatus(unsigned char& status) const;

  VellemanCommunication* m_communication;
  
};

#endif // ends #ifndef VellemanController_h
