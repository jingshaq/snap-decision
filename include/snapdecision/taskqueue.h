#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class TaskHandle
{
public:
  enum class TaskState
  {
    Queued,
    Running,
    Completed,
    Cancelled
  };

  TaskHandle(std::shared_ptr<double> progress_ptr) : progress_ptr_(progress_ptr), state_(TaskState::Queued)
  {
  }

  double getProgress() const
  {
    return *progress_ptr_;
  }
  TaskState getState() const
  {
    return state_;
  }

private:
  friend class TaskQueue;

  void updateProgress(double progress)
  {
    *progress_ptr_ = progress;
  }

  void updateState(TaskState state)
  {
    state_ = state;
  }

  std::shared_ptr<double> progress_ptr_;
  std::atomic<TaskState> state_;
};

class TaskQueue
{
public:
  using Ptr = std::shared_ptr<TaskQueue>;

  TaskQueue(size_t threadCount = 1) : shutdown_(false), shutdown_one_(false)
  {
    setThreadCount(threadCount);
  }

  ~TaskQueue()
  {
    {
      std::lock_guard<std::mutex> lock(task_queue_mutex_);
      shutdown_ = true;
      while (!tasks_.empty())
      {
        tasks_.pop();
      }
    }
    cond_.notify_all();

    joinFinishedThreads();  // Ensure all threads are joined on destruction

    for (std::thread& worker : workers_)
    {
      if (worker.joinable())
      {
        worker.join();
      }
    }
  }

  void setThreadCount(size_t count)
  {
    std::lock_guard lock(thread_management_mutex_);
    if (count >= workers_.size())
    {
      for (size_t i = workers_.size(); i < count; ++i)
      {
        workers_.emplace_back(std::thread([this] { this->workerFunction(); }));
      }
    }
    else
    {
      size_t excess_threads = workers_.size() - count;
      for (size_t i = 0; i < excess_threads; ++i)
      {
        shutdown_one_ = true;
        cond_.notify_one();
        threads_to_join_.push_back(std::move(workers_.back()));
        workers_.pop_back();
      }
    }
    joinFinishedThreads();
  }

  std::shared_ptr<TaskHandle> submit(std::function<void(double&)> func, int priority = 0)
  {
    auto progress_ptr = std::make_shared<double>(0.0);
    auto handle = std::make_shared<TaskHandle>(progress_ptr);
    {
      std::lock_guard<std::mutex> lock(task_queue_mutex_);
      tasks_.emplace(priority,
                     [func, handle, progress_ptr]()
                     {
                       handle->updateState(TaskHandle::TaskState::Running);
                       func(*progress_ptr);
                       handle->updateState(TaskHandle::TaskState::Completed);
                     });
    }
    cond_.notify_one();
    return handle;
  }

private:
  void joinFinishedThreads()
  {
    std::lock_guard lock(thread_management_mutex_);
    auto it = threads_to_join_.begin();
    while (it != threads_to_join_.end())
    {
      if (it->joinable())
      {
        it->join();
        it = threads_to_join_.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

  struct Task
  {
    int priority;
    std::function<void()> func;

    bool operator<(const Task& other) const
    {
      return priority < other.priority;
    }
  };

  std::priority_queue<Task> tasks_;
  std::vector<std::thread> workers_;
  std::vector<std::thread> threads_to_join_;
  std::mutex task_queue_mutex_;
  std::recursive_mutex thread_management_mutex_;
  std::condition_variable cond_;
  std::atomic<bool> shutdown_;
  std::atomic<bool> shutdown_one_;

  void workerFunction()
  {
    while (true)
    {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(task_queue_mutex_);
        cond_.wait(lock, [this] { return shutdown_ || shutdown_one_ || !tasks_.empty(); });
        if (shutdown_one_)
        {
          shutdown_one_ = false;  // Reset the flag
          return;                 // Exit the thread
        }
        if (shutdown_ && tasks_.empty())
        {
          return;  // Exit the thread
        }
        task = tasks_.top().func;
        tasks_.pop();
      }
      task();
    }
  }
};
