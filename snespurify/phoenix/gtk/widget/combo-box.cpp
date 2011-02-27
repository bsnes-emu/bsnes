static void ComboBox_change(ComboBox *self) {
  if(self->p.locked == false && self->onChange) self->onChange();
}

void pComboBox::append(const string &text) {
  gtk_combo_box_append_text(GTK_COMBO_BOX(gtkWidget), text);
  if(itemCounter++ == 0) setSelection(0);
}

void pComboBox::reset() {
  locked = true;
  for(signed n = itemCounter - 1; n >= 0; n--) {
    gtk_combo_box_remove_text(GTK_COMBO_BOX(gtkWidget), n);
  }
  itemCounter = 0;
  locked = false;
}

unsigned pComboBox::selection() {
  return gtk_combo_box_get_active(GTK_COMBO_BOX(gtkWidget));
}

void pComboBox::setSelection(unsigned row) {
  locked = true;
  gtk_combo_box_set_active(GTK_COMBO_BOX(gtkWidget), row);
  locked = false;
}

void pComboBox::constructor() {
  itemCounter = 0;
  gtkWidget = gtk_combo_box_new_text();
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "changed", G_CALLBACK(ComboBox_change), (gpointer)&comboBox);
}
