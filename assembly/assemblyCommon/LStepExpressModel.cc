/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <LStepExpressModel.h>
#include <nqlogger.h>

#include <QFileInfo>
#include <QDir>
#include <QStringList>

LStepExpressModel::LStepExpressModel(
  const std::string& port,
  const std::string& lstep_ver,
  const std::string& lstep_iver,
  const int updateInterval,
  const int motionUpdateInterval,
  QObject* /*parent*/
)
 : QObject()
 , port_(port.c_str())
 , lstep_ver_ (lstep_ver.c_str())
 , lstep_iver_(lstep_iver.c_str())
 , updateInterval_(updateInterval)
 , motionUpdateInterval_(motionUpdateInterval)
 , updateCount_(0)
{
    const std::vector<int> allZerosI{ 0, 0, 0, 0 };
    const std::vector<double> allZerosD{ 0.0, 0.0, 0.0, 0.0 };

    axis_ = allZerosI;
    axisDirection_ = allZerosI;
    dim_ = allZerosI;
    pa_ = allZerosI;

    joystickEnabled_ = 0;
    joystickAxisEnabled_ = allZerosI;

    axisStatus_ = std::vector<int>{ 0x0b, 0x0b, 0x0b, 0x0b };
    accelerationJerk_ = allZerosD;
    decelerationJerk_ = allZerosD;
    acceleration_ = allZerosD;
    deceleration_ = allZerosD;
    velocity_ = allZerosD;
    position_ = allZerosD;

    inMotion_ = false;
    isUpdating_ = false;
    isPaused_ = false;
    finishedCalibrating_ = false;

    timer_ = new QTimer(this);
    timer_->setInterval(motionUpdateInterval_);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateMotionInformationFromTimer()));
//    connect(this, SIGNAL(informationChanged()), this, SLOT(updateInformation()));
}

LStepExpressModel::~LStepExpressModel()
{
}

void LStepExpressModel::renewController(const QString& port)
{
  if(controller_ != nullptr){ delete controller_; }

  controller_ = new LStepExpress_t(port.toStdString(), lstep_ver_.toStdString(), lstep_iver_.toStdString());
}

void LStepExpressModel::getStatus(bool& status)
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "getStatus(" << status << ")"
         << ": null pointer to controller, no action taken";

      return;
    }

    QMutexLocker locker(&mutex_);
    status = controller_->GetStatus();
}

void LStepExpressModel::getError(int& error)
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "getError(" << error << ")"
         << ": null pointer to controller, no action taken";

      return;
    }

    QMutexLocker locker(&mutex_);
    error = controller_->GetError();
}

void LStepExpressModel::getSystemStatus(std::string& value)
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "getSystemStatus(" << value << ")"
         << ": null pointer to controller, no action taken";

      return;
    }

    QMutexLocker locker(&mutex_);
    controller_->GetSystemStatusText(value);
}

void LStepExpressModel::pauseUpdate()
{
    NQLog("LStepExpressModel", NQLog::Debug)<< "pauseUpdate";

    QMutexLocker locker(&mutex_);

    isPaused_ = true;

    setControlsEnabled(false);
}

void LStepExpressModel::continueUpdate()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "continueUpdate";

    QMutexLocker locker(&mutex_);

    isPaused_ = false;

    setControlsEnabled(true);
}

QString LStepExpressModel::getAxisName(unsigned int axis)
{
//    if(controller_ == nullptr)
//    {
//      NQLog("LStepExpressModel", NQLog::Critical) << "getAxisName(" << axis << ")"
//         << ": null pointer to controller, no action taken";
//
//      return "";
//    }

    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisName(" << axis << ")";

    QMutexLocker locker(&mutex_);

    QString temp(controller_->GetAxisName((VLStepExpress::Axis)axis));

    return temp;
}

QString LStepExpressModel::getAxisDimensionShortName(unsigned int axis)
{
//    if(controller_ == nullptr)
//    {
//      NQLog("LStepExpressModel", NQLog::Critical) << "getAxisDimensionShortName(" << axis << ")"
//         << ": null pointer to controller, no action taken";
//
//      return "";
//    }

    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisDimensionShortName(" << axis << ")";

    QMutexLocker locker(&mutex_);

    QString temp(controller_->GetAxisDimensionShortName((VLStepExpress::Dimension)dim_[axis]));

    return temp;
}

QString LStepExpressModel::getAxisVelocityShortName(unsigned int axis)
{
//    if(controller_ == nullptr)
//    {
//      NQLog("LStepExpressModel", NQLog::Critical) << "getAxisVelocityShortName(" << axis << ")"
//         << ": null pointer to controller, no action taken";
//
//      return "";
//    }

    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisVelocityShortName(" << axis << ")";

    QMutexLocker locker(&mutex_);

    QString temp(controller_->GetAxisVelocityShortName((VLStepExpress::Dimension)dim_[axis]));

    return temp;
}

QString LStepExpressModel::getAxisAccelerationShortName(unsigned int axis)
{
//    if(controller_ == nullptr)
//    {
//      NQLog("LStepExpressModel", NQLog::Critical) << "getAxisAccelerationShortName(" << axis << ")"
//         << ": null pointer to controller, no action taken";
//
//      return "";
//    }

    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisAccelerationShortName(" << axis << ")";

    QMutexLocker locker(&mutex_);

    QString temp(controller_->GetAxisAccelerationShortName((VLStepExpress::Dimension)dim_[axis]));

    return temp;
}

QString LStepExpressModel::getAxisAccelerationJerkShortName(unsigned int axis)
{
//    if(controller_ == nullptr)
//    {
//      NQLog("LStepExpressModel", NQLog::Critical) << "getAxisAccelerationJerkShortName(" << axis << ")"
//         << ": null pointer to controller, no action taken";
//
//      return "";
//    }

    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisAccelerationJerkShortName(" << axis << ")";

    QMutexLocker locker(&mutex_);

    QString temp(controller_->GetAxisAccelerationJerkShortName((VLStepExpress::Dimension)dim_[axis]));

    return temp;
}

QString LStepExpressModel::getAxisStatusText(unsigned int axis)
{
//    if(controller_ == nullptr)
//    {
//      NQLog("LStepExpressModel", NQLog::Critical) << "getAxisStatusText(" << axis << ")"
//         << ": null pointer to controller, no action taken";
//
//      return "";
//    }

    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisStatusText("<< axis <<")";

    QMutexLocker locker(&mutex_);

    QString temp(controller_->GetAxisStatusText((VLStepExpress::AxisStatus)axisStatus_[axis]));

    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisStatusText(" << axis << ")=" << temp.toStdString();

    return temp;
}

bool LStepExpressModel::getAxisState(unsigned int axis)
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "getAxisState(" << axis << ")"
         << ": null pointer to controller, no action taken";

      return false;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisState(" << axis << ")";

    QMutexLocker locker(&mutex_);

    std::vector<int> ivalues;
    controller_->GetAxisStatus(ivalues);
    axisStatus_[axis] = (ivalues)[axis];

    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisState(" << axis << ")"
       << ", axis_status=" << axisStatus_[axis];

    return (axisStatus_[axis]!=VLStepExpress::AXISDISABLED);
}

bool LStepExpressModel::getAxisEnabled(unsigned int axis)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "getAxisEnabled(" << axis << ")"
       << ", axis_enabled=" << axis_[axis];

    return axis_[axis];
}

double LStepExpressModel::getAccelerationJerk(unsigned int axis)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getAccelerationJerk(" << axis << ")";

  return accelerationJerk_[axis];
}

double LStepExpressModel::getDecelerationJerk(unsigned int axis)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getDecelerationJerk(" << axis << ")";

  return decelerationJerk_[axis];
}

double LStepExpressModel::getAcceleration(unsigned int axis)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getAcceleration(" << axis << ")";

  return acceleration_[axis];
}

double LStepExpressModel::getDeceleration(unsigned int axis)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getDeceleration(" << axis << ")";

  return deceleration_[axis];
}

double LStepExpressModel::getVelocity(unsigned int axis)
{
  NQLog("LStepExpressModel", NQLog::Debug) << "getVelocity(" << axis << ")";

  return velocity_[axis];
}

double LStepExpressModel::getPosition(unsigned int axis)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "getPosition(" << axis << ")";

    return position_[axis];
}

void LStepExpressModel::setAccelerationJerk(const std::vector<double>& values)
{
  if(values.size() != 4)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setAccelerationJerk(values)"
       << ": length of input vector (" << values.size() << ") differs from 4, no action taken";

    return;
  }

  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setAccelerationJerk"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")"
       << ": null pointer to controller, no action taken";

    return;
  }

  NQLog("LStepExpressModel", NQLog::Spam) << "setAccelerationJerk"
     << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")";

  controller_->SetAccelerationJerk(values);
}

void LStepExpressModel::setAccelerationJerk(const double x, const double y, const double z, const double a)
{
  std::vector<double> values{ x, y, z, a };
  this->setAccelerationJerk(values);
}

void LStepExpressModel::setAccelerationJerk(const unsigned int axis, const double value)
{
  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setAccelerationJerk(axis=" << axis << ", value=" << value << ")"
       << ": null pointer to controller, no action taken";

    return;
  }

  if (value==accelerationJerk_[axis]) return;

  NQLog("LStepExpressModel", NQLog::Spam) << "setAccelerationJerk"
     << "(axis="  << axis << ", value=" << value << ", old value=" << accelerationJerk_[axis] << ")";

  controller_->SetAccelerationJerk((VLStepExpress::Axis)axis, value);

  decelerationJerk_[axis] = value;
}

void LStepExpressModel::setDecelerationJerk(const std::vector<double>& values)
{
  if(values.size() != 4)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setDecelerationJerk(values)"
       << ": length of input vector (" << values.size() << ") differs from 4, no action taken";

    return;
  }

  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setDecelerationJerk"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")"
       << ": null pointer to controller, no action taken";

    return;
  }

  NQLog("LStepExpressModel", NQLog::Spam) << "setDecelerationJerk"
     << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")";

  controller_->SetDecelerationJerk(values);
}

void LStepExpressModel::setDecelerationJerk(const double x, const double y, const double z, const double a)
{
  std::vector<double> values{ x, y, z, a };
  this->setDecelerationJerk(values);
}

void LStepExpressModel::setDecelerationJerk(const unsigned int axis, const double value)
{
  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setDecelerationJerk(axis=" << axis << ", value=" << value << ")"
       << ": null pointer to controller, no action taken";

    return;
  }

  if (value==decelerationJerk_[axis]) return;

  NQLog("LStepExpressModel", NQLog::Spam) << "setDecelerationJerk"
     << "(axis="  << axis << ", value=" << value << ", old value=" << decelerationJerk_[axis] << ")";

  controller_->SetDecelerationJerk((VLStepExpress::Axis)axis, value);

  decelerationJerk_[axis] = value;
}

void LStepExpressModel::setAcceleration(const std::vector<double>& values)
{
  if(values.size() != 4)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setAcceleration(values)"
       << ": length of input vector (" << values.size() << ") differs from 4, no action taken";

    return;
  }

  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setAcceleration"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")"
       << ": null pointer to controller, no action taken";

    return;
  }

  NQLog("LStepExpressModel", NQLog::Spam) << "setAcceleration"
     << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")";

  controller_->SetAcceleration(values);
}

void LStepExpressModel::setAcceleration(const double x, const double y, const double z, const double a)
{
  std::vector<double> values{ x, y, z, a };
  this->setAcceleration(values);
}

void LStepExpressModel::setAcceleration(const unsigned int axis, const double value)
{
  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setAcceleration(axis=" << axis << ", value=" << value << ")"
       << ": null pointer to controller, no action taken";

    return;
  }

  if (value==acceleration_[axis]) return;

  NQLog("LStepExpressModel", NQLog::Spam) << "setAcceleration"
     << "(axis="  << axis << ", value=" << value << ", old value=" << acceleration_[axis] << ")";

  controller_->SetAcceleration((VLStepExpress::Axis)axis, value);

  acceleration_[axis] = value;
}

void LStepExpressModel::setDeceleration(const std::vector<double>& values)
{
  if(values.size() != 4)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setDeceleration(values)"
       << ": length of input vector (" << values.size() << ") differs from 4, no action taken";

    return;
  }

  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setDeceleration"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")"
       << ": null pointer to controller, no action taken";

    return;
  }

  NQLog("LStepExpressModel", NQLog::Spam) << "setDeceleration"
     << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")";

  controller_->SetDeceleration(values);
}

void LStepExpressModel::setDeceleration(const double x, const double y, const double z, const double a)
{
  std::vector<double> values{ x, y, z, a };
  this->setDeceleration(values);
}

void LStepExpressModel::setDeceleration(const unsigned int axis, const double value)
{
  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setDeceleration(axis=" << axis << ", value=" << value << ")"
       << ": null pointer to controller, no action taken";

    return;
  }

  if (value==deceleration_[axis]) return;

  NQLog("LStepExpressModel", NQLog::Spam) << "setDeceleration"
     << "(axis="  << axis << ", value=" << value << ", old value=" << deceleration_[axis] << ")";

  controller_->SetDeceleration((VLStepExpress::Axis)axis, value);

  deceleration_[axis] = value;
}

void LStepExpressModel::setVelocity(const std::vector<double>& values)
{
  if(values.size() != 4)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setVelocity(values)"
       << ": length of input vector (" << values.size() << ") differs from 4, no action taken";

    return;
  }

  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setVelocity"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")"
       << ": null pointer to controller, no action taken";

    return;
  }

  NQLog("LStepExpressModel", NQLog::Spam) << "setVelocity"
      << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")";

  controller_->SetVelocity(values);
}

void LStepExpressModel::setVelocity(const double x, const double y, const double z, const double a)
{
  std::vector<double> values{ x, y, z, a };
  this->setVelocity(values);
}

void LStepExpressModel::setVelocity(const unsigned int axis, const double value)
{
  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "setVelocity(axis=" << axis << ", value=" << value << ")"
       << ": null pointer to controller, no action taken";

    return;
  }

  if (value==velocity_[axis]) return;

  NQLog("LStepExpressModel", NQLog::Spam) << "setVelocity"
     << "(axis="  << axis << ", value=" << value << ", old value=" << velocity_[axis] << ")";

  controller_->SetVelocity((VLStepExpress::Axis)axis, value);

  velocity_[axis] = value;
}

void LStepExpressModel::moveRelative(const std::vector<double>& values)
{
  if(values.size() != 4)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "moveRelative(values)"
       << ": length of input vector (" << values.size() << ") differs from 4, no action taken";

    NQLog("LStepExpressModel", NQLog::Spam) << "moveRelative(values)"
       << ": emitting signal \"motionFinished\"";

    emit emergencyStop_request();
    emit motionFinished();
  }

  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "moveRelative"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")"
       << ": null pointer to controller, no action taken";

    NQLog("LStepExpressModel", NQLog::Spam) << "moveRelative"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")"
       << ": emitting signal \"motionFinished\"";

    emit emergencyStop_request();
    emit motionFinished();
  }
  else
  {
    NQLog("LStepExpressModel", NQLog::Spam) << "moveRelative"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")";

    controller_->MoveRelative(values);

    inMotion_ = true;

    NQLog("LStepExpressModel", NQLog::Spam) << "moveRelative"
       << ": emitting signal \"motionStarted\"";

    emit motionStarted();
  }
}

void LStepExpressModel::moveRelative(const double x, const double y, const double z, const double a)
{
  std::vector<double> values{ x, y, z, a };
  this->moveRelative(values);
}

void LStepExpressModel::moveRelative(const unsigned int axis, const double value)
{
  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "moveRelative"
        << "(axis="  << axis << ", value=" << value << ")"
        << ": null pointer to controller, no action taken";

    NQLog("LStepExpressModel", NQLog::Spam) << "moveRelative"
        << "(axis="  << axis << ", value=" << value << ")"
        << ": emitting signal \"motionFinished\"";

    emit emergencyStop_request();
    emit motionFinished();
  }
  else
  {
    NQLog("LStepExpressModel", NQLog::Spam) << "moveRelative"
        << "(axis="  << axis << ", value=" << value << ")";

    controller_->MoveRelative((VLStepExpress::Axis)axis, value);

    inMotion_ = true;

    NQLog("LStepExpressModel", NQLog::Spam) << "moveRelative"
       << ": emitting signal \"motionStarted\"";

    emit motionStarted();
  }
}

void LStepExpressModel::moveAbsolute(const std::vector<double>& values)
{
  if(values.size() != 4)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "moveAbsolute(values)"
       << ": length of input vector (" << values.size() << ") differs from 4, no action taken";

    NQLog("LStepExpressModel", NQLog::Spam) << "moveAbsolute(values)"
       << ": emitting signal \"motionFinished\"";

    emit emergencyStop_request();
    emit motionFinished();
  }

  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "moveAbsolute"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")"
       << ": null pointer to controller, no action taken";

    NQLog("LStepExpressModel", NQLog::Spam) << "moveAbsolute"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")"
       << ": emitting signal \"motionFinished\"";

    emit emergencyStop_request();
    emit motionFinished();
  }
  else
  {
    NQLog("LStepExpressModel", NQLog::Spam) << "moveAbsolute"
       << "(x=" << values[0] << ", y=" << values[1] << ", z=" << values[2] << ", a=" << values[3] << ")";

    controller_->MoveAbsolute(values);

    inMotion_ = true;

    NQLog("LStepExpressModel", NQLog::Spam) << "moveAbsolute"
       << ": emitting signal \"motionStarted\"";

    emit motionStarted();
  }
}

void LStepExpressModel::moveAbsolute(const double x, const double y, const double z, const double a)
{
  std::vector<double> values{ x, y, z, a };
  this->moveAbsolute(values);
}

void LStepExpressModel::moveAbsolute(const unsigned int axis, const double value)
{
  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "moveAbsolute"
        << "(axis="  << axis << ", value=" << value << ")"
        << ": null pointer to controller, no action taken";

    NQLog("LStepExpressModel", NQLog::Spam) << "moveAbsolute"
        << "(axis="  << axis << ", value=" << value << ")"
        << ": emitting signal \"motionFinished\"";

    emit emergencyStop_request();
    emit motionFinished();
  }
  else
  {
    NQLog("LStepExpressModel", NQLog::Spam) << "moveAbsolute"
        << "(axis="  << axis << ", value=" << value << ")";

    controller_->MoveAbsolute((VLStepExpress::Axis) axis, value);

    inMotion_ = true;

    NQLog("LStepExpressModel", NQLog::Spam) << "moveAbsolute"
       << ": emitting signal \"motionStarted\"";

    emit motionStarted();
  }
}

void LStepExpressModel::errorQuit()
{
  if(controller_ == nullptr)
  {
    NQLog("LStepExpressModel", NQLog::Critical) << "errorQuit"
       << ": null pointer to controller, no action taken";

    return;
  }

  NQLog("LStepExpressModel", NQLog::Spam) << "errorQuit";

  controller_->ErrorQuit();
}

void LStepExpressModel::calibrate()
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "calibrate"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Spam) << "calibrate";

    controller_->Calibrate();

    inMotion_ = true;

    finishedCalibrating_ = true;

    NQLog("LStepExpressModel", NQLog::Spam) << "calibrate"
       << ": emitting signal \"motionStarted\"";

    emit motionStarted();
}

void LStepExpressModel::emergencyStop()
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "emergencyStop"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Spam) << "emergencyStop";

    controller_->EmergencyStop();

    inMotion_ = false;

    finishedCalibrating_ = false;

    NQLog("LStepExpressModel", NQLog::Spam) << "emergencyStop"
       << ": emitting signal \"emergencyStop_request\"";

    emit emergencyStop_request();

    NQLog("LStepExpressModel", NQLog::Spam) << "emergencyStop"
       << ": emitting signal \"motionFinished\"";

    emit motionFinished();
}

bool LStepExpressModel::getJoystickEnabled()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "getJoystickEnabled";

    return (joystickEnabled_==1);
}

bool LStepExpressModel::getJoystickAxisEnabled(unsigned int axis)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "getJoystickAxisEnabled(" << axis << ")";

    return (joystickAxisEnabled_[axis]==1);
}

void LStepExpressModel::setAxisEnabled(unsigned int axis, bool enabled)
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "setAxisEnabled(" << axis << ", " << enabled << ")"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "setAxisEnabled(" << axis << ", " << enabled << ")";

    int temp = (int)enabled;

    int temp2;
    controller_->GetAxisEnabled((VLStepExpress::Axis)axis, temp2);
    axis_[axis] = temp2;

    if(axis_[axis] != temp)
    {
        controller_->SetPowerAmplifierStatus((VLStepExpress::Axis)axis, temp);
        controller_->SetAxisEnabled((VLStepExpress::Axis)axis, temp);
        axis_[axis] = temp;
        updateInformation();

        NQLog("LStepExpressModel", NQLog::Debug) << "setAxisEnabled(" << axis << ", " << enabled << ")"
           << ": emitting signal \"informationChanged\"";

        emit informationChanged();
    }
}

void LStepExpressModel::setJoystickEnabled(bool enabled)
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "setJoystickEnabled(" << enabled << ")"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "setJoystickEnabled(" << enabled << ")";

    int temp = (int)enabled;

    if(joystickEnabled_ != temp)
    {
      std::vector<int> ivalues = {0,0,0,0};

      if(temp == 0){controller_->SetJoystickAxisEnabled(ivalues); joystickAxisEnabled_ = ivalues;}

      controller_->SetJoystickEnabled(temp);

      int temp2 = (int)controller_->GetJoystickEnabled();

      joystickEnabled_ = temp;

      if(temp2 == 1){controller_->SetJoystickAxisEnabled(ivalues); joystickAxisEnabled_ = ivalues;}

      updateInformation();

      NQLog("LStepExpressModel", NQLog::Debug) << "setJoystickEnabled(" << enabled << ")"
         << ": emitting signal \"informationChanged\"";

      emit informationChanged();
    }
}

void LStepExpressModel::setJoystickAxisEnabled(unsigned int axis, bool enabled)
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "setJoystickAxisEnabled(" << axis << ", " << enabled << ")"
         << ": null pointer to controller, no action taken";

      return;
    }

    int temp = (int)enabled;

    NQLog("LStepExpressModel", NQLog::Debug) << "setJoystickAxisEnabled(" << axis << ", " << enabled << ")"
       << ", joystick_enabled=" << joystickEnabled_;

    if(joystickEnabled_ && joystickAxisEnabled_[axis] != temp)
    {
      controller_->SetJoystickAxisEnabled((VLStepExpress::Axis)axis, temp);
      joystickAxisEnabled_[axis] = temp;
    }
}

void LStepExpressModel::setValue(const QString & command, const QString & value)
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "setValue(" << command << ", " << value << ")"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "setValue(" << command << ", " << value << ")";

    std::string temp;
    controller_->SetValue(command.toStdString(), value.toStdString());
}

void LStepExpressModel::getValue(const QString & command, QString & value)
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "getValue(" << command << ", " << value << ")"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "setValue(" << command << ", " << value << ")";

    std::string temp;
    controller_->GetValue(command.toStdString(), temp);
    value = temp.c_str();
}

void LStepExpressModel::validConfig()
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "validConfig"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "validConfig";

    controller_->ValidConfig();
}

void LStepExpressModel::validParameter()
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "validParameter"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "validParameter";

    controller_->ValidParameter();
}

void LStepExpressModel::saveConfig()
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "saveConfig"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "saveConfig";

    controller_->SaveConfig();
}

void LStepExpressModel::reset()
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "reset"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "reset";

    controller_->Reset();
}

void LStepExpressModel::initialize()
{
    NQLog("LStepExpressModel", NQLog::Debug) << "initialize";

    setDeviceState(INITIALIZING);

    bool enabled(false);

    if(port_.isEmpty())
    {
      NQLog("LStepExpressModel", NQLog::Warning) << "initialize"
         << ": path to device file is empty, LStepExpressModel will not be initialized";
    }
    else
    {
      // browse ports, and choose the first one for which
      // LStep controller is successfully enabled
      const QFileInfo port_qfileinfo(port_);

      QDir portDir(port_qfileinfo.dir().canonicalPath());
      portDir.setNameFilters(QStringList(port_qfileinfo.fileName()));
      portDir.setFilter(QDir::System);

      const QStringList ports_list = portDir.entryList();

      for(const auto& port : ports_list)
      {
        renewController(portDir.canonicalPath()+"/"+port);

        enabled = (controller_ != nullptr) && (controller_->DeviceAvailable());

        if(enabled){ break; }
      }
    }

    if(enabled)
    {
      NQLog("LStepExpressModel", NQLog::Message) << "initialize"
         << ": successfully accessed port " << controller_->ioPort();

      QMutexLocker locker(&mutex_);

      controller_->SetAutoStatus(2);

      std::vector<int> allZerosI{ 0, 0, 0, 0 };
      std::vector<int> OnI{1,1,1,1};

      controller_->SetPowerAmplifierStatus(allZerosI);
      controller_->SetAxisEnabled(allZerosI);
      controller_->SetJoystickEnabled(false);

      axis_ = allZerosI;
      joystickEnabled_ = false;
      joystickAxisEnabled_ = allZerosI;

      updateInformation();

      setDeviceState(READY);
    }
    else
    {
      NQLog("LStepExpressModel", NQLog::Message) << "initialize"
         << ": failed to initialize LStepExpressModel (no valid device file found under path " << port_ << ")";

      setDeviceState( OFF );
      delete controller_;
      controller_ = nullptr;
    }
}

void LStepExpressModel::setDeviceState(State state)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "setDeviceState";

    if(state_ != state)
    {
      state_ = state;

      if(state_ == READY){ timer_->start(); }
      else               { timer_->stop (); }

      NQLog("LStepExpressModel", NQLog::Debug) << "setDeviceState"
         << ": emitting signal \"deviceStateChanged\"";

      emit deviceStateChanged(state);
    }
}

void LStepExpressModel::updateInformation()
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Debug) << "updateInformation"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "updateInformation";

    /*
      if (thread()==QApplication::instance()->thread()) {
      NQLog("LStepExpressModel"<< " running in main application thread";
      } else {
      NQLog("LStepExpressModel"<< " running in dedicated thread";
      }
    */

    bool changed = false;
    bool positionChanged = false;

    std::vector<int> ivalues;
    std::vector<double> dvalues;

    controller_->GetAxisEnabled(ivalues);
    if (ivalues!=axis_) {
        axis_ = ivalues;
        changed = true;
    }

    controller_->GetDimension(ivalues);
    if (ivalues!=dim_) {
        dim_ = ivalues;
        changed = true;
    }

    controller_->GetAcceleration(dvalues);
    if (dvalues!=acceleration_) {
      acceleration_ = dvalues;
      changed = true;
    }

    controller_->GetDeceleration(dvalues);
    if (dvalues!=deceleration_) {
      deceleration_ = dvalues;
      changed = true;
    }

    controller_->GetAccelerationJerk(dvalues);
    if (dvalues!=accelerationJerk_) {
      accelerationJerk_ = dvalues;
      changed = true;
    }

    controller_->GetDecelerationJerk(dvalues);
    if (dvalues!=decelerationJerk_) {
      decelerationJerk_ = dvalues;
      changed = true;
    }

    controller_->GetVelocity(dvalues);
    if (dvalues!=velocity_) {
      velocity_ = dvalues;
      changed = true;
    }

    controller_->GetPosition(dvalues);
    if (dvalues!=position_) {
      position_ = dvalues;
      positionChanged = true;
    }

    int joystick = controller_->GetJoystickEnabled();
    // int joystick_model = this->getJoystickEnabled();
    if (joystick!=joystickEnabled_) {
        joystickEnabled_ = joystick;
        changed = true;
    }

    if(joystickEnabled_){
      controller_->GetJoystickAxisEnabled(ivalues);
      if (ivalues!=joystickAxisEnabled_) {
        joystickAxisEnabled_ = ivalues;
        changed = true;
      }
    }

    if(changed)
    {
        NQLog("LStepExpressModel", NQLog::Debug) << "updateInformation"
           << ": emitting signal \"informationChanged\"";

        emit informationChanged();
    }

    if(positionChanged)
    {
        NQLog("LStepExpressModel", NQLog::Debug) << "updateInformation"
           << ": emitting signal \"motionInformationChanged\"";

        emit motionInformationChanged();
    }
}

void LStepExpressModel::updateMotionInformation()
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation";

    static const int nUpdates = updateInterval_/motionUpdateInterval_;

    if ( state_ == READY && !isPaused_) {

      isUpdating_ = true;

      updateCount_++;
      if (updateCount_==nUpdates) {
        updateInformation();
        updateCount_ = 0;
      }

      /*
        if (thread()==QApplication::instance()->thread()) {
        NQLog("LStepExpressModel"<< " running in main application thread";
        } else {
        NQLog("LStepExpressModel"<< " running in dedicated thread";
        }
      */

      bool changed = false;

      std::vector<int> ivalues;
      std::vector<int> pavalues;
      std::vector<int> evalues;
      std::vector<double> dvalues;

      controller_->GetAxisStatus(ivalues);
      controller_->GetPowerAmplifierStatus(pavalues);
      controller_->GetAxisEnabled(evalues);

      if (ivalues!=axisStatus_) {
        axisStatus_ = ivalues;
        changed = true;
      }

      if (inMotion_) {
        bool temp = true;
        for(int i = 0; i < 4; i++){
          bool ifaxisenabledandready = ( (ivalues)[i] == LStepExpress_t::AXISSTANDSANDREADY || (ivalues)[i] == LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i] == 1;
          bool ifaxisnotenabled = (axis_)[i] == 0;
          temp *= ifaxisenabledandready || ifaxisnotenabled;
          //	    NQLog("LStepExpressModel", NQLog::Spam) <<" axis status =  "<<(ivalues)[i]<<" axis enabled = "<<(axis_)[i]<<" temp = "<<temp;
        }
        if(temp)
        {
          inMotion_ = false;

          NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation"
              << ": emitting signal \"motionFinished\"";

          emit motionFinished();
        }

          /*
	if (std::all_of(ivalues.begin(), ivalues.end(),
	[](int i){	  NQLog("LStepExpressModel", NQLog::Spam)<< "updateMotionInformation() axis status =  "<<i; bool temp = (i==LStepExpress_t::AXISSTANDSANDREADY || i==LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i]==1; return temp;})) {
	NQLog("LStepExpressModel", NQLog::Spam)<< "updateMotionInformation() finished moving inMotion_ = false";
	inMotion_ = false;
	emit motionFinished();
	}*/
      }

      if( (axis_)[0] || (axis_)[1] || (axis_)[2] || (axis_)[3]){
        controller_->GetPosition(dvalues);
        if (dvalues!=position_) {
          //NQLog("LStepExpressModel", NQLog::Spam)<< "updateMotionInformation() new position values"  ;
          position_ = dvalues;
          changed = true;
        }
      }

      //      NQLog("LStepExpressModel", NQLog::Spam)<< "updateMotionInformation() finishedCalibrating_ =  "<<finishedCalibrating_<<" in motion =  "<<inMotion_  ;
      if(!inMotion_ && finishedCalibrating_){
        //  NQLog("LStepExpressModel", NQLog::Spam)<< "updateMotionInformation() after calibration, setting position values to zero"  ;
        std::vector<double> posvalues{0.0, 0.0, 0.0, 0.0};
        controller_->SetPosition(posvalues);
        position_ = posvalues;
        changed = true;
        finishedCalibrating_ = false;
      }

      if(changed)
      {
        NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformation"
            << ": emitting signal \"motionInformationChanged\"";

        emit motionInformationChanged();
      }

      isUpdating_ = false;
    }
}

void LStepExpressModel::updateMotionInformationFromTimer()
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformationFromTimer"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformationFromTimer";

    static const int nUpdates = updateInterval_/motionUpdateInterval_;

    if((state_ == READY) && (isPaused_ == false))
    {
      isUpdating_ = true;

      updateCount_++;
      if (updateCount_==nUpdates) {
        updateInformation();
        updateCount_ = 0;
      }

      /*
        if (thread()==QApplication::instance()->thread()) {
        NQLog("LStepExpressModel"<< " running in main application thread";
        } else {
        NQLog("LStepExpressModel"<< " running in dedicated thread";
        }
      */

      bool changed = false;

      std::vector<int> ivalues;
      std::vector<int> pavalues;
      std::vector<int> evalues;
      std::vector<double> dvalues;

      controller_->GetAxisStatus(ivalues);
      controller_->GetPowerAmplifierStatus(pavalues);
      controller_->GetAxisEnabled(evalues);

      if (ivalues!=axisStatus_) {
        axisStatus_ = ivalues;
        changed = true;
      }

      if(inMotion_)
      {
        bool temp = true;
        for(int i = 0; i < 4; i++)
        {
          bool ifaxisenabled = ( (ivalues)[i] == LStepExpress_t::AXISSTANDSANDREADY || (ivalues)[i] == LStepExpress_t::AXISACKAFTERCALIBRATION) && (axis_)[i] == 1;
          bool ifaxisnotenabled = (axis_)[i] == 0;
          temp *= (ifaxisenabled || ifaxisnotenabled);
        }
        if(temp)
        {
          inMotion_ = false;

          NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformationFromTimer"
              << ": emitting signal \"motionFinished\"";

          emit motionFinished();
        }
      }

      if((axis_)[0] || (axis_)[1] || (axis_)[2] || (axis_)[3])
      {
        controller_->GetPosition(dvalues);
        if (dvalues!=position_) {
          position_ = dvalues;
          changed = true;
        }
      }

      if(!inMotion_ && finishedCalibrating_)
      {
        std::vector<double> posvalues{0.0, 0.0, 0.0, 0.0};
        controller_->SetPosition(posvalues);
        position_ = posvalues;
        changed = true;
        finishedCalibrating_ = false;
      }

      if(changed)
      {
        NQLog("LStepExpressModel", NQLog::Debug) << "updateMotionInformationFromTimer"
            << ": emitting signal \"motionInformationChanged\"";

        emit motionInformationChanged();
      }

      isUpdating_ = false;
    }
}

void LStepExpressModel::setDeviceEnabled(bool enabled)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "setDeviceEnabled(" << enabled << ")";

    if(state_ == READY && !enabled)
    {
      std::vector<int> allZeros{ 0, 0, 0, 0 };
      controller_->SetPowerAmplifierStatus(allZeros);
      controller_->SetAxisEnabled(allZeros);
    }

    AbstractDeviceModel<LStepExpress_t>::setDeviceEnabled(enabled);
}

void LStepExpressModel::setControlsEnabled(bool enabled)
{
    NQLog("LStepExpressModel", NQLog::Debug) << "setControlsEnabled(" << enabled << ")"
       << ": emitting signal \"controlStateChanged(" << enabled << ")\"";

    emit controlStateChanged(enabled);
}

void LStepExpressModel::setPositionControllerEnabled(const bool enable)
{
    if(controller_ == nullptr)
    {
      NQLog("LStepExpressModel", NQLog::Critical) << "setPositionControllerEnabled(" << enable << ")"
         << ": null pointer to controller, no action taken";

      return;
    }

    NQLog("LStepExpressModel", NQLog::Debug) << "setPositionControllerEnabled(" << enable << ")";

    controller_->SetPositionControllerEnabled(enable);

    NQLog("LStepExpressModel", NQLog::Message) << "setPositionControllerEnabled(" << enable << ")"
       << ": Position Controller switched " << (controller_->GetPositionControllerEnabled() ? "ON" : "OFF");
}
