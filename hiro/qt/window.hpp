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
  auto setTitle(const string& text) -> void;
  auto setVisible(bool visible) -> void override;

  auto _append(mWidget& widget) -> void;
  auto _menuHeight() const -> uint;
  auto _menuTextHeight() const -> uint;
  auto _statusHeight() const -> uint;
  auto _statusTextHeight() const -> uint;
  auto _updateFrameGeometry() -> void;

  QtWindow* qtWindow = nullptr;
  QVBoxLayout* qtLayout = nullptr;
  QMenuBar* qtMenuBar = nullptr;
  QStatusBar* qtStatusBar = nullptr;
  QWidget* qtContainer = nullptr;
  Geometry windowedGeometry{128, 128, 256, 256};
};

}

#endif
