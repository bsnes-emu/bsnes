namespace hiro {

struct pWindow : pObject {
  Declare(Window, Object)

  GtkWidget* widget = nullptr;
  GtkWidget* menuContainer = nullptr;
  GtkWidget* formContainer = nullptr;
  GtkWidget* statusContainer = nullptr;
  GtkWidget* gtkMenu = nullptr;
  GtkWidget* gtkStatus = nullptr;
  GtkAllocation lastAllocation = {0};
  bool onSizePending = false;

  auto append(shared_pointer<mMenuBar> menuBar) -> void;
  auto append(shared_pointer<mStatusBar> statusBar) -> void;
  auto focused() const -> bool override;
  auto frameMargin() const -> Geometry;
  auto remove(shared_pointer<mMenuBar> menuBar) -> void;
  auto remove(shared_pointer<mStatusBar> statusBar) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setDroppable(bool droppable) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFocused() -> void override;
  auto setFullScreen(bool fullScreen) -> void;
  auto setGeometry(Geometry geometry) -> void;
  auto setModal(bool modal) -> void;
  auto setResizable(bool resizable) -> void;
  auto setTitle(const string& title) -> void;
  auto setVisible(bool visible) -> void override;

  auto _append(mWidget& widget) -> void;
  auto _append(mMenu& menu) -> void;
  auto _menuHeight() const -> signed;
  auto _setIcon(const string& basename) -> bool;
  auto _setMenuEnabled(bool enabled) -> void;
  auto _setMenuFont(const string& font) -> void;
  auto _setMenuVisible(bool visible) -> void;
  auto _setStatusEnabled(bool enabled) -> void;
  auto _setStatusFont(const string& font) -> void;
  auto _setStatusText(const string& text) -> void;
  auto _setStatusVisible(bool visible) -> void;
  auto _statusHeight() const -> signed;
};

}
