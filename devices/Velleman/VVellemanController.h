/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//            Modeled on cmstkmodlab/devices/Conrad/VConradController.h        //
//                           Last Updated August 16, 2019                       //                
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef VVellemanController_h
#define VVellemanController_h

#include <string>
#include <vector>

/// \brief Interface Class handling readout of Velleman Relay Card
class VVellemanController {

public:

  VVellemanController(const std::string&);
  virtual ~VVellemanController();

  virtual bool initialize() = 0;

  virtual std::vector<bool> queryRelayStatus() const = 0;
  virtual bool setRelay(unsigned relay, unsigned char relayCMD) const = 0;
  virtual bool setSingleRelay(unsigned relay, unsigned char relayCMD) const = 0;
  virtual bool setMultiRelays(std::vector<unsigned> relays, unsigned char relayCMD) const = 0;
};

#endif   // ends #ifndef VVellemanController_h
