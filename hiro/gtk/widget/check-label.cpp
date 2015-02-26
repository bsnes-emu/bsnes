namespace hiro {

static auto CheckLabel_toggle(GtkToggleButton* toggleButton, pCheckLabel* p) -> void {
  p->state().checked = gtk_toggle_button_get_active(toggleButton);
  if(!p->locked()) p->self().doToggle();
}

auto pCheckLabel::construct() -> void {
  gtkWidget = gtk_check_button_new_with_label("");

  setChecked(state().checked);
  setText(state().text);

  g_signal_connect(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(CheckLabel_toggle), (gpointer)this);

  pWidget::construct();
}

auto pCheckLabel::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pCheckLabel::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);
  return {size.width() + 28, size.height() + 4};
}

auto pCheckLabel::setChecked(bool checked) -> void {
  lock();
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), checked);
  unlock();
}

auto pCheckLabel::setText(const string& text) -> void {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
  setFont(self().font(true));  //gtk_button_set_label() recreates label, which destroys currently assigned font
}

}
