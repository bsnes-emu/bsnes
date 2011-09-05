static void CheckBox_tick(CheckBox *self) {
  self->state.checked = self->checked();
  if(self->p.locked == false && self->onTick) self->onTick();
}

bool pCheckBox::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkWidget));
}

Geometry pCheckBox::minimumGeometry() {
  Geometry geometry = pFont::geometry(widget.state.font, checkBox.state.text);
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

  setChecked(checkBox.state.checked);
  setText(checkBox.state.text);
}

void pCheckBox::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pCheckBox::orphan() {
  destructor();
  constructor();
}
