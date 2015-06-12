#if defined(Hiro_Widget)

namespace hiro {

struct pWidget : pSizable {
  Declare(Widget, Sizable)

  auto focused() -> bool;
  virtual auto minimumSize() -> Size;
  auto setEnabled(bool enabled) -> void override;
  auto setFocused() -> void;
  auto setFont(const string& font) -> void override;
  virtual auto setGeometry(Geometry geometry) -> void;
  auto setVisible(bool visible) -> void override;

  auto _parentHandle() -> HWND;
  auto _parentWidget() -> maybe<mWidget&>;
  auto _parentWindow() -> maybe<mWindow&>;
  auto _setState() -> void;

  bool abstract = false;
  HWND hwnd = 0;
  HFONT hfont = 0;
};

}

#endif
