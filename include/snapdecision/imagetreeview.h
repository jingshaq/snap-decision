#pragma once

#include <QKeyEvent>
#include <QTreeView>

#include "snapdecision/types.h"

class ImageTreeView : public QTreeView
{
  Q_OBJECT

public:
  ImageTreeView(QWidget* parent = nullptr);

  void navigate(int direction);

  keyEventFunction key_event_function_;

protected:
  void keyPressEvent(QKeyEvent* event) override;
};
