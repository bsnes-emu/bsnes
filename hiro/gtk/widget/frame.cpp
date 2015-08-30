#if defined(Hiro_Frame)

namespace hiro {

auto pFrame::construct() -> void {
  gtkWidget = gtk_frame_new(nullptr);
  gtkLabel = gtk_label_new("");
  gtk_frame_set_label_widget(GTK_FRAME(gtkWidget), gtkLabel);
  gtk_widget_show(gtkLabel);

  setText(state().text);

  pWidget::construct();
}

auto pFrame::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pFrame::append(shared_pointer<mLayout> layout) -> void {
  if(auto layout = _layout()) layout->setFont(layout->self().font(true));
}

auto pFrame::container(mWidget& widget) -> GtkWidget* {
  return gtk_widget_get_parent(gtkWidget);
}

auto pFrame::remove(shared_pointer<mLayout> layout) -> void {
}

auto pFrame::setEnabled(bool enabled) -> void {
  if(auto layout = _layout()) layout->setEnabled(layout->self().enabled(true));
  pWidget::setEnabled(enabled);
}

auto pFrame::setFont(const Font& font) -> void {
  if(auto layout = _layout()) layout->setFont(layout->self().font(true));
  pFont::setFont(gtkLabel, font);
}

auto pFrame::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  if(auto layout = state().layout) {
    Size size = pFont::size(self().font(true), state().text);
    if(!state().text) size.setHeight(10);
    geometry.setX(geometry.x() + 2);
    geometry.setY(geometry.y() + (size.height() - 1));
    geometry.setWidth(geometry.width() - 5);
    geometry.setHeight(geometry.height() - (size.height() + 2));
    layout->setGeometry(geometry);
  }
}

auto pFrame::setText(const string& text) -> void {
  gtk_label_set_text(GTK_LABEL(gtkLabel), text);
}

auto pFrame::setVisible(bool visible) -> void {
  if(auto layout = _layout()) layout->setVisible(layout->self().visible(true));
  pWidget::setVisible(visible);
}

auto pFrame::_layout() -> pLayout* {
  if(auto layout = state().layout) return layout->self();
  return nullptr;
}

}

#endif
