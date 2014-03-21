#ifndef ConradControllerFake_h
#define ConradControllerFake_h

#include <vector>

#include "VConradController.h"

/// \brief Class handling readout of Conrad multimeter
class ConradControllerFake : public VConradController {

public:

  ConradControllerFake(const char* comPort);
  virtual ~ConradControllerFake();

  bool initialize();

  std::vector<bool> queryStatus() const;
  bool setChannel(unsigned channel, bool value) const;
  bool setSingleChannel(unsigned channel, bool value) const;

private:

  mutable bool status_[8];
};

#endif
