/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef __VJULABOFP50_H
#define __VJULABOFP50_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

typedef const char* ioport_t;

class VJulaboFP50
{
 public:

  VJulaboFP50( ioport_t );
  virtual ~VJulaboFP50() {}
  
  virtual bool SetWorkingTemperature( const float ) const = 0;
  virtual bool SetPumpPressure( const unsigned int ) const = 0;
  virtual bool SetCirculatorOn( void ) const = 0;
  virtual bool SetCirculatorOff( void ) const = 0;
  virtual bool SetControlParameters( float, int, int ) const = 0;

  virtual bool IsCommunication( void ) const = 0;
  virtual float GetBathTemperature( void ) const = 0;
  virtual float GetSafetySensorTemperature( void ) const = 0;
  virtual float GetWorkingTemperature( void ) const = 0;
  virtual int GetHeatingPower( void ) const = 0;
  virtual unsigned int GetPumpPressure( void ) const = 0;
  virtual bool GetCirculatorStatus( void ) const = 0;
  virtual std::pair<int,std::string> GetStatus( void ) const = 0;
  virtual float GetProportionalParameter( void ) const = 0;
  virtual int GetIntegralParameter( void ) const = 0;
  virtual int GetDifferentialParameter( void ) const = 0;

  virtual bool SaveControlParameters( const std::string& ) const = 0;
  virtual bool LoadControlParametersAndApply( const std::string& ) const = 0;
  
  static constexpr int FP50LowerTempLimit = -50;
  static constexpr int FP50UpperTempLimit =  30;
};

#endif
