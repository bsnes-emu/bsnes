namespace hiro {
  struct pWindow;
  struct pMenu;
  struct pLayout;
  struct pWidget;
}

#define Declare(Name, Base) \
  p##Name(m##Name& reference) : p##Base(reference) {} \
  auto self() const -> m##Name& { return (m##Name&)reference; } \
  auto state() const -> m##Name::State& { return self().state; } \
  auto construct() -> void override; \
  auto destruct() -> void override; \

#include "font.hpp"
#include "desktop.hpp"
#include "monitor.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "browser-window.hpp"
#include "message-window.hpp"

#include "object.hpp"
#include "group.hpp"

#include "hotkey.hpp"
#include "timer.hpp"
#include "window.hpp"
#include "status-bar.hpp"
#include "menu-bar.hpp"
#include "popup-menu.hpp"

#include "action/action.hpp"
#include "action/menu.hpp"
#include "action/menu-separator.hpp"
#include "action/menu-item.hpp"
#include "action/menu-check-item.hpp"
#include "action/menu-radio-item.hpp"

#include "sizable.hpp"
#include "layout.hpp"

#include "widget/widget.hpp"
#include "widget/button.hpp"
#include "widget/canvas.hpp"
#include "widget/check-button.hpp"
#include "widget/check-label.hpp"
#include "widget/combo-button.hpp"
#include "widget/combo-button-item.hpp"
#include "widget/console.hpp"
#include "widget/frame.hpp"
#include "widget/hex-edit.hpp"
#include "widget/horizontal-scroller.hpp"
#include "widget/horizontal-slider.hpp"
#include "widget/icon-view.hpp"
#include "widget/icon-view-item.hpp"
#include "widget/label.hpp"
#include "widget/line-edit.hpp"
#include "widget/list-view.hpp"
#include "widget/list-view-header.hpp"
#include "widget/list-view-column.hpp"
#include "widget/list-view-item.hpp"
#include "widget/list-view-cell.hpp"
#include "widget/progress-bar.hpp"
#include "widget/radio-button.hpp"
#include "widget/radio-label.hpp"
#include "widget/source-edit.hpp"
#include "widget/tab-frame.hpp"
#include "widget/tab-frame-item.hpp"
#include "widget/text-edit.hpp"
#include "widget/tree-view.hpp"
#include "widget/tree-view-item.hpp"
#include "widget/vertical-scroller.hpp"
#include "widget/vertical-slider.hpp"
#include "widget/viewport.hpp"

#undef Declare

#include "application.hpp"
#include "settings.hpp"
