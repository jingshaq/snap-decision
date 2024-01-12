#include "snapdecision/diagnostics.h"
#include <iostream>

DiagnosticFunction makeDefaultDiagnosticFunction()
{
  return [](LogLevel level, const std::string& message)
  {
    switch (level)
    {
      case LogLevel::Info:
        std::clog << "[INFO] " << message << "\n";
        return;
      case LogLevel::Warn:
        std::clog << "[WARN] " << message << "\n";
        return;
      case LogLevel::Error:
        std::clog << "[ERROR] " << message << "\n";
        return;
    }
  };
}
