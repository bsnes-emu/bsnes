namespace phoenix {

static void TextEdit_change(TextEdit* self) {
  self->state.text = self->text();
  if(self->p.locked == false && self->onChange) self->onChange();
}

bool pTextEdit::focused() {
  return GTK_WIDGET_HAS_FOCUS(subWidget);
}

void pTextEdit::setBackgroundColor(Color color) {
  GdkColor gdkColor = CreateColor(color.red, color.green, color.blue);
  gtk_widget_modify_base(subWidget, GTK_STATE_NORMAL, &gdkColor);
}

void pTextEdit::setCursorPosition(unsigned position) {
  GtkTextMark* mark = gtk_text_buffer_get_mark(textBuffer, "insert");
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(textBuffer, &iter);
  gtk_text_iter_set_offset(&iter, min(position, gtk_text_iter_get_offset(&iter)));
  gtk_text_buffer_place_cursor(textBuffer, &iter);
  gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(subWidget), mark);
}

void pTextEdit::setEditable(bool editable) {
  gtk_text_view_set_editable(GTK_TEXT_VIEW(subWidget), editable);
}

void pTextEdit::setForegroundColor(Color color) {
  GdkColor gdkColor = CreateColor(color.red, color.green, color.blue);
  gtk_widget_modify_text(subWidget, GTK_STATE_NORMAL, &gdkColor);
}

void pTextEdit::setText(string text) {
  locked = true;
  gtk_text_buffer_set_text(textBuffer, text, -1);
  locked = false;
}

void pTextEdit::setWordWrap(bool wordWrap) {
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(subWidget), wordWrap ? GTK_WRAP_WORD_CHAR : GTK_WRAP_NONE);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkWidget),
    wordWrap ? GTK_POLICY_NEVER : GTK_POLICY_ALWAYS,
    GTK_POLICY_ALWAYS);
}

string pTextEdit::text() {
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(textBuffer, &start);
  gtk_text_buffer_get_end_iter(textBuffer, &end);
  char* temp = gtk_text_buffer_get_text(textBuffer, &start, &end, true);
  string text = temp;
  g_free(temp);
  return text;
}

void pTextEdit::constructor() {
  gtkWidget = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkWidget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(gtkWidget), GTK_SHADOW_ETCHED_IN);

  subWidget = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(subWidget), GTK_WRAP_WORD_CHAR);
  gtk_container_add(GTK_CONTAINER(gtkWidget), subWidget);

  textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(subWidget));

  g_signal_connect_swapped(G_OBJECT(textBuffer), "changed", G_CALLBACK(TextEdit_change), (gpointer)&textEdit);

  gtk_widget_show(subWidget);

  setEditable(textEdit.state.editable);
  setText(textEdit.state.text);
  setWordWrap(textEdit.state.wordWrap);
}

void pTextEdit::destructor() {
  gtk_widget_destroy(subWidget);
  gtk_widget_destroy(gtkWidget);
}

void pTextEdit::orphan() {
  destructor();
  constructor();
}

}
