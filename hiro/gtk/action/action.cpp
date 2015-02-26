namespace hiro {

auto pAction::construct() -> void {
}

auto pAction::destruct() -> void {
}

auto pAction::setEnabled(bool enabled) -> void {
  gtk_widget_set_sensitive(widget, enabled);
}

auto pAction::setFont(const string& font) -> void {
  pFont::setFont(widget, font);
}

auto pAction::setVisible(bool visible) -> void {
  gtk_widget_set_visible(widget, visible);
}

//GTK+ uses _ for mnemonics, __ for _
//transform so that & is used for mnemonics, && for &
auto pAction::_mnemonic(string text) -> string {
  text.transform("&_", "\x01\x02");
  text.replace("\x01\x01", "&");
  text.transform("\x01", "_");
  text.replace("\x02", "__");
  return text;
}

}
