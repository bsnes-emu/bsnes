static void CheckBox_tick(CheckBox *self) {
  if(self->onTick && self->object->locked == false) self->onTick();
}

void CheckBox::setParent(Layout &parent) {
}

void CheckBox::setText(const string &text) {
  gtk_button_set_label(GTK_BUTTON(object->widget), text);
}

bool CheckBox::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(object->widget));
}

void CheckBox::setChecked(bool checked) {
  object->locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(object->widget), checked);
  object->locked = false;
}

CheckBox::CheckBox() {
  object->widget = gtk_check_button_new_with_label("");
  g_signal_connect_swapped(G_OBJECT(object->widget), "toggled", G_CALLBACK(CheckBox_tick), (gpointer)this);
}
