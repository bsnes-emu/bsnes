#if defined(Hiro_Menu)

namespace hiro {

struct pMenu : public pAction {
  Declare(Menu, Action)

  auto append(sAction action) -> void;
  auto remove(sAction action) -> void;
  auto setFont(const string& font) -> void override;
  auto setIcon(const image& icon) -> void;
  auto setText(const string& text) -> void;

  auto _setState() -> void override;

  QMenu* qtMenu = nullptr;
};

}

#endif
