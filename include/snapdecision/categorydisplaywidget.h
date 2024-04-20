#pragma once

#include "snapdecision/enums.h"
#include <QFrame>
#include <QMouseEvent>

class CategoryDisplayWidget : public QFrame
{
  Q_OBJECT

public:
  CategoryDisplayWidget(QWidget* parent = nullptr);

  void setCounts(int delete_count, int unclassified_count, int keep_count, int superkeep_count);

signals:
  void activeChange(DecisionType, bool);

protected:
  void paintEvent(QPaintEvent* event) override;

  void mousePressEvent(QMouseEvent *event) override;

private:
  void setState(int which, bool new_value);

  int delete_count_{ 0 };
  int unclassified_count_{ 0 };
  int keep_count_{ 0 };
  int superkeep_count_{ 0 };

  int width_delete_{ 0 };
  int width_unclassified_{ 0 };
  int width_keep_{ 0 };
  int width_superkeep_{ 0 };

  bool delete_active_{ true };
  bool unclassified_active_{ true };
  bool keep_active_{ true };
  bool superkeep_active_{ true };

  QColor color_delete_;
  QColor color_keep_;
  QColor color_gold_;
  QColor color_unclassified_;
};
