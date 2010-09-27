static void CheckBox_tick(CheckBox *self) {
  if(self->onTick && self->object->locked == false) self->onTick();
}

void CheckBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  object->widget = gtk_check_button_new_with_label(text);
  widget->parent = &parent;
  gtk_widget_set_size_request(object->widget, width, height);
  g_signal_connect_swapped(G_OBJECT(object->widget), "toggled", G_CALLBACK(CheckBox_tick), (gpointer)this);
  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

bool CheckBox::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(object->widget));
}

void CheckBox::setChecked(bool checked) {
  object->locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(object->widget), checked);
  object->locked = false;
}
