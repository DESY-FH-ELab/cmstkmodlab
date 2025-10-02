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

#ifndef __KEYENCEFAKE_H
#define __KEYENCEFAKE_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "VKeyence.h"
#include "KeyenceComHandler.h"

class KeyenceFake : public VKeyence
{
 public:

  KeyenceFake( const ioport_t );
  ~KeyenceFake();

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
  void MeasurementValueOutput(int out, double & value);
  void PanelLock(int status);

  /*
  //probably not necessary
  void Timing(int out, int status);
  void AutoZero(int out, bool status);
  */
  void Reset(int out);
  void ProgramChange(int prog_number);
  void ProgramCheck(int & value);
  /*
  void StatResultOutput(int out, std::string value);
  void ClearStat(int out);
  void StartDataStorage();
  void StopDataStorage();
  void InitDataStorage();
  void OutputDataStorage(int out, std::vector<double> values);
  void DataStorageStatus(std::string value);
  */

  //for initialization
  //communicationspeed;
  //type_of_parity;
  //autotransmission;
      

  // low level methods                                                                                                                                                                                      
  void SendCommand(const std::string &);
  void ReceiveString(std::string &);

 private:

  void StripBuffer( std::string &) const;
  void DeviceInit();

  KeyenceComHandler* comHandler_;
  bool isDeviceAvailable_;
  bool comMode_;
  int samplingRate_;
  int averagingRate_;
};

#endif
