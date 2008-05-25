void hiro_pcheckbox_tick(pCheckbox *p) {
  if(!p->locked && p->self.on_tick) p->self.on_tick(Event(Event::Tick, p->checked(), &p->self));
}

void pCheckbox::create(uint style, uint width, uint height, const char *text) {
  checkbox = gtk_check_button_new_with_label(text ? text : "");
  set_default_font(checkbox);
  gtk_widget_set_size_request(checkbox, width, height);
  gtk_widget_show(checkbox);
  g_signal_connect_swapped(G_OBJECT(checkbox), "toggled", G_CALLBACK(hiro_pcheckbox_tick), (gpointer)this);
}

void pCheckbox::set_text(const char *text) {
  if(!checkbox) return;
  gtk_button_set_label(GTK_BUTTON(checkbox), text ? text : "");
}

void pCheckbox::check(bool state) {
  locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbox), state ? TRUE : FALSE);
  locked = false;
}

void pCheckbox::uncheck() {
  check(false);
}

bool pCheckbox::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbox));
}

pCheckbox::pCheckbox(Checkbox &self_) : pFormControl(self_), self(self_) {
  checkbox = 0;
  locked = false;
}

/* internal */

GtkWidget* pCheckbox::gtk_handle() {
  return checkbox;
}
