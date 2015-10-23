#include <iostream>

#include "LStepExpressFake.h"

LStepExpressFake::LStepExpressFake( const ioport_t ioPort )
  :VLStepExpress(ioPort),
   autoStatus_(1)
{
  axisStatus_ = std::vector<int>{ VLStepExpress::AXISREADY,
                                  VLStepExpress::AXISREADY,
                                  VLStepExpress::AXISREADY,
                                  VLStepExpress::AXISREADY };
  axis_ = std::vector<int>{ 1, 1, 1, 1 };
  axisDirection_ = std::vector<int>{ 0, 0, 0, 0 };
  dim_ = std::vector<int>{ 2, 2, 2, 3 };
  pa_ = std::vector<int>{ 0, 0, 0, 0 };
  position_ = std::vector<double>{ 0.0, 0.0, 0.0, 0.0 };
  moverel_ = std::vector<double>{ 0.0, 0.0, 0.0, 0.0 };
}

LStepExpressFake::~LStepExpressFake()
{

}

void LStepExpressFake::GetAutoStatus(int & value)
{
  value = autoStatus_;
}

void LStepExpressFake::SetAutoStatus(int value)
{
  autoStatus_ = value;
}

void LStepExpressFake::GetAxisStatus(std::vector<int> & values)
{
  values = axis_;
}

void LStepExpressFake::GetAxisEnabled(std::vector<int> & values)
{
  values = axis_;
}

void LStepExpressFake::GetAxisEnabled(VLStepExpress::Axis axis, int & value)
{
  value = axis_[axis];
}

void LStepExpressFake::SetAxisEnabled(std::vector<int> & values)
{
  axis_ = values;
}

void LStepExpressFake::SetAxisEnabled(VLStepExpress::Axis axis, int value)
{
  axis_[axis] = value;
}

void LStepExpressFake::GetAxisDirection(std::vector<int> & values)
{
  values = axisDirection_;
}

void LStepExpressFake::GetAxisDirection(VLStepExpress::Axis axis, int & value)
{
  value = axisDirection_[axis];
}

void LStepExpressFake::SetAxisDirection(std::vector<int> & values)
{
  axisDirection_ = values;
}

void LStepExpressFake::SetAxisDirection(VLStepExpress::Axis axis, int value)
{
  axisDirection_[axis] = value;
}

void LStepExpressFake::GetDimension(std::vector<int> & values)
{
  values = dim_;
}

void LStepExpressFake::GetDimension(VLStepExpress::Axis axis, int & value)
{
  value = dim_[axis];
}

void LStepExpressFake::SetDimension(std::vector<int> & values)
{
  dim_ = values;
}

void LStepExpressFake::SetDimension(VLStepExpress::Axis axis, int value)
{
  dim_[axis] = value;
}

void LStepExpressFake::GetPowerAmplifierStatus(std::vector<int> & values)
{
  values = pa_;
}

void LStepExpressFake::GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value)
{
  value = pa_[axis];
}

void LStepExpressFake::SetPowerAmplifierStatus(std::vector<int> & values)
{
  pa_ = values;
}

void LStepExpressFake::SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value)
{
  pa_[axis] = value;
}

void LStepExpressFake::GetPosition(std::vector<double> & values)
{
  values = position_;
}

void LStepExpressFake::GetPosition(VLStepExpress::Axis axis, double & value)
{
  value = position_[axis];
}

void LStepExpressFake::SetPosition(std::vector<double> & values)
{
  position_ = values;
}

void LStepExpressFake::SetPosition(VLStepExpress::Axis axis, double value)
{
  position_[axis] = value;
}

void LStepExpressFake::MoveAbsolute(std::vector<double> & values)
{
  position_ = values;
}

void LStepExpressFake::MoveAbsolute(double x, double y, double z, double a)
{
  position_[VLStepExpress::X] = x;
  position_[VLStepExpress::Y] = y;
  position_[VLStepExpress::Z] = z;
  position_[VLStepExpress::A] = a;
}

void LStepExpressFake::MoveAbsolute(VLStepExpress::Axis axis, double value)
{
  position_[axis] = value;
}

void LStepExpressFake::MoveRelative(std::vector<double> & values)
{
  moverel_ = values;
  std::vector<double>::iterator itpos = position_.begin();
  for (std::vector<double>::iterator it = moverel_.begin();
      it!=moverel_.end();
      ++it, ++itpos) {
      *itpos += *it;
  }
}

void LStepExpressFake::MoveRelative(double x, double y, double z, double a)
{
  moverel_[VLStepExpress::X] = x;
  moverel_[VLStepExpress::Y] = y;
  moverel_[VLStepExpress::Z] = z;
  moverel_[VLStepExpress::A] = a;
  std::vector<double>::iterator itpos = position_.begin();
  for (std::vector<double>::iterator it = moverel_.begin();
      it!=moverel_.end();
      ++it, ++itpos) {
      *itpos += *it;
  }
}

void LStepExpressFake::MoveRelative(VLStepExpress::Axis axis, double value)
{
  moverel_[VLStepExpress::X] = 0.0;
  moverel_[VLStepExpress::Y] = 0.0;
  moverel_[VLStepExpress::Z] = 0.0;
  moverel_[VLStepExpress::A] = 0.0;
  moverel_[axis] = value;
  std::vector<double>::iterator itpos = position_.begin();
  for (std::vector<double>::iterator it = moverel_.begin();
       it!=moverel_.end();
       ++it, ++itpos) {
    *itpos += *it;
  }
}

void LStepExpressFake::MoveRelative()
{
  std::vector<double>::iterator itpos = position_.begin();
  for (std::vector<double>::iterator it = moverel_.begin();
      it!=moverel_.end();
      ++it, ++itpos) {
      *itpos += *it;
  }
}

void GetSystemStatus(std::vector<int>& values)
{
  values.resize(4, 5);
}

void GetSystemStatus(VLStepExpress::Axis axis, int & value)
{
  value = 5;
}

void LStepExpressFake::SendCommand(const std::string & command)
{
  std::cout << "SendCommand: " << command << std::endl;
}
