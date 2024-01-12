#include "snapdecision/imagetreemodel.h"

#include <QAbstractItemModel>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QModelIndex>
#include <QPalette>
#include <QVariant>

#include "snapdecision/enums.h"
#include "snapdecision/utils.h"
#include "snapdecision/imagedescriptionnode.h"

ImageTreeModel::ImageTreeModel(QObject* parent) : QAbstractItemModel(parent)
{
  green_image = QIcon(QPixmap(":/images/fox_file_green.png"));
  red_image = QIcon(QPixmap(":/images/fox_file_red.png"));
  blue_image = QIcon(QPixmap(":/images/fox_file_blue.png"));
  grey_image = QIcon(QPixmap(":/images/fox_file_grey.png"));

  burst_image = QIcon(QPixmap(":/images/burst.png"));
  location_image = QIcon(QPixmap(":/images/location.png"));
}

ImageTreeModel::~ImageTreeModel()
{
}

QModelIndex ImageTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  auto parentNode = nodeFromIndex(parent, root_.get());

  if (static_cast<std::size_t>(row) >= parentNode->children.size())
  {
    return QModelIndex();
  }

  return createIndex(row, column, parentNode->children[row].get());
}

QModelIndex ImageTreeModel::parent(const QModelIndex& child) const
{
  if (!child.isValid())
    return QModelIndex();

  auto childNode = nodeFromIndex(child);
  if (!childNode)
    return QModelIndex();

  auto parentNode = childNode->parent.lock();  // Get the parent node
  if (!parentNode || parentNode == root_)
    return QModelIndex();

  // Find the row number of the child in the parent node
  auto grandparentNode = parentNode->parent.lock();
  if (!grandparentNode)
    return QModelIndex();

  int row = std::distance(grandparentNode->children.begin(),
                          std::find_if(grandparentNode->children.begin(), grandparentNode->children.end(),
                                       [parentNode](const auto& node) { return node == parentNode; }));

  return createIndex(row, 0, parentNode.get());
}

int ImageTreeModel::rowCount(const QModelIndex& parent) const
{
  if (parent.column() > 0)
    return 0;

  auto parentNode = nodeFromIndex(parent, root_.get());
  if (!parentNode)
  {
    return 0;
  }
  return static_cast<int>(parentNode->children.size());
}

int ImageTreeModel::columnCount(const QModelIndex& /* parent */) const
{
  return 1;
}

QVariant ImageTreeModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  auto node = nodeFromIndex(index, root_.get());

  // Display Role: Text display for the tree view
  if (role == Qt::DisplayRole)
  {
    switch (node->node_type)
    {
      case NodeType::Root:
        return "Root";
      case NodeType::Location:
        return "New Location " + timeToString(node->getTime());
      case NodeType::Scene:
        return "Burst";
      case NodeType::Image:
        if (node->full_raw_path.empty() || node->raw_path_extension.empty())
        {
          return QString::fromStdString(node->filename);
        }
        else
        {
          return QString::fromStdString(node->filename + "/." + node->raw_path_extension);
        }
      default:
        return QVariant();
    }
  }

  if (role == Qt::FontRole)
  {
    if (node)
    {
      if (node->node_type == NodeType::Image)
      {
        if (node->decision != DecisionType::Delete)
        {
          QFont boldFont;
          boldFont.setBold(true);
          return boldFont;
        }
      }

      if (node->node_type == NodeType::Location)
      {
        QFont boldFont;
        boldFont.setBold(true);
        return boldFont;
      }
    }
  }

  // Foreground Role: Text color for images scheduled to be deleted
  if (role == Qt::ForegroundRole && (node->node_type == NodeType::Image))
  {
    const auto d = node->decision;
    return QBrush(decisionColor(d).darker(150));
  }

  if (role == Qt::DecorationRole)
  {
    switch (node->node_type)
    {
      case NodeType::Image:

        switch (node->decision)
        {
          case DecisionType::Delete:
            return red_image;
          case DecisionType::Keep:
            return green_image;
          case DecisionType::Unclassified:
            return blue_image;
          case DecisionType::Unknown:
            return grey_image;
        }
        break;

      case NodeType::Scene:
        return burst_image;

      case NodeType::Location:
        return location_image;

      default:
        break;
    }
  }

  return QVariant();
}

void ImageTreeModel::setImageRoot(const ImageDescriptionNode::Ptr& root_node)
{
  beginResetModel();
  root_ = root_node;
  endResetModel();

  recomputeCachedData();
}

ImageDescriptionNode* ImageTreeModel::nodeFromIndex(const QModelIndex& index) const
{
  return nodeFromIndex(index, nullptr);
}

QModelIndex ImageTreeModel::indexForImage(const QString& imageName)
{
  if (index_for_image_.contains(imageName))
  {
    return index_for_image_[imageName];
  }
  return QModelIndex();
}

QVector<QString> ImageTreeModel::getFileList() const
{
  return file_list_;
}

void ImageTreeModel::recomputeCachedData()
{
  file_list_.clear();
  index_for_image_.clear();

  std::function<void(const QModelIndex& parent)> f;

  f = [&f, this](const QModelIndex& parent)
  {
    const int rc = rowCount(parent);
    for (int row = 0; row < rc; ++row)
    {
      const QModelIndex idx = index(row, 0, parent);

      auto node = nodeFromIndex(idx, root_.get());

      if (node && !node->full_path.empty())
      {
        const auto fn = QString::fromStdString(node->full_path);

        file_list_.push_back(fn);
        index_for_image_[fn] = idx;
      }

      if (hasChildren(idx))
      {
        f(idx);
      }
    }
  };

  f(QModelIndex());
}

ImageDescriptionNode* ImageTreeModel::nodeFromIndex(const QModelIndex& index, ImageDescriptionNode* fallback) const
{
  if (index.isValid())
  {
    return static_cast<ImageDescriptionNode*>(index.internalPointer());
  }
  return fallback;
}

Qt::ItemFlags ImageTreeModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

  if (!index.isValid())
    return defaultFlags;

  if (auto node = nodeFromIndex(index, root_.get()))
  {
    if (node && node->node_type != NodeType::Image)
    {
      // Make non-leaf nodes not selectable and not focusable
      return defaultFlags & ~(Qt::ItemIsSelectable);  // | Qt::ItemIsEnabled);
    }
  }
  // Leaf nodes retain default behavior
  return defaultFlags;
}
