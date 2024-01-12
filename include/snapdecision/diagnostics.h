#pragma once

#include <functional>
#include <string>

#include "snapdecision/enums.h"


using DiagnosticFunction = std::function<void(LogLevel level, const std::string& message)>;

DiagnosticFunction makeDefaultDiagnosticFunction();
