static void Button_tick(Button *self) {
  if(self->onTick) self->onTick();
}

void Button::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  object->widget = gtk_button_new_with_label(text);
  widget->parent = &parent;
  gtk_widget_set_size_request(object->widget, width, height);
  g_signal_connect_swapped(G_OBJECT(object->widget), "clicked", G_CALLBACK(Button_tick), (gpointer)this);
  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}
