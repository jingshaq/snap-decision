#pragma once

#include <QFrame>

class CategoryDisplayWidget : public QFrame
{
  Q_OBJECT

public:
  CategoryDisplayWidget(QWidget* parent = nullptr);

  void setCounts(int delete_count, int unclassified_count, int keep_count);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  int delete_count_{ 0 };
  int unclassified_count_{ 0 };
  int keep_count_{ 0 };

  QColor color_delete_;
  QColor color_keep_;
  QColor color_unclassified_;
};
