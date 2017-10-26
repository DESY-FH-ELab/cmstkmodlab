#include <Log.h>

#include <iostream>
#include <stdexcept>

int Log::verbosity = 1;

void Log::KILL(const std::string& log_){

  throw std::runtime_error(log_);

  return;
}

void Log::WARNING(const std::string& log_){

  std::cerr << "@@@ WARNING -- "+log_+"\n";

  return;
}

Log::NQLog2::NQLog2(const QString& module, const LogLevel level) :
  module_(module),
  level_(level),
  stream_(&buffer_)
{
}

Log::NQLog2::~NQLog2()
{
  if((level_ - NQLog2::Fatal + Log::verbosity) > 0)
  {
    NQLogger::instance()->write(module_, NQLog::LogLevel(level_), buffer_);
  }
}
