#if defined(Hiro_Button)

namespace hiro {

struct pButton : pWidget {
  Declare(Button, Widget)

  auto minimumSize() const -> Size override;
  auto setBordered(bool bordered) -> void;
  auto setImage(const Image& image) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;

  QtButton* qtButton = nullptr;
};

}

#endif
