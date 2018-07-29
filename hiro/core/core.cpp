#include <nall/intrinsics.hpp>

#if defined(HIRO_WINDOWS)
  #include "../windows/header.hpp"
#elif defined(HIRO_QT)
  #include "../qt/header.hpp"
#elif defined(HIRO_GTK)
  #include "../gtk/header.hpp"
#elif defined(HIRO_COCOA)
  #include "../cocoa/header.hpp"
#endif

#include "core.hpp"
using namespace nall;

#if defined(HIRO_WINDOWS)
  #include "../windows/platform.cpp"
#elif defined(HIRO_QT)
  #include "../qt/platform.cpp"
#elif defined(HIRO_GTK)
  #include "../gtk/platform.cpp"
#elif defined(HIRO_COCOA)
  #include "../cocoa/platform.cpp"
#endif

#define signal(function, ...) \
  (delegate ? self()->function(__VA_ARGS__) : decltype(self()->function(__VA_ARGS__))())

namespace hiro {
  #include "color.cpp"
  #include "gradient.cpp"
  #include "alignment.cpp"
  #include "cursor.cpp"
  #include "position.cpp"
  #include "size.cpp"
  #include "geometry.cpp"
  #include "font.cpp"

  #include "application.cpp"
  #include "desktop.cpp"
  #include "monitor.cpp"
  #include "keyboard.cpp"
  #include "mouse.cpp"
  #include "browser-window.cpp"
  #include "message-window.cpp"

  #include "property.cpp"

  #include "object.cpp"
  #include "group.cpp"

  #include "hotkey.cpp"
  #include "timer.cpp"
  #include "window.cpp"
  #include "status-bar.cpp"
  #include "menu-bar.cpp"
  #include "popup-menu.cpp"

  #include "action/action.cpp"
  #include "action/menu.cpp"
  #include "action/menu-separator.cpp"
  #include "action/menu-item.cpp"
  #include "action/menu-check-item.cpp"
  #include "action/menu-radio-item.cpp"

  #include "sizable.cpp"

  #include "widget/widget.cpp"
  #include "widget/button.cpp"
  #include "widget/canvas.cpp"
  #include "widget/check-button.cpp"
  #include "widget/check-label.cpp"
  #include "widget/combo-button.cpp"
  #include "widget/combo-button-item.cpp"
  #include "widget/combo-edit.cpp"
  #include "widget/combo-edit-item.cpp"
  #include "widget/console.cpp"
  #include "widget/frame.cpp"
  #include "widget/hex-edit.cpp"
  #include "widget/horizontal-scroll-bar.cpp"
  #include "widget/horizontal-slider.cpp"
  #include "widget/icon-view.cpp"
  #include "widget/icon-view-item.cpp"
  #include "widget/label.cpp"
  #include "widget/line-edit.cpp"
  #include "widget/progress-bar.cpp"
  #include "widget/radio-button.cpp"
  #include "widget/radio-label.cpp"
  #include "widget/source-edit.cpp"
  #include "widget/tab-frame.cpp"
  #include "widget/tab-frame-item.cpp"
  #include "widget/table-view.cpp"
  #include "widget/table-view-header.cpp"
  #include "widget/table-view-column.cpp"
  #include "widget/table-view-item.cpp"
  #include "widget/table-view-cell.cpp"
  #include "widget/text-edit.cpp"
  #include "widget/tree-view.cpp"
  #include "widget/tree-view-item.cpp"
  #include "widget/vertical-scroll-bar.cpp"
  #include "widget/vertical-slider.cpp"
  #include "widget/viewport.cpp"
}

#undef signal
