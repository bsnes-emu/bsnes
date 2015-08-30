#if defined(Hiro_CheckButton)

namespace hiro {

struct pCheckButton : pWidget {
  Declare(CheckButton, Widget)

  auto minimumSize() const -> Size override;
  auto setBordered(bool bordered) -> void;
  auto setChecked(bool checked) -> void;
  auto setImage(const Image& image) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;

  auto _setState() -> void;

  QtCheckButton* qtCheckButton = nullptr;
};

}

#endif
