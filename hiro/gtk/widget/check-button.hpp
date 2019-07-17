#if defined(Hiro_CheckButton)

namespace hiro {

struct pCheckButton : pWidget {
  Declare(CheckButton, Widget)

  auto minimumSize() const -> Size override;
  auto setBordered(bool bordered) -> void;
  auto setChecked(bool checked) -> void;
  auto setIcon(const image& icon) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;
};

}

#endif
