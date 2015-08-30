#if defined(Hiro_MenuItem)

namespace hiro {

struct pMenuItem : pAction {
  Declare(MenuItem, Action)

  auto setImage(const Image& image) -> void;
  auto setText(const string& text) -> void;
};

}

#endif
