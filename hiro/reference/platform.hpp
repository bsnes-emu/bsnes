namespace phoenix {
  struct pFont;
  struct pWindow;
  struct pMenu;
  struct pLayout;
  struct pWidget;
}

#include "font.hpp"
#include "desktop.hpp"
#include "monitor.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "browser-window.hpp"
#include "message-window.hpp"
#include "object.hpp"
#include "timer.hpp"
#include "window.hpp"
#include "popup-menu.hpp"

#include "action/action.hpp"
#include "action/menu.hpp"
#include "action/separator.hpp"
#include "action/item.hpp"
#include "action/check-item.hpp"
#include "action/radio-item.hpp"

#include "widget/sizable.hpp"
#include "widget/layout.hpp"
#include "widget/widget.hpp"
#include "widget/button.hpp"
#include "widget/canvas.hpp"
#include "widget/check-button.hpp"
#include "widget/check-label.hpp"
#include "widget/combo-button.hpp"
#include "widget/console.hpp"
#include "widget/frame.hpp"
#include "widget/hex-edit.hpp"
#include "widget/horizontal-scroller.hpp"
#include "widget/horizontal-slider.hpp"
#include "widget/label.hpp"
#include "widget/line-edit.hpp"
#include "widget/list-view.hpp"
#include "widget/progress-bar.hpp"
#include "widget/radio-button.hpp"
#include "widget/radio-label.hpp"
#include "widget/tab-frame.hpp"
#include "widget/text-edit.hpp"
#include "widget/vertical-scroller.hpp"
#include "widget/vertical-slider.hpp"
#include "widget/viewport.hpp"

#include "application.hpp"
