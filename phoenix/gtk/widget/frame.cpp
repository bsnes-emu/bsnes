namespace phoenix {

GtkWidget* pFrame::container(Widget& widget) {
  return gtk_widget_get_parent(gtkWidget);
}

Position pFrame::containerOffset() {
  return {0, 0};
}

void pFrame::setEnabled(bool enabled) {
  if(frame.state.layout) frame.state.layout->setEnabled(frame.state.layout->enabled());
  pWidget::setEnabled(enabled);
}

void pFrame::setGeometry(Geometry geometry) {
  pWidget::setGeometry(geometry);
  if(frame.state.layout == nullptr) return;
  Size size = pFont::size(widget.state.font, frame.state.text);
  if(frame.state.text.empty()) size.height = 8;
  geometry.x += 2, geometry.width -= 5;
  geometry.y += size.height - 1, geometry.height -= size.height + 2;
  frame.state.layout->setGeometry(geometry);
}

void pFrame::setText(string text) {
  gtk_frame_set_label(GTK_FRAME(gtkWidget), text);
}

void pFrame::setVisible(bool visible) {
  if(frame.state.layout) frame.state.layout->setVisible(frame.state.layout->visible());
  pWidget::setVisible(visible);
}

void pFrame::constructor() {
  gtkWidget = gtk_frame_new("");
}

void pFrame::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pFrame::orphan() {
  destructor();
  constructor();
}

}
