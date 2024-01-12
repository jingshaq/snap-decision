#pragma once

#include <QMap>
#include <QString>

#include "snapdecision/taskqueue.h"

#include "databasemanager.h"
#include "diagnostics.h"
#include "imagecache.h"
#include "imagetreemodel.h"
#include "snapdecision/dnn.h"
#include "snapdecision/imagegroup.h"

class MainModel
{
public:
  DNN::Ptr dnn_;
  ImageCache::Ptr image_cache_;
  ImageTreeModel::Ptr image_tree_model_;
  TaskQueue::Ptr task_queue_;
  DatabaseManager::Ptr database_manager_;
  DiagnosticFunction diagnostic_function_;
  ImageGroup::Ptr image_group_;
};
