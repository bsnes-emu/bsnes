static void Button_tick(Button *self) {
  if(self->onTick) self->onTick();
}

void pButton::setText(const string &text) {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
}

void pButton::constructor() {
  gtkWidget = gtk_button_new();
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "clicked", G_CALLBACK(Button_tick), (gpointer)&button);
}
