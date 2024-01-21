#include "snapdecision/borderwidget.h"

#include "snapdecision/decision.h"
#include "snapdecision/enums.h"

BorderWidget::BorderWidget(QWidget* parent) : QWidget(parent)
{
  setAttribute(Qt::WA_TransparentForMouseEvents);

  connect(&blink_timer_, &QTimer::timeout, this,
          [this]()
          {
            border_visible_ = !border_visible_;
            update();  // Trigger a redraw
          });

  // Configure the single-shot timer
  single_shot_timer_.setSingleShot(true);
  connect(&single_shot_timer_, &QTimer::timeout, this,
          [this]()
          {
            blink_timer_.stop();
            showDecision(decision_);
          });
}

void BorderWidget::setDecision(DecisionType d)
{
  decision_ = d;
  border_visible_ = false;
  startBlinking(d);
}

void BorderWidget::showDecision(DecisionType d)
{
  decision_ = d;
  fixed_color_ = decisionColor(d);
  border_visible_ = d != DecisionType::Unknown;
  update();
}

void BorderWidget::startBlinking(DecisionType d)
{
  blink_color_ = decisionColor(d);
  blink_border_size_ = 10;
  blink_timer_.start(100);

  // Stop the previous single-shot timer if it's running
  if (single_shot_timer_.isActive())
  {
    single_shot_timer_.stop();
  }

  // Start the single-shot timer
  single_shot_timer_.start(500);
}

void BorderWidget::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  if (border_visible_)
  {
    if (blink_timer_.isActive())
    {
      QPen pen(blink_color_, blink_border_size_);
      painter.setPen(pen);
      painter.drawRect(rect().adjusted(1, 1, -1, -1));
    }
    else
    {
      QPen pen(fixed_color_, fixed_border_size_);
      painter.setPen(pen);
      painter.drawRect(rect().adjusted(1, 1, -1, -1));
    }
  }
}
