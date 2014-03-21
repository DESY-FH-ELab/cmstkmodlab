#ifndef ConradController_h
#define ConradController_h

#include <vector>

#include "VConradController.h"

class ConradCommunication;

/// \brief Class handling readout of Conrad multimeter
class ConradController : public VConradController {

public:

  ConradController(const char* comPort);
  ~ConradController();

  bool initialize();

  std::vector<bool> queryStatus() const;
  bool setChannel(unsigned channel, bool value) const;
  bool setSingleChannel(unsigned channel, bool value) const;

private:
  bool queryRawStatus(unsigned char& status) const;

  ConradCommunication* m_communication;
};

#endif
