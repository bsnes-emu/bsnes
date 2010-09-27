static void RadioBox_tick(RadioBox *self) {
  if(self->onTick && self->checked() && self->object->locked == false) self->onTick();
}

void RadioBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  first = this;
  object->parentWindow = &parent;
  object->widget = gtk_radio_button_new_with_label(0, text);
  widget->parent = &parent;
  gtk_widget_set_size_request(object->widget, width, height);
  g_signal_connect_swapped(G_OBJECT(object->widget), "toggled", G_CALLBACK(RadioBox_tick), (gpointer)this);
  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

void RadioBox::create(RadioBox &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  first = parent.first;
  object->parentWindow = parent.object->parentWindow;
  object->widget = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(parent.object->widget), text);
  gtk_widget_set_size_request(object->widget, width, height);
  g_signal_connect_swapped(G_OBJECT(object->widget), "toggled", G_CALLBACK(RadioBox_tick), (gpointer)this);
  if(object->parentWindow->window->defaultFont) setFont(*object->parentWindow->window->defaultFont);
  gtk_fixed_put(GTK_FIXED(object->parentWindow->object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

bool RadioBox::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(object->widget));
}

void RadioBox::setChecked() {
  object->locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(object->widget), true);
  object->locked = false;
}
