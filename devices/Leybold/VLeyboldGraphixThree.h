#ifndef _VLEYBOLDGRAPHIXTHREE_H_
#define _VLEYBOLDGRAPHIXTHREE_H_

#include <string>
#include <map>
#include <utility>

typedef const char* ioport_t;

class VLeyboldGraphixThree
{
 public:

  enum SensorDetectionMode {
    SensorDetectionAuto    = 0,
    SensorDetectionManual  = 1
  };

  enum SensorType {
    SensorType_NOSEN     = 0,
    SensorType_TTRx      = 1000,
    SensorType_TTR90     = 1900,
    SensorType_TTR91     = 1910,
    SensorType_TTR91N    = 1911
  };

  enum SensorStatus {
    SensorStatus_nosen   = 0,
    SensorStatus_ok      = 1,
    SensorStatus_range   = 2,
    SensorStatus_soff    = 3,
    SensorStatus_ErrorH  = 4,
    SensorStatus_ErrorL  = 5,
    SensorStatus_ErrorS  = 6,
    SensorStatus_unknown
  };

  enum DisplayUnit {
    DisplayUnit_mbar   = 0,
    DisplayUnit_Torr   = 1,
    DisplayUnit_Pa     = 2,
    DisplayUnit_psi    = 3,
    DisplayUnit_Micron = 4,
    DisplayUnit_unknown
  };

  enum SetPointChannel {
    SetPointChannelOff   = 0,
    SetPointChannel1     = 1,
    SetPointChannel2     = 2,
    SetPointChannel3     = 3
  };

  VLeyboldGraphixThree( const ioport_t );
  virtual ~VLeyboldGraphixThree();

  virtual bool DeviceAvailable() const = 0;

  virtual std::string GetVersion() const = 0;
  virtual int GetSerialNumber() const = 0;
  virtual int GetItemNumber() const = 0;

  virtual int GetNumberOfChannels() const = 0;

  virtual SensorDetectionMode GetSensorDetectionMode(int sensor) const = 0;
  virtual void SetSensorDetectionMode(int sensor, SensorDetectionMode mode) = 0;

  virtual std::string GetSensorTypeName(int sensor) const = 0;
  SensorType GetSensorType(int sensor) const;
  virtual void SetSensorTypeName(int sensor, std::string type) = 0;
  void SetSensorType(int sensor, SensorType type);

  virtual std::string GetSensorName(int sensor) const = 0;
  virtual void SetSensorName(int sensor, const std::string& name) = 0;

  virtual SensorStatus GetSensorStatus(int sensor) const = 0;
  std::string GetSensorStatusText(SensorStatus status) const;
  const std::map<SensorStatus,std::string>& GetSensorStatusTexts() const {
    return sensorTextStatus_;
  }

  virtual double GetPressure(int sensor) const = 0;

  virtual DisplayUnit GetDisplayUnit() const = 0;
  virtual void SetDisplayUnit(DisplayUnit unit) = 0;
  std::string GetDisplayUnitName(DisplayUnit unit) const;
  const std::map<DisplayUnit,std::string>& GetDisplayUnitNames() const {
    return displayUnitNames_;
  }

  virtual SetPointChannel GetSetPointChannelAssignment(int sp) const = 0;
  virtual void SetSetPointChannelAssignment(int sp, SetPointChannel channel) = 0;

  virtual double GetSetPointOnPressure(int sp) const = 0;
  virtual void SetSetPointOnPressure(int sp, double pressure) = 0;

  virtual double GetSetPointOffPressure(int sp) const = 0;
  virtual void SetSetPointOffPressure(int sp, double pressure) = 0;

  virtual bool GetSetPointStatus(int sp) const = 0;

 protected:

  std::map<std::string,SensorStatus> sensorStatusText_;
  std::map<SensorStatus, std::string> sensorTextStatus_;

  std::map<DisplayUnit,std::string> displayUnitNames_;
  std::map<std::string,DisplayUnit> displayNameUnits_;
};

#endif // _VLEYBOLDGRAPHIXTHREE_H_
