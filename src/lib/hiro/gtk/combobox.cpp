void hiro_pcombobox_change(pCombobox *p) {
  if(p->self.on_change) p->self.on_change(event_t(event_t::Change, p->get_selection(), &p->self));
}

void pCombobox::create(unsigned style, unsigned width, unsigned height, const char *text) {
  combobox = gtk_combo_box_new_text();
  set_default_font(combobox);
  gtk_widget_set_size_request(combobox, width, height);
  gtk_widget_show(combobox);

  g_signal_connect_swapped(G_OBJECT(combobox), "changed", G_CALLBACK(hiro_pcombobox_change), (gpointer)this);
}

void pCombobox::add_item(const char *text) {
  if(!combobox) return;
  gtk_combo_box_append_text(GTK_COMBO_BOX(combobox), text ? text : "?");
  if(counter++ == 0) set_selection(0);
}

int pCombobox::get_selection() {
  return gtk_combo_box_get_active(GTK_COMBO_BOX(combobox));
}

void pCombobox::set_selection(int index) {
  gtk_combo_box_set_active(GTK_COMBO_BOX(combobox), index);
}

void pCombobox::reset() {
  if(counter == 0) return;
  for(int i = counter - 1; i >= 0; i--) {
    gtk_combo_box_remove_text(GTK_COMBO_BOX(combobox), i);
  }
  counter = 0;
}

pCombobox::pCombobox(Combobox &self_) : pFormControl(self_), self(self_) {
  combobox = 0;
  counter = 0;
}

/* internal */

GtkWidget* pCombobox::gtk_handle() {
  return combobox;
}
