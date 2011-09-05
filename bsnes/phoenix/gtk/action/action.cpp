void pAction::setEnabled(bool enabled) {
  gtk_widget_set_sensitive(widget, enabled);
}

void pAction::setFont(const string &font) {
  pFont::setFont(widget, font);
}

void pAction::setVisible(bool visible) {
  gtk_widget_set_visible(widget, visible);
}

void pAction::constructor() {
}

void pAction::orphan() {
}
