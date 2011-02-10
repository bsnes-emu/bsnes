void ComboBox_change(ComboBox *self) {
  if(self->object->locked == false && self->onChange) self->onChange();
}

void ComboBox::reset() {
  object->locked = true;
  for(signed i = comboBox->counter - 1; i >= 0; i--) {
    gtk_combo_box_remove_text(GTK_COMBO_BOX(object->widget), i);
  }
  object->locked = false;
  comboBox->counter = 0;
}

void ComboBox::addItem(const string &text) {
  gtk_combo_box_append_text(GTK_COMBO_BOX(object->widget), text);
  if(comboBox->counter++ == 0) setSelection(0);
}

unsigned ComboBox::selection() {
  return gtk_combo_box_get_active(GTK_COMBO_BOX(object->widget));
}

void ComboBox::setSelection(unsigned item) {
  object->locked = true;
  gtk_combo_box_set_active(GTK_COMBO_BOX(object->widget), item);
  object->locked = false;
}

ComboBox::ComboBox() {
  comboBox = new ComboBox::Data;
  comboBox->counter = 0;
  object->widget = gtk_combo_box_new_text();
  g_signal_connect_swapped(G_OBJECT(object->widget), "changed", G_CALLBACK(ComboBox_change), (gpointer)this);
}
