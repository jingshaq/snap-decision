#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <mutex>

#include "snapdecision/borderwidget.h"
#include "snapdecision/enums.h"
#include "snapdecision/types.h"

class SnapDecisionGraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  SnapDecisionGraphicsView(QWidget* parent = nullptr);
  ~SnapDecisionGraphicsView();
  void setImage(const QPixmap& pixmap, const std::string& filename, int orientation);

  keyEventFunction key_event_function_;

  void setDecision(DecisionType d);
  void showDecision(DecisionType d);

  void fitPixmapInView();

  void setViewLevel(int i);

  void pan(float dx, float dy);
  void panView(int dx, int dy);

  void storeCenter();
  void restoreCenter();

public slots:
  void externalZoomIn();

  void externalZoomOut();


protected:
  void wheelEvent(QWheelEvent* event) override;

  void mouseDoubleClickEvent(QMouseEvent *event) override;

  void zoomAt(const QPointF& center_pos, double factor);

  void zoom(double factor);

  void mousePressEvent(QMouseEvent* event) override;

  void mouseMoveEvent(QMouseEvent* event) override;

  void mouseReleaseEvent(QMouseEvent* event) override;

  void keyPressEvent(QKeyEvent* event) override;

  void resizeEvent(QResizeEvent* event) override;

private:
  double calculateBaseScaleFactor();

  std::mutex mutex_;
  std::string current_filename_;
  bool border_visible_{ true };

  BorderWidget* border_widget_{ nullptr };
  QGraphicsScene* scene_{ nullptr };
  QGraphicsPixmapItem* pix_item_{ nullptr };
  QPoint last_pan_point_;

  double baseScaleFactor = 1.0; // Store the base scale factor
  QPointF current_center_;

  bool first_load_needs_fit_in_view_{ false };
};
