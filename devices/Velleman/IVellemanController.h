/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//            Modeled on cmstkmodlab/devices/Conrad/VConradController.h        //
//                           Last Updated August 7, 2019                       //                
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef IVellemanController_h
#define IVellemanController_h

#include <string>
#include <vector>

/// \brief Interface Class handling readout of Velleman Relay Card
class IVellemanController {

public:

  IVellemanController(const std::string&);
  virtual ~IVellemanController();

  virtual bool initialize() = 0;

  virtual std::vector<bool> queryRelayStatus() const = 0;
  virtual bool setRelay(unsigned relay, unsigned relayCMD) const = 0;
  virtual bool setSingleRelay(unsigned relay, unsigned relayCMD) const = 0;
  virtual bool setMultiRelays(std::vector<unsigned> relays) const = 0;
};

#endif   // ends #ifndef IVellemanController_h
