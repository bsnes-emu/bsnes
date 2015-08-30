#if defined(Hiro_Menu)

namespace hiro {

struct pMenu : public pAction {
  Declare(Menu, Action)

  auto append(sAction action) -> void;
  auto remove(sAction action) -> void;
  auto setImage(const Image& image) -> void;
  auto setText(const string& text) -> void;

  auto _setState() -> void override;

  QMenu* qtMenu = nullptr;
};

}

#endif
