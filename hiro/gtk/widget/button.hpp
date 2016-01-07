#if defined(Hiro_Button)

namespace hiro {

struct pButton : pWidget {
  Declare(Button, Widget)

  auto minimumSize() const -> Size override;
  auto setBordered(bool bordered) -> void;
  auto setIcon(const image& icon) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;

  auto _doActivate() -> void;

  GtkButton* gtkButton = nullptr;
};

}

#endif
