#ifndef __VLSTEPEXPRESS_H
#define __VLSTEPEXPRESS_H

#include <string>
#include <vector>

typedef const char* ioport_t;

class VLStepExpress {

 public:

  enum Axis {
    X = 0,
    Y = 1,
    Z = 2,
    A = 3
  };

  enum Dimension {
    MICROSTEPS = 0,
    MICROMETER = 1,
    MILLIMETER = 2,
    DEGREE     = 3,
    ROTATIONS  = 4
  };

  VLStepExpress( const ioport_t );
  virtual ~VLStepExpress();

 public:

  virtual bool DeviceAvailable() const = 0;

  void GetDimension(std::vector<int> & values);
  void GetDimension(VLStepExpress::Axis axis, int & value);
  void GetPowerAmplifierStatus(std::vector<int> & values);
  void GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value);
  void GetPosition(std::vector<double> & values);
  void GetPosition(VLStepExpress::Axis axis, double & value);

  void SetDimension(std::vector<int> & values);
  void SetDimension(VLStepExpress::Axis axis, int value);
  void SetPowerAmplifierStatus(std::vector<int> & values);
  void SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value);
  void SetPosition(std::vector<double> & values);
  void SetPosition(VLStepExpress::Axis axis, double value);

  void SetValue(const std::string & command, const std::string & value);
  void SetValue(const std::string & command, VLStepExpress::Axis axis, const std::string & value);
  void SetValue(const std::string & command, int value1);
  void SetValue(const std::string & command, int value1, int value2);
  void SetValue(const std::string & command, int value1, int value2, int value3);
  void SetValue(const std::string & command, int value1, int value2, int value3, int value4);
  void SetValue(const std::string & command, VLStepExpress::Axis axis, int value);
  void SetValue(const std::string & command, double value1);
  void SetValue(const std::string & command, double value1, double value2);
  void SetValue(const std::string & command, double value1, double value2, double value3);
  void SetValue(const std::string & command, double value1, double value2, double value3, double value4);
  void SetValue(const std::string & command, VLStepExpress::Axis axis, double value);
  void SetValue(const std::string & command, const std::vector<int> & values);
  void SetValue(const std::string & command, const std::vector<double> & values);

  void GetValue(const std::string & command, std::string & value);
  void GetValue(const std::string & command, VLStepExpress::Axis axis, std::string & value);
  void GetValue(const std::string & command, std::vector<int> & values);
  void GetValue(const std::string & command, VLStepExpress::Axis axis, int & value);
  void GetValue(const std::string & command, std::vector<double> & values);
  void GetValue(const std::string & command, VLStepExpress::Axis axis, double & value);

  // low level methods
  virtual void SendCommand(const std::string &) = 0;
  virtual void ReceiveString(std::string &) = 0;

 protected:

  char GetAxisName(VLStepExpress::Axis axis);
};

#endif
