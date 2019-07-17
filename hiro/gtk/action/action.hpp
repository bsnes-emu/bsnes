#if defined(Hiro_Action)

namespace hiro {

struct pAction : pObject {
  Declare(Action, Object)

  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setVisible(bool visible) -> void override;

  auto _mnemonic(string text) -> string;

  GtkWidget* widget = nullptr;
};

}

#endif
