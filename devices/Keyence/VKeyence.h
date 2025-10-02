/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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

  virtual void SetABLE(bool on, int out) = 0;
  virtual void SetMaterialMode(int out, int mode) = 0;
  virtual void SetDiffuseMode(int out, int mode) = 0;
  virtual void SetSamplingRate(int mode) = 0;
  virtual void SetAveraging(int out, int mode) = 0;
  virtual void ChangeToCommunicationMode(bool commOn) = 0;
  virtual void MeasurementValueOutput(int out, double & value) = 0;
  virtual void PanelLock(int status) = 0;

  /*
  virtual void Timing(int out, int status) = 0;
  virtual void AutoZero(int out, bool status) = 0;
  */
  virtual void Reset(int out) = 0;
  virtual void ProgramChange(int prog_number) = 0;
  virtual void ProgramCheck(int & value) = 0;
  /*
  virtual void StatResultOutput(int out, std::string value) = 0;
  virtual void ClearStat(int out) = 0;
  virtual void StartDataStorage() = 0;
  virtual void StopDataStorage() = 0;
  virtual void InitDataStorage() = 0;
  virtual void OutputDataStorage(int out, std::vector<double> values) = 0;
  virtual void DataStorageStatus(std::string value) = 0;
  */

  // low level methods
  virtual void SendCommand(const std::string &) = 0;
  virtual void ReceiveString(std::string &) = 0;

  std::string SetValue(const std::string & command);
  std::string SetValue(const std::string & command, int value1);
  std::string SetValue(const std::string & command, int value1, int value2);
  std::string SetValue(const std::string & command, const std::string & value);
  std::string SetValue(const std::string & command, double value1);
  std::string SetValue(const std::string & command, double value1, double value2);
  std::string SetValue(const std::string & command, int value1, const std::string & value2, int value3);

  /*
  void GetValue(const std::string & command, std::string & value);
  void GetValue(const std::string & command, int & value1, std::string & value2);
  void GetValue(const std::string & command, int & value);
  void GetValue(const std::string & command, double & value);
  void GetValue(const std::string & command, int & value1, double & value2);
  void GetValue(const std::string & command, int & value1, std::vector<double> & value2);
  */
};

#endif
