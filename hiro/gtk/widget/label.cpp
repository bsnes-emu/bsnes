namespace hiro {

auto pLabel::construct() -> void {
  gtkWidget = gtk_label_new("");

  _setAlignment();
  setText(state().text);

  pWidget::construct();
}

auto pLabel::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pLabel::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);
  return {size.width(), size.height()};
}

auto pLabel::setHorizontalAlignment(double alignment) -> void {
  _setAlignment();
}

auto pLabel::setText(const string& text) -> void {
  gtk_label_set_text(GTK_LABEL(gtkWidget), text);
}

auto pLabel::setVerticalAlignment(double alignment) -> void {
  _setAlignment();
}

auto pLabel::_setAlignment() -> void {
  gtk_misc_set_alignment(GTK_MISC(gtkWidget), state().horizontalAlignment, state().verticalAlignment);
  auto justify = GTK_JUSTIFY_CENTER;
  if(state().horizontalAlignment < 0.333) justify = GTK_JUSTIFY_LEFT;
  if(state().horizontalAlignment > 0.666) justify = GTK_JUSTIFY_RIGHT;
  gtk_label_set_justify(GTK_LABEL(gtkWidget), justify);
}

}
