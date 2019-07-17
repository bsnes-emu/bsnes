#if defined(Hiro_TextEdit)

namespace hiro {

static auto TextEdit_change(GtkTextBuffer* textBuffer, pTextEdit* p) -> void {
  if(!p->locked()) p->self().doChange();
}

static auto TextEdit_move(GObject* object, GParamSpec* spec, pTextEdit* p) -> void {
  p->state().textCursor = p->textCursor();
  if(!p->locked()) p->self().doMove();
}

auto pTextEdit::construct() -> void {
  gtkWidget = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkWidget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(gtkWidget), GTK_SHADOW_ETCHED_IN);

  subWidget = gtk_text_view_new();
  gtk_widget_show(subWidget);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(subWidget), GTK_WRAP_WORD_CHAR);
  gtk_container_add(GTK_CONTAINER(gtkWidget), subWidget);

  textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(subWidget));

  setBackgroundColor(state().backgroundColor);
  setEditable(state().editable);
  setForegroundColor(state().foregroundColor);
  setText(state().text);
  setTextCursor(state().textCursor);
  setWordWrap(state().wordWrap);

  g_signal_connect(G_OBJECT(textBuffer), "changed", G_CALLBACK(TextEdit_change), (gpointer)this);
  g_signal_connect(G_OBJECT(textBuffer), "notify::cursor-position", G_CALLBACK(TextEdit_move), (gpointer)this);

  pWidget::construct();
}

auto pTextEdit::destruct() -> void {
  state().text = text();
  gtk_widget_destroy(subWidget);
  gtk_widget_destroy(gtkWidget);
}

auto pTextEdit::focused() const -> bool {
  return gtk_widget_has_focus(subWidget);
}

auto pTextEdit::setBackgroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_base(subWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pTextEdit::setEditable(bool editable) -> void {
  gtk_text_view_set_editable(GTK_TEXT_VIEW(subWidget), editable);
}

auto pTextEdit::setFocused() -> void {
  gtk_widget_grab_focus(subWidget);
}

auto pTextEdit::setForegroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_text(subWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pTextEdit::setText(const string& text) -> void {
  lock();
  textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(subWidget));
  gtk_text_buffer_set_text(textBuffer, text, -1);
  unlock();
}

auto pTextEdit::setTextCursor(TextCursor cursor) -> void {
  lock();
  GtkTextIter offset, length;
  gtk_text_buffer_get_end_iter(textBuffer, &offset);
  gtk_text_buffer_get_end_iter(textBuffer, &length);
  signed end = gtk_text_iter_get_offset(&offset);
  gtk_text_iter_set_offset(&offset, max(0, min(end, cursor.offset())));
  gtk_text_iter_set_offset(&length, max(0, min(end, cursor.offset() + cursor.length())));
  gtk_text_buffer_select_range(textBuffer, &offset, &length);
  auto mark = gtk_text_buffer_get_mark(textBuffer, "insert");
  gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(subWidget), mark);
  unlock();
}

auto pTextEdit::setWordWrap(bool wordWrap) -> void {
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(subWidget), wordWrap ? GTK_WRAP_WORD_CHAR : GTK_WRAP_NONE);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkWidget),
    wordWrap ? GTK_POLICY_NEVER : GTK_POLICY_ALWAYS,
    GTK_POLICY_ALWAYS);
}

auto pTextEdit::text() const -> string {
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(textBuffer, &start);
  gtk_text_buffer_get_end_iter(textBuffer, &end);
  char* temp = gtk_text_buffer_get_text(textBuffer, &start, &end, true);
  string text = temp;
  g_free(temp);
  return text;
}

auto pTextEdit::textCursor() const -> TextCursor {
  TextCursor cursor;
  int offset = 0;
  g_object_get(G_OBJECT(textBuffer), "cursor-position", &offset, nullptr);
  cursor.setOffset(offset);
  GtkTextIter start, end;
  if(gtk_text_buffer_get_selection_bounds(textBuffer, &start, &end)) {
    //if selecting text from left to right, the cursor may be ahead of the selection start ...
    //since hiro combines selection bounds (end-start) into length, move the offset to the start
    int origin = gtk_text_iter_get_offset(&start);
    cursor.setOffset(origin);
    int length = gtk_text_iter_get_offset(&end) - origin;
    cursor.setLength(length);
  }
  return cursor;
}

}

#endif
