

#ifndef _EOS550D_H
#define _EOS550D_H

#include <iostream>


///
/// Base class for EOS550D
///
class EOS550D {

 public:

  // all possible speeds, correct order, so we can call them by index
  enum ShutterSpeed { BULB, L30, L25, L20, L15, L13, L10, L8, L6, L5, L4, L3_2, L2_5, L2, 
		      L1_6, L1_3, L1, L0_8, L0_6, L0_5, L0_4, L0_3, S4, S5, S6, S8, S10, 
		      S13, S15, S20, S25, S30, S40, S50, S60, S80, S100, S125, S160, S200, 
		      S250, S320, S400, S500, S640, S800, S1000, S1250, S1600, S2000, S2500, 
		      S3200, S4000, SINVALID };


  // all possible apertures (for current lens);
  // correct order, so we can call them by index
  enum Aperture { F5_6, F6_3, F7_1, F8, F9, F10, F11, F13, F14, F16, F18, F20, F22, F25, F29, F32, F36, FINVALID };


  // all possible iso values, correct order, so we can call them by index
  enum Iso { ISOAUTO, ISO100, ISO200, ISO400, ISO800, ISO1600, ISO3200, ISO6400, ISOINVALID };


  class EOS550DConfig {
  public:
    EOS550DConfig() {}
    ~EOS550DConfig() {}
    EOS550D::Aperture aperture_;
    EOS550D::ShutterSpeed shutterSpeed_;
    EOS550D::Iso iso_;
  };


  EOS550D() {}
  virtual ~EOS550D() {}

  virtual EOS550D::Aperture const& getAperture( void ) { return cfg_.aperture_; }
  virtual EOS550D::ShutterSpeed const& getShutterSpeed( void ) { return cfg_.shutterSpeed_; }
  virtual EOS550D::Iso const& getIso( void ) { return cfg_.iso_; }
  virtual EOS550D::EOS550DConfig const& getConfig( void ) { return cfg_; }

  virtual EOS550D::Aperture const& readAperture( void ) = 0;
  virtual EOS550D::ShutterSpeed const& readShutterSpeed( void ) = 0;
  virtual EOS550D::Iso const& readIso( void  ) = 0;
  virtual EOS550D::EOS550DConfig const& readConfig( void ) = 0;

  virtual bool writeAperture( EOS550D::Aperture ) = 0;
  virtual bool writeShutterSpeed( EOS550D::ShutterSpeed ) = 0;
  virtual bool writeIso( EOS550D::Iso ) = 0;
  virtual bool writeConfig( EOS550DConfig const& ) = 0;

  virtual bool getAndSaveImage( const std::string& ) = 0;

 protected: 

  EOS550D::EOS550DConfig cfg_;

};

#endif
