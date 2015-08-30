#if defined(Hiro_Widget)

namespace hiro {

struct pWidget : pSizable {
  Declare(Widget, Sizable)

  auto focused() const -> bool override;
  virtual auto minimumSize() -> Size;
  auto setEnabled(bool enabled) -> void override;
  auto setFocused() -> void;
  auto setFont(const Font& font) -> void override;
  virtual auto setGeometry(Geometry geometry) -> void;
  auto setVisible(bool visible) -> void override;

  auto _parentHandle() -> HWND;
  auto _parentWidget() -> maybe<pWidget&>;
  auto _parentWindow() -> maybe<pWindow&>;
  auto _setState() -> void;

  bool abstract = false;
  HWND hwnd = 0;
  HFONT hfont = 0;
};

}

#endif
