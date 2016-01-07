#if defined(Hiro_TabFrame)

namespace hiro {

struct pTabFrame : pWidget {
  Declare(TabFrame, Widget)

  auto append(sTabFrameItem item) -> void;
  auto container(mWidget& widget) -> GtkWidget* override;
  auto remove(sTabFrameItem item) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setItemClosable(unsigned position, bool closable) -> void;
  auto setItemIcon(unsigned position, const image& icon) -> void;
  auto setItemLayout(unsigned position, sLayout layout) -> void;
  auto setItemMovable(unsigned position, bool movable) -> void;
  auto setItemSelected(unsigned position) -> void;
  auto setItemText(unsigned position, const string& text) -> void;
  auto setNavigation(Navigation navigation) -> void;
  auto setVisible(bool visible) -> void override;

  auto _append(shared_pointer<mTabFrameItem> item) -> void;
  auto _remove(shared_pointer<mTabFrameItem> item) -> void;
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
