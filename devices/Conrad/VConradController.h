#ifndef VConradController_h
#define VConradController_h

#include <vector>

/// \brief Class handling readout of Conrad multimeter
class VConradController {

public:

  VConradController(const char* comPort);
  ~VConradController();

  virtual bool initialize() = 0;

  virtual std::vector<bool> queryStatus() const = 0;
  virtual bool setChannel(unsigned channel, bool value) const = 0;
  virtual bool setSingleChannel(unsigned channel, bool value) const = 0;
};

#endif
