#ifndef VConradController_h
#define VConradController_h

#include <vector>

typedef const char* ioport_t;

/// \brief Class handling readout of Conrad multimeter
class VConradController {

public:

  VConradController( const ioport_t );
  virtual ~VConradController();

  virtual bool initialize() = 0;

  virtual std::vector<bool> queryStatus() const = 0;
  virtual bool setChannel(unsigned channel, bool value) const = 0;
  virtual bool setSingleChannel(unsigned channel, bool value) const = 0;
};

#endif
