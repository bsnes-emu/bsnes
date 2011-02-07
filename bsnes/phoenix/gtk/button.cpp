static void Button_tick(Button *self) {
  if(self->onTick) self->onTick();
}

void Button::setParent(Layout &parent) {
}

void Button::setText(const string &text) {
  gtk_button_set_label(GTK_BUTTON(object->widget), text);
}

Button::Button() {
  object->widget = gtk_button_new();
  g_signal_connect_swapped(G_OBJECT(object->widget), "clicked", G_CALLBACK(Button_tick), (gpointer)this);
}
