namespace hiro {

struct pFont;
struct pObject;
struct pWindow;
struct pMenu;
struct pLayout;
struct pWidget;

struct AppMessage {
  enum : unsigned {
    None = WM_APP,
    ListView_doPaint,
    ListView_onActivate,
    ListView_onChange,
  };
};

using WindowProc = auto CALLBACK (*)(HWND, UINT, WPARAM, LPARAM) -> LRESULT;

static vector<wObject> windows;

}

#define Declare(Name, Base) \
  p##Name(m##Name& reference) : p##Base(reference) {} \
  auto self() const -> m##Name& { return (m##Name&)reference; } \
  auto state() const -> m##Name::State& { return self().state; } \
  auto construct() -> void override; \
  auto destruct() -> void override; \
  auto reconstruct() -> void override { destruct(), construct(); } \

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
#include "widget/frame.hpp"
#include "widget/hex-edit.hpp"
#include "widget/horizontal-scroll-bar.hpp"
#include "widget/horizontal-slider.hpp"
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
#include "widget/tab-frame.hpp"
#include "widget/tab-frame-item.hpp"
#include "widget/text-edit.hpp"
#include "widget/vertical-scroll-bar.hpp"
#include "widget/vertical-slider.hpp"
#include "widget/viewport.hpp"

#include "application.hpp"

#undef Declare
