#if defined(Hiro_RadioButton)

namespace hiro {

struct pRadioButton : pWidget {
  Declare(RadioButton, Widget)

  auto minimumSize() const -> Size;
  auto setBordered(bool bordered) -> void;
  auto setChecked() -> void;
  auto setGroup(sGroup group) -> void;
  auto setImage(const Image& image) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;

  auto groupLocked() const -> bool;
};

}

#endif
