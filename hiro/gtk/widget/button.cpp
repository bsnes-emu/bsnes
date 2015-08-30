#if defined(Hiro_Button)

namespace hiro {

static auto Button_activate(GtkButton* gtkButton, pButton* p) -> void { p->_doActivate(); }

auto pButton::construct() -> void {
  gtkWidget = gtk_button_new();
  gtkButton = GTK_BUTTON(gtkWidget);

  setBordered(state().bordered);
  setImage(state().image);
  setOrientation(state().orientation);
  setText(state().text);

  g_signal_connect(G_OBJECT(gtkButton), "clicked", G_CALLBACK(Button_activate), (gpointer)this);

  pWidget::construct();
}

auto pButton::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pButton::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().image.width());
    size.setHeight(max(size.height(), state().image.height()));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().image.width()));
    size.setHeight(size.height() + state().image.height());
  }

  return {size.width() + (state().text ? 24 : 12), size.height() + 12};
}

auto pButton::setBordered(bool bordered) -> void {
  gtk_button_set_relief(gtkButton, bordered ? GTK_RELIEF_NORMAL : GTK_RELIEF_NONE);
}

auto pButton::setImage(const Image& image) -> void {
  if(image) {
    auto gtkImage = CreateImage(image);
    gtk_button_set_image(gtkButton, (GtkWidget*)gtkImage);
  } else {
    gtk_button_set_image(gtkButton, nullptr);
  }
}

auto pButton::setOrientation(Orientation orientation) -> void {
  switch(orientation) {
  case Orientation::Horizontal: gtk_button_set_image_position(gtkButton, GTK_POS_LEFT); break;
  case Orientation::Vertical:   gtk_button_set_image_position(gtkButton, GTK_POS_TOP);  break;
  }
}

auto pButton::setText(const string& text) -> void {
  gtk_button_set_label(gtkButton, text);
  setFont(self().font(true));  //gtk_button_set_label() recreates label, which destroys currently assigned font
}

auto pButton::_doActivate() -> void {
  self().doActivate();
}

}

#endif
