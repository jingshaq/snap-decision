#pragma once

#include <QImage>
#include <QObject>
#include <QPixmap>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>

#include "diagnostics.h"
#include "snapdecision/taskqueue.h"
#include "snapdecision/types.h"

class ImageCache;

class ImageCacheHandle
{
public:
  using Ptr = std::shared_ptr<ImageCacheHandle>;

  enum class State
  {
    Queued,
    Running,
    Complete,
    Cancelled,
    Unloaded
  };

  ImageCacheHandle(std::weak_ptr<ImageCache> image_cache, const std::string& image_path);

  QPixmap blockingImage();
  QPixmap image();  // gets the image if it's available, otherwise a null QPixmal
  void scheduleImage();

  State getState() const;
  void touch();

  std::string imagePath() const;
  std::shared_ptr<ImageCache> cache() const;

  std::size_t memory() const;
  std::size_t lastUsage() const;
  void unload();

private:
  mutable std::recursive_mutex mutex_;

  std::weak_ptr<ImageCache> image_cache_;

  std::atomic<State> state_{ State::Unloaded };

  QPixmap pixmap_;
  std::string image_path_;
  std::size_t last_touch_{ 0 };
  std::size_t memory_{ 0 };
};

namespace ImageCacheSupport
{

class SignalEmitter : public QObject
{
  Q_OBJECT
public:
  SignalEmitter(QObject* parent = nullptr) : QObject(parent)
  {
  }

  void emitMemoryUsageChanged(CurrentMaxCount cmc);
  void emitHitMissUpdate(CountPair cp);

signals:
  void memoryUsageChanged(CurrentMaxCount);
  void hitMissUpdate(CountPair);
};
}  // namespace ImageCacheSupport

class ImageCache : public std::enable_shared_from_this<ImageCache>
{
public:
  using Ptr = std::shared_ptr<ImageCache>;
  using WeakPtr = std::weak_ptr<ImageCache>;

  ImageCache(const TaskQueue::Ptr& task_queue, DiagnosticFunction diag_function);

  Ptr getptr()
  {
    return shared_from_this();
  }

  ImageCacheHandle::Ptr getHandle(const std::string& image_path);
  ImageCacheHandle::Ptr getImage(const std::string& image_path);
  ImageCacheHandle::Ptr immediateGetImage(const std::string& image_path);
  ImageCacheHandle::Ptr scheduleImage(const std::string& image_path);

  CurrentMaxCount getMemoryUsage() const;
  std::size_t totalMemoryUsage() const;

  void setMaxMemoryUsage(std::size_t max_memory_usage);

  static DiagnosticFunction getDiagFunction(const ImageCache::WeakPtr& wp);

  DiagnosticFunction diagFunction() const;

  void manageCache();

  ImageCacheSupport::SignalEmitter signal_emitter;

private:
  std::unordered_map<std::string, ImageCacheHandle::Ptr> cache_;

  std::atomic<size_t> max_memory_usage_{ 1000 * 1000 * 1000 };  // bytes
  mutable std::recursive_mutex cache_mutex_;

  DiagnosticFunction diag_func_;
  TaskQueue::Ptr task_queue_;

  void blockingLoadToCache(const std::string& image_path);
  void updateHitMiss(std::size_t hit_inc, std::size_t miss_inc);

  std::size_t hit_count_{ 0 };
  std::size_t miss_count_{ 0 };
};
