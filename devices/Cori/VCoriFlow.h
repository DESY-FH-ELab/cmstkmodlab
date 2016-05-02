#ifndef __VCORIFLOW_H
#define __VCORIFLOW_H
#include <string>
using std::string;
typedef const char* ioport_t;

class VCoriFlow
{
 public:

  VCoriFlow( ioport_t );
  virtual ~VCoriFlow() {}
  

  virtual bool IsCommunication( void ) const = 0;

  virtual float getTemp( void ) const = 0;
  virtual float getPres( void ) const = 0;
  virtual float getMeasure( void ) const = 0;
  /// additional stuff
  virtual float getCapacity( void ) const = 0;
  virtual std::string getUnit( void ) const = 0;
  virtual std::string getFluid( void ) const = 0;
  virtual std::string getTag( void ) const = 0;
  virtual std::string setInit( void ) const = 0;
  virtual std::string setTag( void ) const = 0;
  virtual float getDensity( void ) const = 0;
  virtual std::string setCapacity( void ) const = 0;

  protected:
    
  float ToFloat(const char*) const;
  float ToInt(const char*) const;
  std::string ToUnit(const char*) const;
};

#endif
