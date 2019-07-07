#if defined(Hiro_Widget)

namespace hiro {

struct pWidget : pSizable {
  Declare(Widget, Sizable)

  auto focused() const -> bool override;
  virtual auto minimumSize() -> Size;
  virtual auto setDroppable(bool droppable) -> void;
  auto setEnabled(bool enabled) -> void override;
  virtual auto setFocusable(bool focusable) -> void;
  auto setFocused() -> void;
  auto setFont(const Font& font) -> void override;
  virtual auto setGeometry(Geometry geometry) -> void;
  auto setMouseCursor(const MouseCursor& mouseCursor) -> void;
  auto setToolTip(const string& toolTip) -> void;
  auto setVisible(bool visible) -> void override;

  virtual auto doMouseHover() -> void;
  virtual auto doMouseLeave() -> void;
  virtual auto doMouseMove(int x, int y) -> void;
  virtual auto windowProc(HWND, UINT, WPARAM, LPARAM) -> maybe<LRESULT>;

  auto _parentHandle() -> HWND;
  auto _parentWidget() -> maybe<pWidget&>;
  auto _parentWindow() -> maybe<pWindow&>;
  auto _setState() -> void;

  bool abstract = false;
  WindowProc defaultWindowProc = nullptr;
  HWND hwnd = nullptr;
  HFONT hfont = nullptr;
  unique_pointer<pToolTip> toolTip;
};

}

#endif
