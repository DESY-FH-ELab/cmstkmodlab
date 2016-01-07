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

  enum AxisStatus {
    AXISSTANDSANDREADY         = 0x00,
    AXISMOVING                 = 0x01,
    AXISJOYSTICK               = 0x02,
    AXISINCONTROL              = 0x03,
    AXISLIMITSWITCHTRIPPED     = 0x04,
    AXISACKAFTERCALIBRATION    = 0x05,
    AXISERRACKAFTERCALIBRATION = 0x06,
    AXISACKAFTERTBLSTROKEMSR   = 0x07,
    AXISINSETUP                = 0x08,
    AXISTIMEOUT                = 0x09,
    AXISERROR                  = 0x0a,
    AXISDISABLED               = 0x0b,
    AXISSTATEUNKNOWN           = 0xff
  };

  VLStepExpress( const ioport_t );
  virtual ~VLStepExpress();

  virtual bool DeviceAvailable() const = 0;

  virtual void GetAutoStatus(int & value) = 0;
  virtual void SetAutoStatus(int value) = 0;

  virtual void GetAxisStatus(std::vector<int> & values) = 0;

  virtual void GetAxisEnabled(std::vector<int> & values) = 0;
  virtual void GetAxisEnabled(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetAxisEnabled(std::vector<int> & values) = 0;
  virtual void SetAxisEnabled(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetAxisDirection(std::vector<int> & values) = 0;
  virtual void GetAxisDirection(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetAxisDirection(std::vector<int> & values) = 0;
  virtual void SetAxisDirection(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetDimension(std::vector<int> & values) = 0;
  virtual void GetDimension(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetDimension(std::vector<int> & values) = 0;
  virtual void SetDimension(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetPowerAmplifierStatus(std::vector<int> & values) = 0;
  virtual void GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetPowerAmplifierStatus(std::vector<int> & values) = 0;
  virtual void SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value) = 0;

  virtual void GetPosition(std::vector<double> & values) = 0;
  virtual void GetPosition(VLStepExpress::Axis axis, double & value) = 0;
  virtual void SetPosition(std::vector<double> & values) = 0;
  virtual void SetPosition(VLStepExpress::Axis axis, double value) = 0;

  virtual void MoveAbsolute(std::vector<double> & values) = 0;
  virtual void MoveAbsolute(double x, double y, double z, double a) = 0;
  virtual void MoveAbsolute(VLStepExpress::Axis axis, double value) = 0;

  virtual void MoveRelative(std::vector<double> & values) = 0;
  virtual void MoveRelative(double x, double y, double z, double a) = 0;
  virtual void MoveRelative(VLStepExpress::Axis axis, double value) = 0;
  virtual void MoveRelative() = 0;

  virtual bool GetStatus() = 0;
  virtual void GetSystemStatus(std::vector<int>& values) = 0;
  virtual void GetSystemStatus(VLStepExpress::Axis axis, int & value) = 0;
  virtual int GetError() = 0;

  virtual bool GetJoystickEnabled() = 0;
  virtual void SetJoystickEnabled(bool enabled) = 0;

  virtual void GetJoystickAxisEnabled(std::vector<int> & values) = 0;
  virtual void GetJoystickAxisEnabled(VLStepExpress::Axis axis, int & value) = 0;
  virtual void SetJoystickAxisEnabled(std::vector<int> & values) = 0;
  virtual void SetJoystickAxisEnabled(VLStepExpress::Axis axis, int value) = 0;

  virtual void Reset() = 0;
  virtual void ConfirmErrorRectification() = 0;
  virtual void ValidConfig() = 0;
  virtual void ValidParameter() = 0;

  // low level methods
  virtual void SendCommand(const std::string &) = 0;
  virtual void ReceiveString(std::string &) = 0;

  char GetAxisName(VLStepExpress::Axis axis);
  const char * GetAxisDimensionName(VLStepExpress::Dimension dimension);
  char GetAxisStatusText(VLStepExpress::AxisStatus status);
};

#endif
