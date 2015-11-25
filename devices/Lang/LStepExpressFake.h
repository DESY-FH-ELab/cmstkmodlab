#ifndef __LSTEPEXPRESSFAKE_H
#define __LSTEPEXPRESSFAKE_H

#include <string>
#include <vector>

#include "VLStepExpress.h"

class LStepExpressFake : public VLStepExpress
{
 public:

  LStepExpressFake( const ioport_t );
  ~LStepExpressFake();

 public:

  bool DeviceAvailable() const { return true; }

  void GetAutoStatus(int & value);
  void SetAutoStatus(int value);

  void GetAxisStatus(std::vector<int> & values);

  void GetAxisEnabled(std::vector<int> & values);
  void GetAxisEnabled(VLStepExpress::Axis axis, int & value);
  void SetAxisEnabled(std::vector<int> & values);
  void SetAxisEnabled(VLStepExpress::Axis axis, int value);

  void GetAxisDirection(std::vector<int> & values);
  void GetAxisDirection(VLStepExpress::Axis axis, int & value);
  void SetAxisDirection(std::vector<int> & values);
  void SetAxisDirection(VLStepExpress::Axis axis, int value);

  void GetDimension(std::vector<int> & values);
  void GetDimension(VLStepExpress::Axis axis, int & value);
  void SetDimension(std::vector<int> & values);
  void SetDimension(VLStepExpress::Axis axis, int value);

  void GetPowerAmplifierStatus(std::vector<int> & values);
  void GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value);
  void SetPowerAmplifierStatus(std::vector<int> & values);
  void SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value);

  void GetPosition(std::vector<double> & values);
  void GetPosition(VLStepExpress::Axis axis, double & value);
  void SetPosition(std::vector<double> & values);
  void SetPosition(VLStepExpress::Axis axis, double value);

  void MoveAbsolute(std::vector<double> & values);
  void MoveAbsolute(double x, double y, double z, double a);
  void MoveAbsolute(VLStepExpress::Axis axis, double value);

  void MoveRelative(std::vector<double> & values);
  void MoveRelative(double x, double y, double z, double a);
  void MoveRelative(VLStepExpress::Axis axis, double value);
  void MoveRelative();

  bool GetStatus() { return true; }
  void GetSystemStatus(std::vector<int> & values);
  void GetSystemStatus(VLStepExpress::Axis axis, int & value);
  int GetError() { return 0; }

  void Reset() {}
  void ConfirmErrorRectification() {}
  void ValidConfig() {}
  void ValidParameter() {}

  // low level debugging methods
  void SendCommand(const std::string & command);
  void ReceiveString(std::string &) { }

  private:

  int autoStatus_;
  std::vector<int> axisStatus_;
  std::vector<int> axis_;
  std::vector<int> axisDirection_;
  std::vector<int> dim_;
  std::vector<int> pa_;
  std::vector<double> position_;
  std::vector<double> moverel_;
};

#endif
