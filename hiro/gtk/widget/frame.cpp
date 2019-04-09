#if defined(Hiro_Frame)

namespace hiro {

auto pFrame::construct() -> void {
  gtkWidget = gtk_frame_new(nullptr);
  gtkLabel = gtk_label_new("");
  gtk_frame_set_label_widget(GTK_FRAME(gtkWidget), gtkLabel);
  gtk_widget_show(gtkLabel);
  gtkChild = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(gtkWidget), gtkChild);
  gtk_widget_show(gtkChild);

  setText(state().text);

  pWidget::construct();
}

auto pFrame::destruct() -> void {
  gtk_widget_destroy(gtkChild);
  gtk_widget_destroy(gtkLabel);
  gtk_widget_destroy(gtkWidget);
}

auto pFrame::append(sSizable sizable) -> void {
  if(auto sizable = _sizable()) {
    sizable->setFont(sizable->self().font(true));
    sizable->setVisible(sizable->self().visible(true));
  }
}

auto pFrame::container(mWidget& widget) -> GtkWidget* {
  if(auto parent = widget.parentFrame(true)) {
    if(auto self = parent->self()) {
      return self->gtkChild;
    }
  }
  return gtk_widget_get_parent(gtkWidget);
}

auto pFrame::remove(sSizable sizable) -> void {
}

auto pFrame::setEnabled(bool enabled) -> void {
  if(auto sizable = _sizable()) sizable->setEnabled(sizable->self().enabled(true));
  pWidget::setEnabled(enabled);
}

auto pFrame::setFont(const Font& font) -> void {
  if(auto sizable = _sizable()) sizable->setFont(sizable->self().font(true));
  pFont::setFont(gtkLabel, font);
}

auto pFrame::setGeometry(Geometry geometry) -> void {
  if(!state().text) {
    //a frame without a title is generally used as a border box (client edge)
    //remove the excess spacing so that the frame renders around the entire widget
    //todo: it may be better to custom draw the frame in this case to avoid hard-coded offsets
    geometry.setY(geometry.y() - 7);
    geometry.setHeight(geometry.height() + 8);
  }
  //match the dimensions of other client edge widgets (eg GtkTreeView)
  geometry.setWidth(geometry.width() + 1);
  pWidget::setGeometry(geometry);

  if(auto& sizable = state().sizable) {
    Size size = pFont::size(self().font(true), state().text);
    if(!state().text) size.setHeight(18);
    geometry.setPosition({4, 0});
    geometry.setWidth(geometry.width() - 13);
    geometry.setHeight(geometry.height() - (size.height() + 2));
    sizable->setGeometry(geometry);
  }
}

auto pFrame::setText(const string& text) -> void {
  gtk_label_set_text(GTK_LABEL(gtkLabel), text);
  setGeometry(self().geometry());
}

auto pFrame::setVisible(bool visible) -> void {
  if(auto& sizable = state().sizable) sizable->setVisible(visible);
  pWidget::setVisible(visible);
}

auto pFrame::_sizable() -> maybe<pSizable&> {
  if(auto& sizable = state().sizable) {
    if(auto self = sizable->self()) return *self;
  }
  return {};
}

}

#endif
