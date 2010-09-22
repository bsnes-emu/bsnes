static void EditBox_change(EditBox *self) {
  if(self->object->locked == false && self->onChange) self->onChange();
}

void EditBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  object->widget = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(object->widget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(object->widget), GTK_SHADOW_ETCHED_IN);
  gtk_widget_set_size_request(object->widget, width, height);
  object->subWidget = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(object->subWidget), GTK_WRAP_WORD_CHAR);
  gtk_container_add(GTK_CONTAINER(object->widget), object->subWidget);
  object->textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(object->subWidget));
  gtk_text_buffer_set_text(object->textBuffer, text, -1);
  g_signal_connect_swapped(G_OBJECT(object->textBuffer), "changed", G_CALLBACK(EditBox_change), (gpointer)this);
  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->subWidget);
  gtk_widget_show(object->widget);
}

void EditBox::setEditable(bool editable) {
  gtk_text_view_set_editable(GTK_TEXT_VIEW(object->subWidget), editable);
}

void EditBox::setWordWrap(bool wordWrap) {
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(object->subWidget), wordWrap ? GTK_WRAP_WORD_CHAR : GTK_WRAP_NONE);
}

string EditBox::text() {
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(object->textBuffer, &start);
  gtk_text_buffer_get_end_iter(object->textBuffer, &end);
  char *temp = gtk_text_buffer_get_text(object->textBuffer, &start, &end, true);
  string text = temp;
  g_free(temp);
  return text;
}

void EditBox::setText(const char *text) {
  object->locked = true;
  gtk_text_buffer_set_text(object->textBuffer, text, -1);
  object->locked = false;
}
