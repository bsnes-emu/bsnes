#if defined(Hiro_Widget)

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
  return gtk_widget_has_focus(gtkWidget);
}

auto pWidget::setEnabled(bool enabled) -> void {
  if(!gtkWidget) return;
  gtk_widget_set_sensitive(gtkWidget, enabled);
}

auto pWidget::setFocused() -> void {
  if(!gtkWidget) return;
  gtk_widget_grab_focus(gtkWidget);
}

auto pWidget::setFont(const Font& font) -> void {
  if(!gtkWidget) return;
  return pFont::setFont(gtkWidget, font);
}

auto pWidget::setGeometry(Geometry geometry) -> void {
  if(!gtkWidget) return;
  if(gtkParent) gtk_fixed_move(GTK_FIXED(gtkParent), gtkWidget, geometry.x(), geometry.y());
  if(geometry.width()  < 1) geometry.setWidth (1);
  if(geometry.height() < 1) geometry.setHeight(1);
  gtk_widget_set_size_request(gtkWidget, geometry.width(), geometry.height());
  if(gtk_widget_get_realized(gtkWidget)) {
    static bool locked = false;
    if(!locked) {
      locked = true;
      auto time = chrono::millisecond();
      while(chrono::millisecond() - time < 20) {
        gtk_main_iteration_do(false);
        GtkAllocation allocation;
        gtk_widget_get_allocation(gtkWidget, &allocation);
        if(allocation.width  != geometry.width ()) continue;
        if(allocation.height != geometry.height()) continue;
        break;
      }
      locked = false;
    }
  }
  self().doSize();
}

auto pWidget::setVisible(bool visible) -> void {
  if(!gtkWidget) return;
  gtk_widget_set_visible(gtkWidget, visible);
}

}

#endif
