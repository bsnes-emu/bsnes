namespace hiro {

struct pMenu : pAction {
  Declare(Menu, Action)

  auto append(sAction action) -> void;
  auto remove(sAction action) -> void;
  auto setFont(const string& font) -> void override;
  auto setIcon(const image& icon) -> void;
  auto setText(const string& text) -> void;

  GtkWidget* gtkMenu = nullptr;
};

}
