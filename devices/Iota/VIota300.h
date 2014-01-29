#ifndef __VIOTA300_H
#define __VIOTA300_H

#define __IOTA300_LOWER_FLOW_LIMIT 0
#define __IOTA300_UPPER_FLOW_LIMIT 300

#define __IOTA300_LOWER_PRESSURE_LIMIT 0
#define __IOTA300_UPPER_PRESSURE_LIMIT 70

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
  virtual float GetFlow( void ) const = 0;
  virtual float GetPressure( void ) const = 0;
  virtual float GetStatus( void ) const = 0;
};

#endif
