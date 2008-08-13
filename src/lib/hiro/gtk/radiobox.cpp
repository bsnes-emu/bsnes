void hiro_pradiobox_tick(pRadiobox *p) {
  //GTK+ sends two messages: one for the activated radiobox,
  //and one for the deactivated radiobox. ignore the latter.
  if(!p->locked && p->checked() && p->self.on_tick) p->self.on_tick(event_t(event_t::Tick, 0, &p->self));
}

void pRadiobox::create(RadioboxGroup &group, unsigned style, unsigned width, unsigned height, const char *text) {
  if(group.size() == 0 || group[0] == &self) {
    radiobox = gtk_radio_button_new_with_label(0, text ? text : "");
  } else {
    radiobox = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(group[0]->p.gtk_handle()), text ? text : "");
  }
  set_default_font(radiobox);
  gtk_widget_set_size_request(radiobox, width, height);
  gtk_widget_show(radiobox);
  g_signal_connect_swapped(G_OBJECT(radiobox), "toggled", G_CALLBACK(hiro_pradiobox_tick), (gpointer)this);
}

void pRadiobox::set_text(const char *text) {
  if(!radiobox) return;
  gtk_button_set_label(GTK_BUTTON(radiobox), text ? text : "");
}

void pRadiobox::check() {
  locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobox), TRUE);
  locked = false;
}

bool pRadiobox::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobox));
}

pRadiobox::pRadiobox(Radiobox &self_) : pFormControl(self), self(self_) {
  radiobox = 0;
  locked = false;
}

/* internal */

GtkWidget* pRadiobox::gtk_handle() {
  return radiobox;
}
