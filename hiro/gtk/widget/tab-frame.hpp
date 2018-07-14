#if defined(Hiro_TabFrame)

namespace hiro {

struct pTabFrame : pWidget {
  Declare(TabFrame, Widget)

  auto append(sTabFrameItem item) -> void;
  auto container(mWidget& widget) -> GtkWidget* override;
  auto remove(sTabFrameItem item) -> void;
  auto setFont(const Font& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setItemClosable(unsigned position, bool closable) -> void;
  auto setItemIcon(unsigned position, const image& icon) -> void;
  auto setItemMovable(unsigned position, bool movable) -> void;
  auto setItemSelected(unsigned position) -> void;
  auto setItemSizable(unsigned position, sSizable sizable) -> void;
  auto setItemText(unsigned position, const string& text) -> void;
  auto setNavigation(Navigation navigation) -> void;

  auto _synchronizeLayout() -> void;
  auto _synchronizeTab(unsigned position) -> void;
  auto _tabHeight() -> unsigned;
  auto _tabWidth() -> unsigned;

  struct Tab {
    GtkWidget* child = nullptr;
    GtkWidget* container = nullptr;
    GtkWidget* layout = nullptr;
    GtkWidget* image = nullptr;
    GtkWidget* title = nullptr;
    GtkWidget* close = nullptr;
  };
  vector<Tab> tabs;
};

}

#endif
