#if defined(Hiro_Window)

namespace hiro {

struct pWindow : pObject {
  Declare(Window, Object)

  auto append(sLayout layout) -> void;
  auto append(sMenuBar menuBar) -> void;
  auto append(sStatusBar statusBar) -> void;
  auto focused() const -> bool override;
  auto frameMargin() const -> Geometry;
  auto remove(sLayout layout) -> void;
  auto remove(sMenuBar menuBar) -> void;
  auto remove(sStatusBar statusBar) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setDroppable(bool droppable) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFocused() -> void override;
  auto setFullScreen(bool fullScreen) -> void;
  auto setGeometry(Geometry geometry) -> void;
  auto setModal(bool modal) -> void;
  auto setResizable(bool resizable) -> void;
  auto setTitle(const string& text) -> void;
  auto setVisible(bool visible) -> void;

  auto _append(mWidget& widget) -> void;
  auto _menuHeight() const -> signed;
  auto _statusHeight() const -> signed;
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
