#if defined(Hiro_Menu)

namespace hiro {

struct pMenu : pAction {
  Declare(Menu, Action)

  auto append(sAction action) -> void;
  auto remove(sAction action) -> void;
  auto setIcon(const image& icon) -> void;
  auto setText(const string& text) -> void;

  auto _createBitmap() -> void;
  auto _update() -> void;

  HMENU hmenu = 0;
  HBITMAP hbitmap = 0;
};

}

#endif
