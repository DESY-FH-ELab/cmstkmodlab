#ifndef __KEYENCE_H
#define __KEYENCE_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "VKeyence.h"
#include "KeyenceComHandler.h"

class Keyence : public VKeyence
{
 public:

  Keyence( const ioport_t );
  ~Keyence();

 public:

  bool DeviceAvailable() const;

  //only in communication mode
  void SetABLE(bool on, int out);
  void SetMaterialMode(int out, int mode);
  void SetDiffuseMode(int out, int mode);
  void SetSamplingRate(int mode);
  void SetAveraging(int out, int mode);

  //in normal mode
  void ChangeToCommunicationMode(bool commOn);
  void MeasurementValueOutput(int out, double value);
  void PanelLock(int status);

  /*
  //probably not necessary
  void Timing(int out, int status);
  void AutoZero(int out, bool status);
  void Reset(int out);
  void ProgramChange(int prog_number);
  void ProgramCheck(int value);
  void StatResultOutput(int out, std::string value);
  void ClearStat(int out);
  void StartDataStorage();
  void StopDataStorage();
  void InitDataStorage();
  void OutputDataStorage(int out, std::vector<double> values);
  void DataStorageStatus(std::string value);
  */

  // low level methods                                                                                                                                                                                      
  void SendCommand(const std::string &);
  void ReceiveString(std::string &);

 private:

  void StripBuffer( char* ) const;
  void DeviceInit();

  KeyenceComHandler* comHandler_;
  bool isDeviceAvailable_;
};

#endif
