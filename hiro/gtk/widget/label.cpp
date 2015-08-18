#if defined(Hiro_Label)

namespace hiro {

auto pLabel::construct() -> void {
  gtkWidget = gtk_label_new("");

  setAlignment(state().alignment);
  setText(state().text);

  pWidget::construct();
}

auto pLabel::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pLabel::minimumSize() const -> Size {
  auto size = pFont::size(self().font(true), state().text);
  return {size.width(), size.height()};
}

auto pLabel::setAlignment(Alignment alignment) -> void {
  if(!alignment) alignment = {0.0, 0.5};
  gtk_misc_set_alignment(GTK_MISC(gtkWidget), alignment.horizontal(), alignment.vertical());
  auto justify = GTK_JUSTIFY_CENTER;
  if(alignment.horizontal() < 0.333) justify = GTK_JUSTIFY_LEFT;
  if(alignment.horizontal() > 0.666) justify = GTK_JUSTIFY_RIGHT;
  gtk_label_set_justify(GTK_LABEL(gtkWidget), justify);
}

auto pLabel::setText(const string& text) -> void {
  gtk_label_set_text(GTK_LABEL(gtkWidget), text);
}

}

#endif
