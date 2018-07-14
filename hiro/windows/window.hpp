#if defined(Hiro_Window)

namespace hiro {

struct pWindow : pObject {
  Declare(Window, Object)

  auto append(sMenuBar menuBar) -> void;
  auto append(sSizable sizable) -> void;
  auto append(sStatusBar statusBar) -> void;
  auto focused() const -> bool override;
  auto frameMargin() const -> Geometry;
  auto remove(sMenuBar menuBar) -> void;
  auto remove(sSizable sizable) -> void;
  auto remove(sStatusBar statusBar) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setDismissable(bool dismissable) -> void;
  auto setDroppable(bool droppable) -> void;
  auto setEnabled(bool enabled) -> void;
  auto setFocused() -> void;
  auto setFont(const Font& font) -> void override;
  auto setFullScreen(bool fullScreen) -> void;
  auto setGeometry(Geometry geometry) -> void;
  auto setMaximized(bool maximized) -> void;
  auto setMaximumSize(Size size) -> void;
  auto setMinimized(bool minimized) -> void;
  auto setMinimumSize(Size size) -> void;
  auto setModal(bool modal) -> void;
  auto setResizable(bool resizable) -> void;
  auto setTitle(string text) -> void;
  auto setVisible(bool visible) -> void;

  auto onClose() -> void;
  auto onDrop(WPARAM wparam) -> void;
  auto onEraseBackground() -> bool;
  auto onModalBegin() -> void;
  auto onModalEnd() -> void;
  auto onMove() -> void;
  auto onSize() -> void;

  auto _geometry() -> Geometry;
  auto _modalityCount() -> unsigned;
  auto _modalityDisabled() -> bool;
  auto _modalityUpdate() -> void;

  HWND hwnd = nullptr;
  HFONT hstatusfont = nullptr;
  HBRUSH hbrush = nullptr;
  COLORREF hbrushColor = 0;
  Geometry windowedGeometry{128, 128, 256, 256};
};

}

#endif
