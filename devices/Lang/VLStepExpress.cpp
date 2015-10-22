#include <sstream>

#include "VLStepExpress.h"

VLStepExpress::VLStepExpress( const ioport_t ioPort )
{

}

VLStepExpress::~VLStepExpress()
{

}

void VLStepExpress::SetValue(const std::string & command,
                             const std::string & value)
{
  std::ostringstream os;
  os << command << " " << value;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             VLStepExpress::Axis axis, const std::string & value)
{
  std::ostringstream os;
  os << command << " " << (char)axis << " " << value;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             int value1)
{
  std::ostringstream os;
  os << command << " " << value1;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             int value1, int value2)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             int value1, int value2, int value3)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             int value1, int value2, int value3, int value4)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3 << " " << value4;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             VLStepExpress::Axis axis, int value)
{
  std::ostringstream os;
  os << command << " " << (char)axis << " " << value;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             double value1)
{
  std::ostringstream os;
  os << command << " " << value1;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             double value1, double value2)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             double value1, double value2, double value3)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             double value1, double value2, double value3, double value4)
{
  std::ostringstream os;
  os << command << " " << value1 << " " << value2 << " " << value3 << " " << value4;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             VLStepExpress::Axis axis, double value)
{
  std::ostringstream os;
  os << command << " " << (char)axis << " " << value;
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             const std::vector<int> & values)
{
  std::ostringstream os;
  os << command;
  for (std::vector<int>::const_iterator it = values.begin();
       it!=values.end();
       ++it) {
    os << " " << *it;
  }
  this->SendCommand(os.str());
}

void VLStepExpress::SetValue(const std::string & command,
                             const std::vector<double> & values)
{
  std::ostringstream os;
  os << command;
  for (std::vector<double>::const_iterator it = values.begin();
       it!=values.end();
       ++it) {
    os << " " << *it;
  }
  this->SendCommand(os.str());
}

void VLStepExpress::GetValue(const std::string & command,
                             std::string & value)
{
  this->SendCommand(command);
  this->ReceiveString(value);
}

void VLStepExpress::GetValue(const std::string & command, VLStepExpress::Axis axis,
                             std::string & value)
{
  std::ostringstream os;
  os << command << " " << (char)axis;
  this->SendCommand(os.str());
  this->ReceiveString(value);
}

void VLStepExpress::GetValue(const std::string & command,
                             std::vector<int> & values)
{
  this->SendCommand(command);

  std::string buffer;
  int temp;
  this->ReceiveString(buffer);

  values.clear();
  std::istringstream is(buffer);
  while (is >> temp) {
    values.push_back(temp);
  }
}

void VLStepExpress::GetValue(const std::string & command,
                             VLStepExpress::Axis axis, int & value)
{
  std::ostringstream os;
  os << command << " " << (char)axis;
  this->SendCommand(os.str());

  std::string buffer;
  int temp;
  this->ReceiveString(buffer);
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}

void VLStepExpress::GetValue(const std::string & command,
                             std::vector<double> & values)
{
  this->SendCommand(command);

  std::string buffer;
  double temp;
  this->ReceiveString(buffer);

  values.clear();
  std::istringstream is(buffer);
  while (is >> temp) {
    values.push_back(temp);
  }
}

void VLStepExpress::GetValue(const std::string & command, VLStepExpress::Axis axis,
                             double & value)
{
  std::ostringstream os;
  os << command << " " << (char)axis;
  this->SendCommand(os.str());

  std::string buffer;
  double temp;
  this->ReceiveString(buffer);
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}
