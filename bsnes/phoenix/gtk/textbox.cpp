static void TextBox_activate(TextBox *self) {
  if(self->onActivate) self->onActivate();
}

static void TextBox_change(TextBox *self) {
  if(self->object->locked == false && self->onChange) self->onChange();
}

void TextBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  object->widget = gtk_entry_new();
  widget->parent = &parent;
  gtk_entry_set_text(GTK_ENTRY(object->widget), text);
  gtk_widget_set_size_request(object->widget, width, height);
  g_signal_connect_swapped(G_OBJECT(object->widget), "activate", G_CALLBACK(TextBox_activate), (gpointer)this);
  g_signal_connect_swapped(G_OBJECT(object->widget), "changed", G_CALLBACK(TextBox_change), (gpointer)this);
  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

void TextBox::setEditable(bool editable) {
  gtk_entry_set_editable(GTK_ENTRY(object->widget), editable);
}

string TextBox::text() {
  return gtk_entry_get_text(GTK_ENTRY(object->widget));
}

void TextBox::setText(const string &text) {
  object->locked = true;
  gtk_entry_set_text(GTK_ENTRY(object->widget), text);
  object->locked = false;
}
