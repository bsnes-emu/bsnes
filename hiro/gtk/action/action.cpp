#if defined(Hiro_Action)

namespace hiro {

auto pAction::construct() -> void {
}

auto pAction::destruct() -> void {
}

auto pAction::setEnabled(bool enabled) -> void {
  gtk_widget_set_sensitive(widget, enabled);
}

auto pAction::setFont(const Font& font) -> void {
  pFont::setFont(widget, font);
}

auto pAction::setVisible(bool visible) -> void {
  gtk_widget_set_visible(widget, visible);
}

//GTK+ uses _ for mnemonics, __ for _
auto pAction::_mnemonic(string text) -> string {
  text.replace("_", "__");
  return text;
}

}

#endif
