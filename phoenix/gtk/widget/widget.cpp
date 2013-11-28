namespace phoenix {

GtkWidget* pWidget::container(Widget& widget) {
  return nullptr;
}

bool pWidget::focused() {
  return GTK_WIDGET_HAS_FOCUS(gtkWidget);
}

Size pWidget::minimumSize() {
  return {0, 0};
}

void pWidget::setEnabled(bool enabled) {
  if(!widget.parent()) enabled = false;
  if(widget.state.abstract) enabled = false;
  if(!widget.enabledToAll()) enabled = false;
  gtk_widget_set_sensitive(gtkWidget, enabled);
}

void pWidget::setFocused() {
  gtk_widget_grab_focus(gtkWidget);
}

void pWidget::setFont(string font) {
  pFont::setFont(gtkWidget, font);
}

void pWidget::setGeometry(Geometry geometry) {
  Position displacement = GetDisplacement(&widget);
  geometry.x -= displacement.x;
  geometry.y -= displacement.y;

  if(gtkParent) gtk_fixed_move(GTK_FIXED(gtkParent), gtkWidget, geometry.x, geometry.y);
  unsigned width = (signed)geometry.width <= 0 ? 1u : geometry.width;
  unsigned height = (signed)geometry.height <= 0 ? 1u : geometry.height;
  gtk_widget_set_size_request(gtkWidget, width, height);
  if(widget.onSize) widget.onSize();
}

void pWidget::setVisible(bool visible) {
  if(!widget.parent()) visible = false;
  if(widget.state.abstract) visible = false;
  if(!widget.visibleToAll()) visible = false;
  gtk_widget_set_visible(gtkWidget, visible);
}

void pWidget::constructor() {
  if(widget.state.abstract) gtkWidget = gtk_fixed_new();
}

void pWidget::destructor() {
  if(widget.state.abstract) gtk_widget_destroy(gtkWidget);
}

void pWidget::orphan() {
  destructor();
  constructor();
}

}
