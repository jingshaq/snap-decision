#include "snapdecision/decision.h"


QColor decisionColor(DecisionType d)
{
  switch (d)
  {
  case DecisionType::SuperKeep:
    return QColor(255, 215, 0);
  case DecisionType::Keep:
    return QColor(47, 232, 74);
    case DecisionType::Delete:
      return QColor(255, 74, 74);
    case DecisionType::Unclassified:
      return QColor(41, 136, 207);
    case DecisionType::Unknown:
      return QColor(32, 32, 32);
  }
  return QColor(32, 32, 32);
}

std::string to_string(DecisionType decision) {
  switch (decision) {
  case DecisionType::SuperKeep: return "SuperKeep";
  case DecisionType::Keep: return "Keep";
  case DecisionType::Delete: return "Delete";
  case DecisionType::Unclassified: return "Unclassified";
  case DecisionType::Unknown: return "Unknown";
  default: return "Unknown"; // Fallback for safety
  }
}

DecisionType to_DecisionType(const std::string& str) {
  if (str == "SuperKeep") return DecisionType::SuperKeep;
  if (str == "Keep") return DecisionType::Keep;
  if (str == "Delete") return DecisionType::Delete;
  if (str == "Unclassified") return DecisionType::Unclassified;
  if (str == "Unknown") return DecisionType::Unknown;
  return DecisionType::Unknown; // Fallback for unrecognized strings
}
