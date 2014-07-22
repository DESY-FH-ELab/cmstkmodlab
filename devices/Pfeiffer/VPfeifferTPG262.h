#ifndef __VPFEIFFERTPG262_H
#define __VPFEIFFERTPG262_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

typedef const char* ioport_t;

class VPfeifferTPG262
{
 public:

  enum ErrorCode {
    tgp262NoError =            0,
    tpg262SyntaxError =        1,
    tpg262Parameter =         10,
    tpg262NoHardware =       100,
    tpg262ControllerError = 1000,
    tpg262Invalid
  };

  enum GaugeStatus {
    tpg262GaugeOkay =                0,
    tpg262GaugeUnderrange =          1,
    tpg262GaugeOverrange =           2,
    tpg262GaugeSensorError =         3,
    tpg262GaugeSensorOff =           4,
    tpg262GaugeNoSensor =            5,
    tpg262GaugeIdentificationError = 6,
    tpg262GaugeInvalidStatus
  };

  typedef std::pair<GaugeStatus,float> reading_t;

  VPfeifferTPG262(ioport_t);
  virtual ~VPfeifferTPG262();
  
  virtual bool IsCommunication( void ) const = 0;
  virtual int GetErrorStatus( void ) const = 0;
  virtual bool GetPressure1(reading_t & reading) = 0;
  virtual bool GetPressure2(reading_t & reading) = 0;
  virtual bool GetPressures(reading_t & reading1, reading_t & reading2) = 0;

  static std::string GetGaugeStatusAsString(GaugeStatus status);

  /*
  virtual void SendETX (void) const = 0;
  virtual void Reset (void) const = 0;
  virtual int KeyLock( void ) const = 0;
  virtual void SetContinuous( void ) const = 0;
  virtual stat_t Readout( float & pressure1, float & pressure2 ) const = 0;
  virtual int GuageIdentification (void) const = 0;
  */
};

#endif
