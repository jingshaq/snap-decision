#pragma once

#include <QObject>

#include "mainmodel.h"
#include "mainwindow.h"
#include "snapdecision/imagegroup.h"
#include "snapdecision/settings.h"
#include "types.h"

// MainController.h - Coordinates between the Model and the View
class MainController : public QObject
{
  Q_OBJECT;

public:
  MainController(MainModel* model, MainWindow* view, Settings* settings);

public slots:  // Slot declaration
  void treeBuildComplete();
  void loadResource(const QString& path);
  void focusOnNode(const QString& image_name);
  void memoryUsageChanged(CurrentMaxCount cmc);
  void updateDecisionCounts();
  void moveDeleteMarked();

private:
  void executeTool(int i);
  void vote(int direction);

  void keyPressed(QKeyEvent* event);
  void setSettings(const Settings& s);

  MainModel* model_;
  MainWindow* view_;
  Settings* settings_;


  int previous_focus_index_{ -1 };
  int current_focus_index_{ -1 };
  QString resource_;
  QString current_image_full_path_;

  int predictNextNode(int step) const;

  void setupConnections();
};
