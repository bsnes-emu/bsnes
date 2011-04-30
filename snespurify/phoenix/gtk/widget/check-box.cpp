static void CheckBox_tick(CheckBox *self) {
  if(self->p.locked == false && self->onTick) self->onTick();
}

bool pCheckBox::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkWidget));
}

Geometry pCheckBox::minimumGeometry() {
  Font &font = pWidget::font();
  Geometry geometry = font.geometry(checkBox.state.text);
  return { 0, 0, geometry.width + 28, geometry.height + 4 };
}

void pCheckBox::setChecked(bool checked) {
  locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), checked);
  locked = false;
}

void pCheckBox::setText(const string &text) {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
}

void pCheckBox::constructor() {
  gtkWidget = gtk_check_button_new_with_label("");
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(CheckBox_tick), (gpointer)&checkBox);
}
