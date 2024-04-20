#pragma once

#include <QDebug>
#include <QtSql>
#include <memory>
#include <mutex>

#include "snapdecision/decision.h"
#include "snapdecision/diagnostics.h"

class DatabaseManager
{
public:
  using Ptr = std::shared_ptr<DatabaseManager>;
  using WeakPtr = std::weak_ptr<DatabaseManager>;

  DatabaseManager(DiagnosticFunction diagnostic_function);

  void checkpoint();

  void switchToInMemory();
  void switchToFileBased(const std::string& filePath);
  std::string getLocation() const;

  void setMake(const std::string& image_path, const std::string& make);
  void setModel(const std::string& image_path, const std::string& model);
  void setDateTime(const std::string& image_path, const std::string& dateTime);
  void setDateTimeOriginal(const std::string& image_path, const std::string& dateTimeOriginal);
  void setSubSecTimeOriginal(const std::string& image_path, const std::string& subSecTimeOriginal);
  void setImageWidth(const std::string& image_path, int width);
  void setImageHeight(const std::string& image_path, int height);
  void setBitsPerSample(const std::string& image_path, int bitsPerSample);
  void setISOSpeedRatings(const std::string& image_path, int isoSpeedRatings);
  void setFNumber(const std::string& image_path, double fNumber);
  void setExposureTime(const std::string& image_path, double shutterSpeedValue);
  void setApertureValue(const std::string& image_path, double apertureValue);
  void setBrightnessValue(const std::string& image_path, double brightnessValue);
  void setExposureBiasValue(const std::string& image_path, double value);
  void setSubjectDistance(const std::string& image_path, double value);
  void setFocalLength(const std::string& image_path, double value);
  void setOrientation(const std::string& image_path, int value);

  void setDecision(const std::string& image_path, DecisionType decision);
  void setExposureProgram(const std::string& image_path, ExposureProgram exposureProgram);
  void setMeteringMode(const std::string& image_path, MeteringMode meteringMode);

  void setCreationMs(const std::string& image_path, std::size_t creation_ms);

  std::optional<std::string> getMake(const std::string& image_path);
  std::optional<std::string> getModel(const std::string& image_path);
  std::optional<std::string> getDateTime(const std::string& image_path);
  std::optional<std::string> getDateTimeOriginal(const std::string& image_path);
  std::optional<std::string> getSubSecTimeOriginal(const std::string& image_path);
  std::optional<int> getImageWidth(const std::string& image_path);
  std::optional<int> getImageHeight(const std::string& image_path);
  std::optional<int> getBitsPerSample(const std::string& image_path);
  std::optional<int> getISOSpeedRatings(const std::string& image_path);
  std::optional<double> getFNumber(const std::string& image_path);
  std::optional<double> getShutterSpeedValue(const std::string& image_path);
  std::optional<double> getApertureValue(const std::string& image_path);
  std::optional<double> getBrightnessValue(const std::string& image_path);
  std::optional<double> getExposureBiasValue(const std::string& image_path);
  std::optional<double> getSubjectDistance(const std::string& image_path);
  std::optional<double> getFocalLength(const std::string& image_path);
  std::optional<int> getOrientation(const std::string& image_path);

  std::array<std::size_t, 5> getDecisionCounts();

  std::optional<DecisionType> getDecision(const std::string& image_path);
  std::optional<ExposureProgram> getExposureProgram(const std::string& image_path);
  std::optional<MeteringMode> getMeteringMode(const std::string& image_path);

  std::optional<std::size_t> getCreationMs(const std::string& image_path);

  std::vector<std::string> getDeleteDecisionFilenames();

  void removeRowsIfAbsolutePath(std::function<bool(const std::string&)> condition);
  bool removeRowForPath(const std::string& path);

  static bool unitTest();

  void clear();
  void close();

private:
  mutable std::recursive_mutex mutex_;

  DiagnosticFunction diagnostic_function_;
  std::shared_ptr<QSqlDatabase> db;

  void initializeDb(QSqlDatabase& target);
  std::shared_ptr<QSqlDatabase> createDb(const QString& db_name, const QString& connection = "");
  void copyDataToNewDb(const QString& new_db_name);
  void copyDbContents(QSqlDatabase& source_db, QSqlDatabase& target_db);
};
