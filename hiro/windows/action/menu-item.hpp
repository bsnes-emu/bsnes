#if defined(Hiro_MenuItem)

namespace hiro {

struct pMenuItem : pAction {
  Declare(MenuItem, Action)

  auto setIcon(const image& icon) -> void;
  auto setText(const string& text) -> void;

  auto onActivate() -> void;

  auto _createBitmap() -> void;

  HBITMAP hbitmap = 0;
};

}

#endif
