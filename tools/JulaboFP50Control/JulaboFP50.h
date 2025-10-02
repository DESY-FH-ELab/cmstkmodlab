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

#ifndef __FP50COMHANDLER_H
#define __FP50COMHANDLER_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include "FP50ComHandler.h"


#define __FP50_LOWER_TEMP_LIMIT -50
#define __FP50_UPPER_TEMP_LIMIT  30



class JulaboFP50 {

 public:

  JulaboFP50( const ioport_t );

 public:

  bool SetWorkingTemp( const float ) const;
  bool SetPumpPressure( const unsigned int ) const;
  bool SetCirculatorOn( void ) const;
  bool SetCirculatorOff( void ) const;
  bool SetControlParameters( float, int, int ) const;

  float GetBathTemperature( void ) const;
  float GetSafetySensorTemperature( void ) const;
  unsigned int GetHeatingPower( void ) const;
  unsigned int GetPumpPressure( void ) const;
  bool GetCirculatorStatus( void ) const;
  std::pair<int,std::string> GetStatus( void ) const;
  float GetProportionalParameter( void ) const;
  int GetIntegralParameter( void ) const;
  int GetDifferentialParameter( void ) const;
  
  bool SaveControlParameters( const std::string& ) const;
  bool LoadControlParametersAndApply( const std::string& ) const;
  void StripBuffer( char* ) const;

 private:
  void Device_Init( void );
  FP50ComHandler* comHandler_;

};

#endif
