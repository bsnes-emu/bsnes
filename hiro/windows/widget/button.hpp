#if defined(Hiro_Button)

namespace hiro {

struct pButton : pWidget {
  Declare(Button, Widget)

  auto minimumSize() const -> Size override;
  auto setBordered(bool bordered) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setIcon(const image& icon) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;
  auto setVisible(bool visible) -> void override;

  auto onActivate() -> void;

  auto _setState() -> void;

  WindowProc windowProc = nullptr;
};

}

#endif
