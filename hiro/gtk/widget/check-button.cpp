namespace hiro {

static auto CheckButton_toggle(GtkToggleButton* toggleButton, pCheckButton* p) -> void {
  p->state().checked = gtk_toggle_button_get_active(toggleButton);
  if(!p->locked()) p->self().doToggle();
}

auto pCheckButton::construct() -> void {
  gtkWidget = gtk_toggle_button_new();

  setBordered(state().bordered);
  setChecked(state().checked);
  setIcon(state().icon);
  setOrientation(state().orientation);
  setText(state().text);

  g_signal_connect(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(CheckButton_toggle), (gpointer)this);

  pWidget::construct();
}

auto pCheckButton::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pCheckButton::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().icon.width);
    size.setHeight(max(size.height(), state().icon.height));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().icon.width));
    size.setHeight(size.height() + state().icon.height);
  }

  return {size.width() + 24, size.height() + 12};
}

auto pCheckButton::setBordered(bool bordered) -> void {
  gtk_button_set_relief(GTK_BUTTON(gtkWidget), bordered ? GTK_RELIEF_NORMAL : GTK_RELIEF_NONE);
}

auto pCheckButton::setChecked(bool checked) -> void {
  lock();
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), checked);
  unlock();
}

auto pCheckButton::setIcon(const image& icon) -> void {
  if(icon) {
    GtkImage* gtkImage = CreateImage(icon);
    gtk_button_set_image(GTK_BUTTON(gtkWidget), (GtkWidget*)gtkImage);
  } else {
    gtk_button_set_image(GTK_BUTTON(gtkWidget), nullptr);
  }
}

auto pCheckButton::setOrientation(Orientation orientation) -> void {
  switch(orientation) {
  case Orientation::Horizontal: gtk_button_set_image_position(GTK_BUTTON(gtkWidget), GTK_POS_LEFT); break;
  case Orientation::Vertical:   gtk_button_set_image_position(GTK_BUTTON(gtkWidget), GTK_POS_TOP);  break;
  }
}

auto pCheckButton::setText(const string& text) -> void {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
  setFont(self().font(true));  //gtk_button_set_label() recreates label, which destroys currently assigned font
}

}
