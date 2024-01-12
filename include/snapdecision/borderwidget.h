#pragma once

#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "snapdecision/enums.h"

class BorderWidget : public QWidget
{
public:
  BorderWidget(QWidget* parent = nullptr);

  void setDecision(DecisionType d);
  void showDecision(DecisionType d);

protected:
  void startBlinking(DecisionType d);

  void paintEvent(QPaintEvent* event) override;

  QTimer blink_timer_;
  QTimer single_shot_timer_;

  QColor blink_color_;
  QColor fixed_color_;

  int blink_border_size_{ 10 };
  int fixed_border_size_{ 4 };
  int single_shot_timer_id_{ 0 };

  bool border_visible_{ false };

  DecisionType decision_{ DecisionType::Unknown };
};
