#pragma once

#include <algorithm>
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
  Keep
};

inline bool decisionShift(DecisionType& base, int shift)
{
  auto original = base;
  // Clamp the shift value between -2 and 2
  shift = std::clamp(shift, -2, 2);

  // clang-format off
    // Lookup table for each combination of DecisionType and shift
    static const DecisionType lookupTable[4][5] = {
        // Shift values: -2     -1      0      1      2
        {DecisionType::Delete, DecisionType::Delete, DecisionType::Unclassified,    DecisionType::Keep, DecisionType::Keep}, // Unknown
        {DecisionType::Delete, DecisionType::Delete, DecisionType::Delete,     DecisionType::Unclassified, DecisionType::Keep}, // Delete
        {DecisionType::Delete, DecisionType::Delete, DecisionType::Unclassified, DecisionType::Keep,         DecisionType::Keep}, // Unclassified
        {DecisionType::Delete, DecisionType::Unclassified, DecisionType::Keep, DecisionType::Keep,         DecisionType::Keep}  // Keep
    };
  // clang-format on

  int baseIndex = std::clamp(static_cast<int>(base), 0, 3);

  base = lookupTable[baseIndex][shift + 2];

  return (base != original);
}

std::string to_string(DecisionType decision);
std::string to_string(MeteringMode mode);
std::string to_string(ExposureProgram program);

DecisionType to_DecisionType(const std::string& str);
MeteringMode to_MeteringMode(const std::string& str);
ExposureProgram to_ExposureProgram(const std::string& str);
