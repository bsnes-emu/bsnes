namespace hiro {

auto pWidget::construct() -> void {
  if(!gtkWidget) return;
  if(auto window = self().parentWindow(true)) {
    if(window->self()) window->self()->_append(self());
    setFont(self().font(true));
    setVisible(self().visible(true));
  }
}

auto pWidget::destruct() -> void {
}

auto pWidget::container(mWidget& widget) -> GtkWidget* {
  return nullptr;
}

auto pWidget::focused() const -> bool {
  if(!gtkWidget) return false;
  return GTK_WIDGET_HAS_FOCUS(gtkWidget);
}

auto pWidget::setEnabled(bool enabled) -> void {
  if(!gtkWidget) return;
  gtk_widget_set_sensitive(gtkWidget, enabled);
}

auto pWidget::setFocused() -> void {
  if(!gtkWidget) return;
  gtk_widget_grab_focus(gtkWidget);
}

auto pWidget::setFont(const string& font) -> void {
  if(!gtkWidget) return;
  return pFont::setFont(gtkWidget, font);
}

auto pWidget::setGeometry(Geometry geometry) -> void {
  if(!gtkWidget) return;
  if(gtkParent) gtk_fixed_move(GTK_FIXED(gtkParent), gtkWidget, geometry.x(), geometry.y());
  gtk_widget_set_size_request(gtkWidget, max(1, geometry.width()), max(1, geometry.height()));
  self().doSize();
}

auto pWidget::setVisible(bool visible) -> void {
  if(!gtkWidget) return;
  gtk_widget_set_visible(gtkWidget, visible);
}

}
