#pragma once

#include <functional>
#include <tuple>
#include <QKeyEvent>

#include "snapdecision/decision.h"

using TimeMs = std::size_t;

using CurrentMaxCount = std::tuple<std::size_t, std::size_t, std::size_t>;

using CountPair = std::tuple<std::size_t, std::size_t>;

using SimpleFunction = std::function<void()>;

using keyEventFunction = std::function<void(QKeyEvent* event)>;
