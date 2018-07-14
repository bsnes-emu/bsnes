#include "settings.hpp"

#define Declare(Name, Base) \
  p##Name(m##Name& reference) : p##Base(reference) {} \
  auto self() const -> m##Name& { return (m##Name&)reference; } \
  auto state() const -> m##Name::State& { return self().state; } \
  auto construct() -> void override; \
  auto destruct() -> void override; \

#include "application.hpp"
#include "font.hpp"
#include "desktop.hpp"
#include "monitor.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "browser-window.hpp"
#include "message-window.hpp"

#include "object.hpp"
#include "group.hpp"

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
#include "widget/progress-bar.hpp"
#include "widget/radio-button.hpp"
#include "widget/radio-label.hpp"
#include "widget/tab-frame.hpp"
#include "widget/tab-frame-item.hpp"
#include "widget/table-view.hpp"
#include "widget/table-view-header.hpp"
#include "widget/table-view-column.hpp"
#include "widget/table-view-item.hpp"
#include "widget/table-view-cell.hpp"
#include "widget/text-edit.hpp"
#include "widget/vertical-scroll-bar.hpp"
#include "widget/vertical-slider.hpp"
#include "widget/viewport.hpp"

namespace hiro {

#if defined(Hiro_Console)
struct pConsole : public QObject, public pWidget {
  Q_OBJECT

public:
  Console& console;
  struct QtConsole : public QTextEdit {
    pConsole& self;
    void keyPressEvent(QKeyEvent*);
    void keyPressEventAcknowledge(QKeyEvent*);
    QtConsole(pConsole& self) : self(self) {}
  };
  QtConsole* qtConsole;

  void print(string text);
  void reset();
  void setBackgroundColor(Color color);
  void setForegroundColor(Color color);
  void setPrompt(string prompt);

  pConsole(Console& console) : pWidget(console), console(console) {}
  void constructor();
  void destructor();
  void orphan();
  void keyPressEvent(QKeyEvent*);
};
#endif

#if defined(Hiro_IconView)
struct pIconView : public QObject, public pWidget {
  Q_OBJECT

public:
  IconView& iconView;
  struct QtListWidget : public QListWidget {
    void resizeEvent(QResizeEvent*);
  };
  QtListWidget* qtIconView;

  void append();
  void remove(unsigned selection);
  void reset();
  void setBackgroundColor(Color color);
  void setFlow(Orientation flow);
  void setForegroundColor(Color color);
  void setImage(unsigned selection, const image& image);
  void setOrientation(Orientation orientation);
  void setSelected(unsigned selection, bool selected);
  void setSelected(const vector<unsigned>& selections);
  void setSelectedAll();
  void setSelectedNone();
  void setSingleSelection(bool singleSelection);
  void setText(unsigned selection, const string& text);

  pIconView(IconView& iconView) : pWidget(iconView), iconView(iconView) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onActivate();
  void onChange();
  void onContext();
};
#endif

#undef Declare

}
