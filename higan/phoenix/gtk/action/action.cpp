namespace phoenix {

void pAction::setEnabled(bool enabled) {
  gtk_widget_set_sensitive(widget, enabled);
}

void pAction::setVisible(bool visible) {
  gtk_widget_set_visible(widget, visible);
}

void pAction::constructor() {
}

void pAction::orphan() {
}

//GTK+ uses _ for mnemonics, __ for _
//transform so that & is used for mnemonics, && for &
string pAction::mnemonic(string text) {
  text.transform("&_", "\x01\x02");
  text.replace("\x01\x01", "&");
  text.transform("\x01", "_");
  text.replace("\x02", "__");
  return text;
}

void pAction::setFont(const string& font) {
  pFont::setFont(widget, font);
}

}
