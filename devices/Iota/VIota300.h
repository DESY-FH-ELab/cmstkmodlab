#ifndef __VIOTA300_H
#define __VIOTA300_H

typedef const char* ioport_t;

class VIota300
{
 public:

  VIota300( ioport_t );
  virtual ~VIota300() {}
  
  virtual bool SetFlow( const float ) const = 0;
  virtual bool SetPressure( const float ) const = 0;
  virtual bool SetStatus( const float ) const = 0;  
  //virtual bool SetControlParameters( float, int, int ) const = 0;

  virtual bool IsCommunication( void ) const = 0;

  virtual float GetSetFlow( void ) const = 0;
  virtual float GetActFlow( void ) const = 0;
  virtual float GetSetPressure( void ) const = 0;
  virtual float GetActPressure( void ) const = 0;
  virtual float GetStatus( void ) const = 0;

  static constexpr int Iota300LowerFlowLimit =   0;
  static constexpr int Iota300UpperFlowLimit = 300;

  static constexpr int Iota300LowerPressureLimit =  0;
  static constexpr int Iota300UpperPressureLimit = 70;
};

#endif
