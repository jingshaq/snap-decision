#include "snapdecision/enums.h"

std::string to_string(MeteringMode mode)
{
  switch (mode)
  {
    case MeteringMode::Unknown:
      return "unknown";
    case MeteringMode::Average:
      return "average";
    case MeteringMode::CenterWeightedAverage:
      return "center weighted average";
    case MeteringMode::Spot:
      return "spot";
    case MeteringMode::MultiSpot:
      return "multi-spot";
    case MeteringMode::Pattern:
      return "pattern";
    case MeteringMode::Partial:
      return "partial";
    default:
      return "unknown";
  }
}

MeteringMode to_MeteringMode(const std::string& str)
{
  if (str == "unknown")
    return MeteringMode::Unknown;
  if (str == "average")
    return MeteringMode::Average;
  if (str == "center weighted average")
    return MeteringMode::CenterWeightedAverage;
  if (str == "spot")
    return MeteringMode::Spot;
  if (str == "multi-spot")
    return MeteringMode::MultiSpot;
  if (str == "pattern")
    return MeteringMode::Pattern;
  if (str == "partial")
    return MeteringMode::Partial;
  return MeteringMode::Unknown;
}

std::string to_string(ExposureProgram program)
{
  switch (program)
  {
    case ExposureProgram::NotDefined:
      return "not defined";
    case ExposureProgram::Manual:
      return "manual";
    case ExposureProgram::NormalProgram:
      return "normal program";
    case ExposureProgram::AperturePriority:
      return "aperture priority";
    case ExposureProgram::ShutterPriority:
      return "shutter priority";
    case ExposureProgram::CreativeProgram:
      return "creative program";
    case ExposureProgram::ActionProgram:
      return "action program";
    case ExposureProgram::PortraitMode:
      return "portrait mode";
    case ExposureProgram::LandscapeMode:
      return "landscape mode";
    default:
      return "not defined";
  }
}

ExposureProgram to_ExposureProgram(const std::string& str)
{
  if (str == "not defined")
    return ExposureProgram::NotDefined;
  if (str == "manual")
    return ExposureProgram::Manual;
  if (str == "normal program")
    return ExposureProgram::NormalProgram;
  if (str == "aperture priority")
    return ExposureProgram::AperturePriority;
  if (str == "shutter priority")
    return ExposureProgram::ShutterPriority;
  if (str == "creative program")
    return ExposureProgram::CreativeProgram;
  if (str == "action program")
    return ExposureProgram::ActionProgram;
  if (str == "portrait mode")
    return ExposureProgram::PortraitMode;
  if (str == "landscape mode")
    return ExposureProgram::LandscapeMode;
  return ExposureProgram::NotDefined;
}
