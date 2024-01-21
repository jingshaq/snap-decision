#pragma once

#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include "snapdecision/databasemanager.h"
#include "snapdecision/imagecache.h"

enum class NodeType
{
  Root,
  Location,
  Scene,
  Image
};

struct ImageDescriptionNode
{
  ImageDescriptionNode(NodeType type = NodeType::Image);

  using Ptr = std::shared_ptr<ImageDescriptionNode>;
  using WeakPtr = std::weak_ptr<ImageDescriptionNode>;

  WeakPtr parent;
  std::vector<Ptr> children;
  NodeType node_type{ NodeType::Image };

  // lazy load image data from image cache
  ImageCacheHandle::Ptr image_cache_handle_;

  std::string filename;       // image.jpg
  std::string full_path;      // /the/full/path/image.jpg
  std::string full_raw_path;  // /the/full/path/image.[raw|cr3|etc]
  std::string raw_path_extension;

  DecisionType decision{ DecisionType::Unclassified };
  ExposureProgram exposure_program{ ExposureProgram::NotDefined };

  template <typename C, typename T = typename std::invoke_result_t<C, ImageDescriptionNode*>>
  std::optional<T> leafConsensus(C callable)
  {
    if (children.empty())  // then leaf
    {
      return callable(this);
    }

    std::optional<T> consensus = children.front()->leafConsensus(callable);

    if (!consensus.has_value())
    {
      return consensus;
    }

    for (std::size_t i = 1; i < children.size(); i++)
    {
      const auto cc = children[i]->leafConsensus(callable);

      if (!cc.has_value())
      {
        return cc;
      }

      if (cc.value() != consensus.value())
      {
        return std::nullopt;
      }
    }

    return consensus;
  }

  std::optional<DecisionType> leafConsensusDecision() const
  {
    if (children.empty())  // then leaf
    {
      return decision;
    }

    std::optional<DecisionType> consensus = children.front()->leafConsensusDecision();

    if (!consensus.has_value())
    {
      return consensus;
    }

    for (std::size_t i = 1; i < children.size(); i++)
    {
      const auto cc = children[i]->leafConsensusDecision();

      if (!cc.has_value())
      {
        return cc;
      }

      if (cc.value() != consensus.value())
      {
        return std::nullopt;
      }
    }

    return consensus;
  }

  std::string exposureProgramString() const
  {
    switch (exposure_program)
    {
      case ExposureProgram::Manual:
        return "Manual";

      case ExposureProgram::AperturePriority:
        return "Av";

      case ExposureProgram::ShutterPriority:
        return "Tv";

      default:
        break;
    }
    return "";
  }

  MeteringMode metering_mode{ MeteringMode::Unknown };
  TimeMs time_ms{ 0 };

  TimeMs getTime() const
  {
    if (time_ms)
    {
      return time_ms;
    }
    if (!children.empty())
    {
      return children.front()->getTime();
    }
    return 0;
  }

  std::string make;
  std::string model;
  int width{ 0 };
  int height{ 0 };
  int iso{ 0 };

  double f_number{ 0 };
  double shutter_speed{ 0 };
  double exposure_bias{ 0 };

  double focal_length{ 0 };
  int orientation{ 0 };  // 0: unspecified in EXIF data
                         // 1: upper left of image
                         // 3: lower right of image
                         // 6: upper right of image
                         // 8: lower left of image
                         // 9: undefined

  // This gets set to true when the loading thread have finished populating
  // the above data.
  std::atomic<bool> ready{ false };
};

ImageDescriptionNode::Ptr buildImageDescriptionNode(const std::string& filename, const ImageCache::Ptr& image_cache,
                                                    const TaskQueue::Ptr& task_queue,
                                                    const DatabaseManager::Ptr& database_manager,
                                                    const SimpleFunction& on_finish);
