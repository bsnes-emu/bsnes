#if defined(Hiro_MenuItem)

namespace hiro {

struct pMenuItem : pAction {
  Declare(MenuItem, Action)

  auto setImage(const Image& image) -> void;
  auto setText(const string& text) -> void;

  auto _setState() -> void override;

  QtMenuItem* qtMenuItem = nullptr;
};

}

#endif
