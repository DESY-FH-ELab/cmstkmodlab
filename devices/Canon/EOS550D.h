

#ifndef _EOS550D_H
#define _EOS550D_H

#include <iostream>


///
/// Base class for EOS550D
///
class EOS550D {

 public:

  enum ShutterSpeed { S1000, L1 }; // TBD
  enum Aperture { F8, F16 }; // TBD
  enum Iso { ISO100, ISO200 };

  EOS550D() {}
  virtual ~EOS550D() {}

  virtual EOS550D::Aperture& getAperture( void ) { return aperture_; }
  virtual EOS550D::ShutterSpeed& getShutterSpeed( void ) { return shutterSpeed_; }
  virtual EOS550D::Iso& getIso( void ) { return iso_; }

  virtual bool setAperture( EOS550D::Aperture ) = 0;
  virtual bool setShutterSpeed( EOS550D::ShutterSpeed ) = 0;
  virtual bool setIso( EOS550D::Iso ) = 0;

  virtual bool getAndSaveImage( const std::string& ) = 0;

 private: 

  EOS550D::Aperture aperture_;
  EOS550D::ShutterSpeed shutterSpeed_;
  EOS550D::Iso iso_;

};

#endif
