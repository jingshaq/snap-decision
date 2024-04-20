#pragma once

#include <string>

enum class LogLevel
{
  Info = 0,
  Warn,
  Error
};

enum class MeteringMode
{
  Unknown = 0,
  Average,
  CenterWeightedAverage,
  Spot,
  MultiSpot,
  Pattern,
  Partial
};

enum class ExposureProgram
{
  NotDefined = 0,
  Manual,
  NormalProgram,
  AperturePriority,
  ShutterPriority,
  CreativeProgram,
  ActionProgram,
  PortraitMode,
  LandscapeMode
};

enum class DecisionType
{
  Unknown = 0,
  Delete,
  Unclassified,
  Keep,
  SuperKeep
};

bool decisionShift(DecisionType& base, int shift);

std::string to_string(DecisionType decision);
std::string to_string(MeteringMode mode);
std::string to_string(ExposureProgram program);

DecisionType to_DecisionType(const std::string& str);
MeteringMode to_MeteringMode(const std::string& str);
ExposureProgram to_ExposureProgram(const std::string& str);
