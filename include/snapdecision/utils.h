#pragma once

#include <QDateTime>
#include <QString>
#include <optional>
#include <string>

#include "types.h"

inline TimeMs getCurrentTimeMilliseconds()
{
  QDateTime currentTime = QDateTime::currentDateTime();
  return static_cast<TimeMs>(currentTime.toMSecsSinceEpoch());
}

inline QString timeToString(const TimeMs& time)
{
  QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(time);
  return dateTime.toString("yyyy-MM-dd HH:mm:ss");
}

inline QString timeToStringPrecise(const TimeMs& time)
{
  QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(time);
  return dateTime.toString("HH:mm:ss.zzz");
}

inline std::optional<int> toInt(const QString& str)
{
  bool ok;
  int result = str.toInt(&ok);

  if (ok)
  {
    return result;
  }

  return std::nullopt;
}

inline std::optional<int> toInt(const std::string& str)
{
  return toInt(QString::fromStdString(str));
}
