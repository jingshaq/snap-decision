#pragma once

#include <functional>
#include <memory>
#include <mutex>

struct EfficientdetD0;

class DNN : public std::enable_shared_from_this<DNN>
{
public:
  using Ptr = std::shared_ptr<DNN>;
#if 0
  DNN();
  ~DNN();

  std::function<void (double&)> initializeWorkerFunction();

  void setEfficientdetD0(std::unique_ptr<EfficientdetD0>&& efficientdet_d0);

private:
  std::recursive_mutex mutex_;
  std::unique_ptr<EfficientdetD0> efficientdet_d0_;
#endif
};
