#if defined(Hiro_Label)

namespace hiro {

auto pLabel::construct() -> void {
  gtkWidget = gtk_event_box_new();
  subWidget = gtk_label_new("");
  gtk_widget_show(subWidget);
  gtk_container_add(GTK_CONTAINER(gtkWidget), subWidget);

  setAlignment(state().alignment);
  setBackgroundColor(state().backgroundColor);
  setForegroundColor(state().foregroundColor);
  setText(state().text);

  pWidget::construct();
}

auto pLabel::destruct() -> void {
  gtk_widget_destroy(subWidget);
  gtk_widget_destroy(gtkWidget);
}

auto pLabel::minimumSize() const -> Size {
  auto size = pFont::size(self().font(true), state().text);
  return {size.width(), size.height()};
}

auto pLabel::setAlignment(Alignment alignment) -> void {
  if(!alignment) alignment = {0.0, 0.5};
  gtk_misc_set_alignment(GTK_MISC(subWidget), alignment.horizontal(), alignment.vertical());
  auto justify = GTK_JUSTIFY_CENTER;
  if(alignment.horizontal() < 0.333) justify = GTK_JUSTIFY_LEFT;
  if(alignment.horizontal() > 0.666) justify = GTK_JUSTIFY_RIGHT;
  gtk_label_set_justify(GTK_LABEL(subWidget), justify);
}

auto pLabel::setBackgroundColor(Color color) -> void {
  if(!color) color = self().parentWindow(true)->backgroundColor();
  auto gdkColor = CreateColor(color);
  gtk_widget_modify_bg(gtkWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pLabel::setForegroundColor(Color color) -> void {
  auto gdkColor = CreateColor(color);
  gtk_widget_modify_fg(subWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pLabel::setText(const string& text) -> void {
  gtk_label_set_text(GTK_LABEL(subWidget), text);
}

auto pLabel::setVisible(bool visible) -> void {
  setBackgroundColor(state().backgroundColor);
  return pWidget::setVisible(visible);
}

}

#endif
