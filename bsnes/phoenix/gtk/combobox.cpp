void ComboBox_change(ComboBox *self) {
  if(self->object->locked == false && self->onChange) self->onChange();
}

void ComboBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  object->widget = gtk_combo_box_new_text();
  gtk_widget_set_size_request(object->widget, width, height);
  g_signal_connect_swapped(G_OBJECT(object->widget), "changed", G_CALLBACK(ComboBox_change), (gpointer)this);

  if(*text) {
    lstring list;
    list.split("\n", text);
    foreach(item, list) addItem(item);
  }

  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

void ComboBox::reset() {
  object->locked = true;
  for(signed i = counter - 1; i >= 0; i--) {
    gtk_combo_box_remove_text(GTK_COMBO_BOX(object->widget), i);
  }
  object->locked = false;
  counter = 0;
}

void ComboBox::addItem(const char *text) {
  gtk_combo_box_append_text(GTK_COMBO_BOX(object->widget), text);
  if(counter++ == 0) setSelection(0);
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
  counter = 0;
}
