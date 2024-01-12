#include "snapdecision/snapdecisiongraphicsview.h"

#include <QImage>
#include <QPixmap>
#include <mutex>

SnapDecisionGraphicsView::SnapDecisionGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

  scene_ = new QGraphicsScene(this);
  this->setScene(scene_);
  setDragMode(QGraphicsView::NoDrag);

  border_widget_ = new BorderWidget(this);
  border_widget_->resize(size());
  border_widget_->show();
}

SnapDecisionGraphicsView::~SnapDecisionGraphicsView()
{
}

void SnapDecisionGraphicsView::setImage(const QPixmap& pixmap, const std::string& filename, int orientation)
{
  if (pixmap.isNull())
  {
    return;
  }

  {
    std::lock_guard lock(mutex_);
    current_filename_ = filename;
  }

  const bool first_image = (pix_item_ == nullptr);

  scene_->clear();
  pix_item_ = scene_->addPixmap(pixmap);
  pix_item_->setZValue(0);

  pix_item_->setTransformOriginPoint(pix_item_->boundingRect().center());
  QPointF item_center = pix_item_->boundingRect().center();
  pix_item_->setPos(-item_center);

  pix_item_->setRotation(0);

  switch (orientation)
  {
    case 3:
      pix_item_->setRotation(180);
      break;
    case 6:
      pix_item_->setRotation(90);
      break;
    case 8:
      pix_item_->setRotation(-90);
      break;
  }

  float value = 50000;

  for (int i = 0; i < 10; i++)
  {
    const QRectF big(-value, -value, value * 2, value * 2);
    value *= 2;
    scene_->setSceneRect(big);

    if (big.contains(pix_item_->sceneBoundingRect()))
    {
      break;
    }
  }

  if (first_image)
  {
    fitPixmapInView();
    first_load_needs_fit_in_view_ = true;
  }

  storeCenter();
}

void SnapDecisionGraphicsView::setDecision(DecisionType d)
{
  if (border_widget_)
  {
    border_widget_->setDecision(d);
  }
}

void SnapDecisionGraphicsView::showDecision(DecisionType d)
{
  if (border_widget_)
  {
    border_widget_->showDecision(d);
  }
}

void SnapDecisionGraphicsView::fitPixmapInView()
{
  if (pix_item_)
  {
    fitInView(pix_item_, Qt::KeepAspectRatio);
    storeCenter();
  }
}

static bool approximatelyEqual(double a, double b)
{
  if (a == b)
  {
    return true;
  }

  if (a * b == 0)
  {
    return false;
  }

  const double x = a / b;

  return x >= 0.99 && x <= 1.01;
}

void SnapDecisionGraphicsView::setViewLevel(int level)
{
  if (level < 1 || level > 5)
    return;

  double base_scale_factor = calculateBaseScaleFactor();
  double new_scale_factor = base_scale_factor;

  if (level == 1)
  {
    if (approximatelyEqual(base_scale_factor, transform().m11()))
    {
      fitPixmapInView();
      return;
    }
  }

  if (level > 1)
  {
    new_scale_factor *= std::pow(2.0, level - 1);
  }

  // Calculate the scale factor relative to the current scale
  double relative_scale_factor = new_scale_factor / transform().m11();

  scale(relative_scale_factor, relative_scale_factor);
  restoreCenter();
}

void SnapDecisionGraphicsView::externalZoomIn()
{
  zoom(1.15);
}

void SnapDecisionGraphicsView::externalZoomOut()
{
  zoom(1 / 1.15);
}

void SnapDecisionGraphicsView::wheelEvent(QWheelEvent* event)
{
  double angle = event->angleDelta().y();

  double factor = qPow(1.0015, angle);  // smoother zoom
  zoomAt(event->position(), factor);
  storeCenter();
}

void SnapDecisionGraphicsView::mouseDoubleClickEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    if (key_event_function_)
    {
      QKeyEvent event(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
      key_event_function_(&event);
    }
  }
}

void SnapDecisionGraphicsView::pan(float dx, float dy)
{
  const ViewportAnchor anchor = transformationAnchor();
  setTransformationAnchor(QGraphicsView::NoAnchor);
  translate(dx, dy);
  setTransformationAnchor(anchor);
  storeCenter();
}

void SnapDecisionGraphicsView::panView(int dx, int dy)
{
  const auto scene_d = mapToScene(QPoint(dx, dy)) - mapToScene(QPoint(0, 0));

  return pan(scene_d.x(), scene_d.y());
}

void SnapDecisionGraphicsView::storeCenter()
{
  current_center_ = mapToScene(viewport()->rect().center());
}

void SnapDecisionGraphicsView::restoreCenter()
{
  centerOn(current_center_);
}

void SnapDecisionGraphicsView::zoomAt(const QPointF& center_pos, double factor)
{
  double min_zoom = 0.05;
  double max_zoom = 100.0;

  QPointF target_scene_pos = mapToScene(center_pos.toPoint());
  ViewportAnchor old_anchor = this->transformationAnchor();
  setTransformationAnchor(QGraphicsView::NoAnchor);

  QTransform matrix = transform();
  matrix.translate(target_scene_pos.x(), target_scene_pos.y());

  // Calculate new scale factor and clamp it
  double scaleFactor = matrix.m11() * factor;  // m11() gives the horizontal scaling factor
  scaleFactor = qMax(min_zoom, qMin(scaleFactor, max_zoom));

  // Apply the clamped scale factor
  matrix.scale(scaleFactor / matrix.m11(), scaleFactor / matrix.m22());
  matrix.translate(-target_scene_pos.x(), -target_scene_pos.y());
  setTransform(matrix);

  setTransformationAnchor(old_anchor);
}

void SnapDecisionGraphicsView::zoom(double factor)
{
  zoomAt(viewport()->contentsRect().center(), factor);
}

void SnapDecisionGraphicsView::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    last_pan_point_ = event->pos();
    setCursor(Qt::ClosedHandCursor);
  }
}

void SnapDecisionGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
  if (!last_pan_point_.isNull())
  {
    QPointF delta = mapToScene(event->pos()) - mapToScene(last_pan_point_);
    pan(delta.x(), delta.y());
    last_pan_point_ = event->pos();
  }
}

void SnapDecisionGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    last_pan_point_ = QPoint();
    setCursor(Qt::ArrowCursor);
  }
}

void SnapDecisionGraphicsView::keyPressEvent(QKeyEvent* event)
{
  if (key_event_function_)
  {
    key_event_function_(event);
  }

  if (!event->isAccepted())
  {
    QGraphicsView::keyPressEvent(event);
  }
}

void SnapDecisionGraphicsView::resizeEvent(QResizeEvent* event)
{
  QGraphicsView::resizeEvent(event);
  if (border_widget_)
  {
    border_widget_->resize(size());
  }

  if (first_load_needs_fit_in_view_)
  {
    fitPixmapInView();
    first_load_needs_fit_in_view_ = false;
  }
}

double SnapDecisionGraphicsView::calculateBaseScaleFactor()
{
  if (pix_item_)
  {
    QRectF pixmap_rect = pix_item_->sceneBoundingRect();
    QRectF view_rect = this->viewport()->rect();

    double scale_x = view_rect.width() / pixmap_rect.width();
    double scale_y = view_rect.height() / pixmap_rect.height();
    return qMin(scale_x, scale_y);  // Use the smaller scale to fit the entire pixmap
  }
  return 1;
}
