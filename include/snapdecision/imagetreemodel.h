#pragma once

#include <QAbstractItemModel>
#include <memory>

#include "imagedescriptionnode.h"
#include "snapdecision/enums.h"

class ImageTreeModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  using Ptr = std::shared_ptr<ImageTreeModel>;

  explicit ImageTreeModel(QObject* parent = nullptr);
  ~ImageTreeModel() override;


  // Required methods to override
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& child) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  // Setter method for ImageDescriptions
  void setImageRoot(const ImageDescriptionNode::Ptr& root_node);

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  ImageDescriptionNode* nodeFromIndex(const QModelIndex& index) const;

  QModelIndex indexForImage(const QString& imageName);

  QVector<QString> getFileList() const;

private:
  void recomputeCachedData();

  QVector<QString> file_list_;
  QMap<QString, QModelIndex> index_for_image_;

  ImageDescriptionNode* nodeFromIndex(const QModelIndex& index, ImageDescriptionNode* fallback) const;
  ImageDescriptionNode::Ptr root_;

  QIcon green_image;
  QIcon red_image;
  QIcon blue_image;
  QIcon grey_image;
  QIcon burst_image;
  QIcon location_image;

};

