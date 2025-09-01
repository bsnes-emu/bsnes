#if defined(COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif
#include "qt.hpp"
#include "qt.moc"
#if defined(COMPILER_CLANG)
#pragma clang diagnostic pop
#endif
#include "platform.hpp"

#include "utility.cpp"
#include "settings.cpp"

#include "font.cpp"
#include "desktop.cpp"
#include "monitor.cpp"
#include "keyboard.cpp"
#include "mouse.cpp"
#include "browser-window.cpp"
#include "message-window.cpp"

#include "object.cpp"
#include "group.cpp"

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
#include "widget/frame.cpp"
#include "widget/hex-edit.cpp"
#include "widget/horizontal-scroll-bar.cpp"
#include "widget/horizontal-slider.cpp"
#include "widget/label.cpp"
#include "widget/line-edit.cpp"
#include "widget/progress-bar.cpp"
#include "widget/radio-button.cpp"
#include "widget/radio-label.cpp"
#include "widget/tab-frame.cpp"
#include "widget/tab-frame-item.cpp"
#include "widget/table-view.cpp"
#include "widget/table-view-column.cpp"
#include "widget/table-view-item.cpp"
#include "widget/table-view-cell.cpp"
#include "widget/text-edit.cpp"
#include "widget/vertical-scroll-bar.cpp"
#include "widget/vertical-slider.cpp"
#include "widget/viewport.cpp"

#include "application.cpp"
