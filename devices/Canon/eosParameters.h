
  /// WARNING!
  /// THESE DEFS ARE REPEATED IN THE DEFO CODE
  /// (DefoMainWindow.h)
  /// if you change them here, do it also there!


  // all possible speeds, correct order, so we can call them by index;
  // last entry must be SINVALID!
  enum ShutterSpeed { 
    BULB, L30, L25, L20, L15, L13, L10, L8, L6, L5, L4, L3_2, L2_5, L2, 
    L1_6, L1_3, L1, L0_8, L0_6, L0_5, L0_4, L0_3, S4, S5, S6, S8, S10, 
    S13, S15, S20, S25, S30, S40, S50, S60, S80, S100, S125, S160, S200, 
    S250, S320, S400, S500, S640, S800, S1000, S1250, S1600, S2000, S2500, 
    S3200, S4000, SINVALID 
  };
  
  
  // all possible apertures (for current lens);
  // correct order, so we can call them by index;
  // last entry must be FINVALID!
  enum Aperture { 
    F5_6, F6_3, F7_1, F8, F9, F10, F11, F13, F14, F16, F18, F20, F22, F25,
    F29, F32, F36, FINVALID
  };
  
  
  // all possible iso values, correct order, so we can call them by index;
  // last entry must be ISOINVALID!
  enum Iso {
    ISOAUTO, ISO100, ISO200, ISO400, ISO800, ISO1600, ISO3200, ISO6400,
    ISOINVALID
  };
