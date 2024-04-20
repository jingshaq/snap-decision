#pragma once

#include <QKeySequence>
#include <QString>
#include <cstdlib>
#include <array>
#include <functional>

#include "qnamespace.h"

struct Settings
{
  std::size_t burst_threshold_ms_{ 250 };
  std::size_t location_theshold_ms_{ 1000 * 60 * 15 };
  QString delete_foler_name_{ "delete" };
  std::size_t cache_memory_mb_{ 750 };
  bool show_debug_console_{ false };

  QKeySequence key_next_image_{ Qt::Key_Right };
  QKeySequence key_prev_image_{ Qt::Key_Left };
  QKeySequence key_keep_and_next_{ Qt::SHIFT | Qt::Key_Space };
  QKeySequence key_gold_and_next_{ Qt::SHIFT | Qt::Key_G };
  QKeySequence key_delete_and_next_{ Qt::SHIFT |Qt::Key_X };
  QKeySequence key_unclassified_to_keep_and_next_{ Qt::Key_Space };
  QKeySequence key_unclassified_to_gold_and_next_{ Qt::Key_G };
  QKeySequence key_unclassified_to_delete_and_next_{ Qt::Key_X };

  QKeySequence key_pan_left_{ Qt::SHIFT | Qt::Key_Left };
  QKeySequence key_pan_right_{ Qt::SHIFT | Qt::Key_Right };
  QKeySequence key_pan_up_{ Qt::SHIFT | Qt::Key_Up };
  QKeySequence key_pan_down_{ Qt::SHIFT | Qt::Key_Down };

  QKeySequence key_reset_view_{ Qt::Key_Escape };
  QKeySequence key_zoom_1_{ Qt::Key_1 };
  QKeySequence key_zoom_2_{ Qt::Key_2 };
  QKeySequence key_zoom_3_{ Qt::Key_3 };
  QKeySequence key_zoom_4_{ Qt::Key_4 };
  QKeySequence key_zoom_5_{ Qt::Key_5 };

  QKeySequence key_vote_up_{ Qt::Key_Up };
  QKeySequence key_vote_down_{ Qt::Key_Down };

  QKeySequence key_preferences_{ Qt::CTRL | Qt::SHIFT | Qt::Key_P };
  QKeySequence key_delete_marked_{ Qt::CTRL | Qt::SHIFT | Qt::Key_D };

  struct Tool
  {
    bool enabled{ false };
    QString name;
    QString command;
    QKeySequence sequence;
  };

  std::array<Tool, 4> tool{
    Tool{true, "Open in Gimp", "\"C:\\Program Files\\GIMP 2\\bin\\gimp-2.10.exe\" %p", QKeySequence{Qt::CTRL |  Qt::Key_G}},
    Tool{true, "Open in Topaz Photo AI", "powershell -Command \"Start-Process 'C:\\Program Files\\Topaz Labs LLC\\Topaz Photo AI\\Topaz Photo AI.exe' -ArgumentList %r\"", QKeySequence{Qt::CTRL |  Qt::Key_T}},
    {},{}
  };
};

using SetSettings = std::function<void(const Settings&)>;
using GetSettings = std::function<Settings()>;
