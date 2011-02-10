static void EditBox_change(EditBox *self) {
  if(self->object->locked == false && self->onChange) self->onChange();
}

void EditBox::setFocused() {
  gtk_widget_grab_focus(object->subWidget);
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

void EditBox::setText(const string &text) {
  object->locked = true;
  gtk_text_buffer_set_text(object->textBuffer, text, -1);
  object->locked = false;
}

void EditBox::setCursorPosition(unsigned position) {
  GtkTextMark *mark = gtk_text_buffer_get_mark(object->textBuffer, "insert");
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(object->textBuffer, &iter);
  gtk_text_iter_set_offset(&iter, min(position, gtk_text_iter_get_offset(&iter)));
  gtk_text_buffer_place_cursor(object->textBuffer, &iter);
  gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(object->subWidget), mark);
}

EditBox::EditBox() {
  object->widget = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(object->widget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(object->widget), GTK_SHADOW_ETCHED_IN);
  object->subWidget = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(object->subWidget), GTK_WRAP_WORD_CHAR);
  gtk_container_add(GTK_CONTAINER(object->widget), object->subWidget);
  object->textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(object->subWidget));
  g_signal_connect_swapped(G_OBJECT(object->textBuffer), "changed", G_CALLBACK(EditBox_change), (gpointer)this);
  gtk_widget_show(object->subWidget);
}
