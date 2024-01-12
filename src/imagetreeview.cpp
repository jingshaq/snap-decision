#include "snapdecision/imagetreeview.h"

ImageTreeView::ImageTreeView(QWidget* parent) : QTreeView(parent)
{
}

void ImageTreeView::navigate(int direction)
{
  const int maxIterations = 10;

  auto isItemNavigable = [](const QModelIndex& index)
  {
    return index.isValid() && index.flags().testFlag(Qt::ItemIsSelectable) && index.flags().testFlag(Qt::ItemIsEnabled);
  };

  auto findNavigableIndex = [this, &isItemNavigable](const QModelIndex& start_index, bool search_up)
  {
    QModelIndex index = start_index;
    int iterations = 0;

    do
    {
      index = search_up ? indexAbove(index) : indexBelow(index);
      iterations++;
    } while (!isItemNavigable(index) && iterations < maxIterations);

    return index;
  };

  QModelIndex currentIndex = this->currentIndex();

  if (direction > 0)
  {
    QModelIndex nextIndex = findNavigableIndex(currentIndex, false);
    if (!nextIndex.isValid())
    {
      // Wrap to the first item
      nextIndex = findNavigableIndex(model()->index(0, 0), false);
    }
    if (nextIndex.isValid())
    {
      setCurrentIndex(nextIndex);
      navigate(direction - 1);
    }
  }

  if (direction < 0)
  {
    QModelIndex prevIndex = findNavigableIndex(currentIndex, true);
    if (!prevIndex.isValid())
    {
      // Wrap to the last item
      QModelIndex lastIndex = model()->index(model()->rowCount() - 1, 0);
      prevIndex = findNavigableIndex(lastIndex, true);
    }
    if (prevIndex.isValid())
    {
      setCurrentIndex(prevIndex);
      navigate(direction + 1);
    }
  }
}

void ImageTreeView::keyPressEvent(QKeyEvent* event)
{
  if (key_event_function_)
  {
     key_event_function_(event);
  }

  if (!event->isAccepted())
  {
    QTreeView::keyPressEvent(event);
  }
}
