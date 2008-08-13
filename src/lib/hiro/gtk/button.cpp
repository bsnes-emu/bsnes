void hiro_pbutton_tick(pButton *p) {
  if(p->self.on_tick) p->self.on_tick(event_t(event_t::Tick, 0, &p->self));
}

void pButton::create(unsigned style, unsigned width, unsigned height, const char *text) {
  button = gtk_button_new_with_label(text ? text : "");
  set_default_font(button);
  gtk_widget_set_size_request(button, width, height);
  gtk_widget_show(button);
  g_signal_connect_swapped(G_OBJECT(button), "clicked", G_CALLBACK(hiro_pbutton_tick), (gpointer)this);
}

void pButton::set_text(const char *text) {
  if(!button) return;
  gtk_button_set_label(GTK_BUTTON(button), text ? text : "");
}

pButton::pButton(Button &self_) : pFormControl(self_), self(self_) {
  button = 0;
}

/* internal */

GtkWidget* pButton::gtk_handle() {
  return button;
}
