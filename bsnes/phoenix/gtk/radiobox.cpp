static void RadioBox_tick(RadioBox *self) {
  if(self->onTick && self->checked() && self->object->locked == false) self->onTick();
}

void RadioBox::setParent(Layout &parent) {
  first = this;
}

void RadioBox::setParent(RadioBox &parent) {
  first = parent.first;
}

void RadioBox::setText(const string &text) {
  gtk_button_set_label(GTK_BUTTON(object->widget), text);
}

bool RadioBox::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(object->widget));
}

void RadioBox::setChecked() {
  object->locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(object->widget), true);
  object->locked = false;
}

RadioBox::RadioBox() {
  object->widget = gtk_radio_button_new_with_label(0, "");
  g_signal_connect_swapped(G_OBJECT(object->widget), "toggled", G_CALLBACK(RadioBox_tick), (gpointer)this);
}
