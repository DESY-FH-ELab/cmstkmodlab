/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//            Modeled on cmstkmodlab/devices/Conrad/ConradController.h         //
//                          Last Updated August 30, 2019                       //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef VellemanController_h
#define VellemanController_h

#include <vector>


#include "VVellemanController.h"

class VellemanCommunication;

class VellemanController : public VVellemanController {


public:

  VellemanController(const std::string&);
  ~VellemanController();

  bool initialize();

  std::vector<bool> queryRelayStatus() const;
  bool setRelay(unsigned relay, unsigned char relayCMD) const;
  bool setSingleRelay(unsigned relay, unsigned char relayCMD) const;
  bool setMultiRelays(std::vector<unsigned> relays, unsigned char relayCMD) const;
  
  std::string comPort() const; 
  
private:

  bool readStatus(unsigned char& cmd, unsigned char& mask, unsigned char& param1, unsigned char& param2, std::string relays) const;
  bool queryRawStatus(unsigned char& status) const;

  VellemanCommunication* m_communication;
  
};

#endif // ends #ifndef VellemanController_h
