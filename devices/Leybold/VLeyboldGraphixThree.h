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


  VLeyboldGraphixThree( const ioport_t );
  virtual ~VLeyboldGraphixThree();

  virtual bool DeviceAvailable() const = 0;

  virtual std::string GetVersion() const = 0;
  virtual int GetSerialNumber() const = 0;
  virtual int GetItemNumber() const = 0;

  virtual int GetNumberOfChannels() const = 0;

  virtual SensorDetectionMode GetSensorDetectionMode(int sensor) const = 0;
  virtual void SetSensorDetectionMode(int sensor, SensorDetectionMode mode) = 0;

  virtual std::string GetSensorType(int sensor) const = 0;
  virtual void SetSensorType(int sensor, std::string type) = 0;

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

 protected:

  std::map<std::string,SensorStatus> sensorStatusText_;
  std::map<SensorStatus, std::string> sensorTextStatus_;

  std::map<DisplayUnit,std::string> displayUnitNames_;
  std::map<std::string,DisplayUnit> displayNameUnits_;
};

#endif // _VLEYBOLDGRAPHIXTHREE_H_
