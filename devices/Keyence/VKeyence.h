#ifndef __VKEYENCE_H
#define __VKEYENCE_H

#include <string>
#include <vector>

typedef const char* ioport_t;

class VKeyence {

 public:

  VKeyence( const ioport_t );
  virtual ~VKeyence();

  virtual bool DeviceAvailable() const = 0;

  virtual bool GetStatus() = 0;
  virtual int GetError() = 0;

  virtual void Reset() = 0;
  virtual void ConfirmErrorRectification() = 0;
  void ValidConfig();
  void ValidParameter();
  virtual void ModeChange() = 0;
  virtual void MeasurementValueOutput(int out, double value) = 0;
  virtual void Timing(int out, int status) = 0;
  virtual void AutoZero(int out, bool status) = 0;
  virtual void Reset(int out) = 0;
  virtual void PanelLock(int status) = 0;
  virtual void ProgramChange(int prog_number) = 0;
  virtual void ProgramCheck(int value) = 0;
  virtual void StatResultOutput(int out, std::string value) = 0;
  virtual void ClearStat(int out) = 0;
  virtual void StartDataStorage() = 0;
  virtual void StopDataStorage() = 0;
  virtual void InitDataStorage() = 0;
  virtual void OutputDataStorage(int out, std::vector<double> values) = 0;
  virtual void DataStorageStatus(std::string value) = 0;
  virtual void SetSamplingRate(int mode) = 0;
  virtual void SetAveraging(int out, int mode) = 0;

  // low level methods
  virtual void SendCommand(const std::string &) = 0;
  virtual void ReceiveString(std::string &) = 0;

  void SetValue(const std::string & command, int value1);
  void SetValue(const std::string & command, int value1, int value2);
  void SetValue(const std::string & command, const std::string & value);
  void SetValue(const std::string & command, double value1);
  void SetValue(const std::string & command, double value1, double value2);
  void SetValue(const std::string & command, int value1, const std::string & value2, int value3);

  void GetValue(const std::string & command, std::string & value);
  void GetValue(const std::string & command, int & value1, std::string & value2);
  void GetValue(const std::string & command, int & value);
  void GetValue(const std::string & command, double & value);
  void GetValue(const std::string & command, int & value1, double & value2);
  void GetValue(const std::string & command, int & value1, std::vector<double> & value2);
};

#endif
