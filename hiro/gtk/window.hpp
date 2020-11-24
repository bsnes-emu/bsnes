#if defined(Hiro_Window)

namespace hiro {

struct pWindow : pObject {
  Declare(Window, Object)

  auto append(sMenuBar menuBar) -> void;
  auto append(sSizable sizable) -> void;
  auto append(sStatusBar statusBar) -> void;
  auto focused() const -> bool override;
  auto frameMargin() const -> Geometry;
  auto handle() const -> uintptr_t;
  auto monitor() const -> uint;
  auto remove(sMenuBar menuBar) -> void;
  auto remove(sSizable sizable) -> void;
  auto remove(sStatusBar statusBar) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setDismissable(bool dismissable) -> void;
  auto setDroppable(bool droppable) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFocused() -> void override;
  auto setFullScreen(bool fullScreen) -> void;
  auto setGeometry(Geometry geometry) -> void;
  auto setMaximized(bool maximized) -> void;
  auto setMaximumSize(Size size) -> void;
  auto setMinimized(bool minimized) -> void;
  auto setMinimumSize(Size size) -> void;
  auto setModal(bool modal) -> void;
  auto setResizable(bool resizable) -> void;
  auto setTitle(const string& title) -> void;
  auto setVisible(bool visible) -> void override;

  auto _append(mWidget& widget) -> void;
  auto _append(mMenu& menu) -> void;
  auto _menuHeight() const -> int;
  auto _menuTextHeight() const -> int;
  auto _setIcon(const string& basename) -> bool;
  auto _setMenuColor() -> void;
  auto _setMenuEnabled(bool enabled) -> void;
  auto _setMenuFont(const Font& font) -> void;
  auto _setMenuVisible(bool visible) -> void;
  auto _setStatusEnabled(bool enabled) -> void;
  auto _setStatusFont(const Font& font) -> void;
  auto _setStatusText(const string& text) -> void;
  auto _setStatusVisible(bool visible) -> void;
  auto _statusHeight() const -> int;
  auto _statusTextHeight() const -> int;
  auto _synchronizeGeometry() -> void;
  auto _synchronizeMargin() -> void;
  auto _synchronizeState() -> void;

  GtkWidget* widget = nullptr;
  GtkWidget* menuContainer = nullptr;
  GtkWidget* formContainer = nullptr;
  GtkWidget* statusContainer = nullptr;
  GtkWidget* gtkMenu = nullptr;
  GtkWidget* gtkStatus = nullptr;
  GtkAllocation lastMove = {0};
  GtkAllocation lastSize = {0};
  bool screenSaver = true;
};

}

#endif
