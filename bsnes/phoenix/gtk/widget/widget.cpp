Geometry pWidget::minimumGeometry() {
  return { 0, 0, 0, 0 };
}

bool pWidget::enabled() {
  return gtk_widget_get_sensitive(gtkWidget);
}

void pWidget::setEnabled(bool enabled) {
  if(widget.state.abstract) enabled = false;
  if(sizable.state.layout && sizable.state.layout->enabled() == false) enabled = false;
  gtk_widget_set_sensitive(gtkWidget, enabled);
}

void pWidget::setFocused() {
  gtk_widget_grab_focus(gtkWidget);
}

void pWidget::setFont(const string &font) {
  pFont::setFont(gtkWidget, font);
}

void pWidget::setGeometry(const Geometry &geometry) {
  if(sizable.window() && sizable.window()->visible()) gtk_fixed_move(GTK_FIXED(sizable.window()->p.formContainer), gtkWidget, geometry.x, geometry.y);
  unsigned width = (signed)geometry.width <= 0 ? 1U : geometry.width;
  unsigned height = (signed)geometry.height <= 0 ? 1U : geometry.height;
  gtk_widget_set_size_request(gtkWidget, width, height);
}

void pWidget::setVisible(bool visible) {
  if(widget.state.abstract) visible = false;
  if(sizable.state.layout && sizable.state.layout->visible() == false) visible = false;
  gtk_widget_set_visible(gtkWidget, visible);
}

void pWidget::constructor() {
  if(widget.state.abstract) gtkWidget = gtk_label_new("");
}

void pWidget::destructor() {
  if(widget.state.abstract) gtk_widget_destroy(gtkWidget);
}

void pWidget::orphan() {
  destructor();
  constructor();
}
