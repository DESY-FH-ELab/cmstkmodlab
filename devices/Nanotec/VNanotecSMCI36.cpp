#include "VNanotecSMCI36.h"

VNanotecSMCI36::VNanotecSMCI36( const ioport_t ioPort )
{
  stepModeNames_.push_back(std::pair<int,std::string>(smci01MicroStepsPerFullStep, "1 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci02MicroStepsPerFullStep, "2 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci04MicroStepsPerFullStep, "4 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci05MicroStepsPerFullStep, "5 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci08MicroStepsPerFullStep, "8 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci10MicroStepsPerFullStep, "10 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci16MicroStepsPerFullStep, "16 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci32MicroStepsPerFullStep, "32 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smci64MicroStepsPerFullStep, "64 uStep per Full Step"));
  stepModeNames_.push_back(std::pair<int,std::string>(smciFeedRateMode, "Feed Rate Mode"));
  stepModeNames_.push_back(std::pair<int,std::string>(smciAdaptiveStepMode, "Adaptive Step Mode"));

  positioningModeNames_.push_back(std::pair<int,std::string>(smciRelativePositioning, "Relative Positioning"));
  positioningModeNames_.push_back(std::pair<int,std::string>(smciAbsolutePositioning, "Absolute Positioning"));
  positioningModeNames_.push_back(std::pair<int,std::string>(smciExternalRefRun, "External Reference Run"));
}

VNanotecSMCI36::~VNanotecSMCI36()
{

}
