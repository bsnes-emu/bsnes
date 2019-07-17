#if defined(Hiro_SourceEdit)

namespace hiro {

static auto SourceEdit_change(GtkTextBuffer*, pSourceEdit* p) -> void {
  if(!p->locked()) p->self().doChange();
}

static auto SourceEdit_move(GObject*, GParamSpec*, pSourceEdit* p) -> void {
  p->state().textCursor = p->textCursor();
  if(!p->locked()) p->self().doMove();
}

auto pSourceEdit::construct() -> void {
  gtkScrolledWindow = (GtkScrolledWindow*)gtk_scrolled_window_new(0, 0);
  gtkContainer = GTK_CONTAINER(gtkScrolledWindow);
  gtkWidget = GTK_WIDGET(gtkScrolledWindow);
  gtk_scrolled_window_set_policy(gtkScrolledWindow, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(gtkScrolledWindow, GTK_SHADOW_ETCHED_IN);

  gtkSourceLanguageManager = gtk_source_language_manager_get_default();
  gtkSourceLanguage = gtk_source_language_manager_get_language(gtkSourceLanguageManager, "");

  gtkSourceStyleSchemeManager = gtk_source_style_scheme_manager_get_default();
  gtkSourceStyleScheme = gtk_source_style_scheme_manager_get_scheme(gtkSourceStyleSchemeManager, "classic");

  gtkSourceBuffer = gtk_source_buffer_new(nullptr);
  gtkTextBuffer = GTK_TEXT_BUFFER(gtkSourceBuffer);
  gtk_source_buffer_set_highlight_matching_brackets(gtkSourceBuffer, true);
  gtk_source_buffer_set_highlight_syntax(gtkSourceBuffer, true);
  gtk_source_buffer_set_language(gtkSourceBuffer, gtkSourceLanguage);
  gtk_source_buffer_set_style_scheme(gtkSourceBuffer, gtkSourceStyleScheme);

  gtkSourceView = (GtkSourceView*)gtk_source_view_new_with_buffer(gtkSourceBuffer);
  gtkTextView = GTK_TEXT_VIEW(gtkSourceView);
  gtkWidgetSourceView = GTK_WIDGET(gtkSourceView);
  gtk_source_view_set_auto_indent(gtkSourceView, false);
  gtk_source_view_set_draw_spaces(gtkSourceView, (GtkSourceDrawSpacesFlags)0);
  gtk_source_view_set_highlight_current_line(gtkSourceView, true);
  gtk_source_view_set_indent_on_tab(gtkSourceView, false);
  gtk_source_view_set_indent_width(gtkSourceView, 4);
  gtk_source_view_set_insert_spaces_instead_of_tabs(gtkSourceView, false);
  gtk_source_view_set_right_margin_position(gtkSourceView, 80);
  gtk_source_view_set_show_line_marks(gtkSourceView, false);
  gtk_source_view_set_show_line_numbers(gtkSourceView, true);
  gtk_source_view_set_show_right_margin(gtkSourceView, true);
  gtk_source_view_set_smart_home_end(gtkSourceView, GTK_SOURCE_SMART_HOME_END_DISABLED);
  gtk_source_view_set_tab_width(gtkSourceView, 4);
  gtk_container_add(gtkContainer, gtkWidgetSourceView);
  gtk_widget_show(gtkWidgetSourceView);

  setEditable(state().editable);
  setLanguage(state().language);
  setNumbered(state().numbered);
  setScheme(state().scheme);
  setText(state().text);
  setTextCursor(state().textCursor);
  setWordWrap(state().wordWrap);

  g_signal_connect(G_OBJECT(gtkSourceBuffer), "changed", G_CALLBACK(SourceEdit_change), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkSourceBuffer), "notify::cursor-position", G_CALLBACK(SourceEdit_move), (gpointer)this);

  pWidget::construct();
}

auto pSourceEdit::destruct() -> void {
  state().text = text();
  gtk_widget_destroy(gtkWidgetSourceView);
  gtk_widget_destroy(gtkWidget);
}

auto pSourceEdit::setEditable(bool editable) -> void {
  gtk_text_view_set_editable(gtkTextView, editable);
}

auto pSourceEdit::setFocused() -> void {
  gtk_widget_grab_focus(gtkWidgetSourceView);
}

auto pSourceEdit::setLanguage(const string& language) -> void {
  string name;
  if(language == "C") name = "c";
  if(language == "C++") name = "cpp";
  if(language == "Makefile") name = "makefile";
  gtkSourceLanguage = gtk_source_language_manager_get_language(gtkSourceLanguageManager, name);
  gtk_source_buffer_set_language(gtkSourceBuffer, gtkSourceLanguage);
}

auto pSourceEdit::setNumbered(bool numbered) -> void {
  gtk_source_view_set_show_line_numbers(gtkSourceView, numbered);
}

auto pSourceEdit::setScheme(const string& requestedScheme) -> void {
  auto scheme = requestedScheme ? requestedScheme : "classic";
  gtkSourceStyleScheme = gtk_source_style_scheme_manager_get_scheme(gtkSourceStyleSchemeManager, scheme.downcase());
  if(!gtkSourceStyleScheme) gtkSourceStyleScheme = gtk_source_style_scheme_manager_get_scheme(gtkSourceStyleSchemeManager, "classic");
  gtk_source_buffer_set_style_scheme(gtkSourceBuffer, gtkSourceStyleScheme);
}

auto pSourceEdit::setText(const string& text) -> void {
  lock();
  //prevent Ctrl+Z from undoing the newly assigned text ...
  //for instance, a text editor widget setting the initial document here
  gtk_source_buffer_begin_not_undoable_action(gtkSourceBuffer);
  gtk_text_buffer_set_text(gtkTextBuffer, text, -1);
  gtk_source_buffer_end_not_undoable_action(gtkSourceBuffer);
  unlock();
}

auto pSourceEdit::setTextCursor(TextCursor cursor) -> void {
  lock();
  GtkTextIter offset, length;
  gtk_text_buffer_get_end_iter(gtkTextBuffer, &offset);
  gtk_text_buffer_get_end_iter(gtkTextBuffer, &length);
  signed end = gtk_text_iter_get_offset(&offset);
  gtk_text_iter_set_offset(&offset, max(0, min(end, cursor.offset())));
  gtk_text_iter_set_offset(&length, max(0, min(end, cursor.offset() + cursor.length())));
  gtk_text_buffer_select_range(gtkTextBuffer, &offset, &length);
  auto mark = gtk_text_buffer_get_mark(gtkTextBuffer, "insert");
  gtk_text_view_scroll_mark_onscreen(gtkTextView, mark);
  unlock();
}

auto pSourceEdit::setWordWrap(bool wordWrap) -> void {
  gtk_text_view_set_wrap_mode(gtkTextView, wordWrap ? GTK_WRAP_WORD_CHAR : GTK_WRAP_NONE);
  gtk_scrolled_window_set_policy(gtkScrolledWindow, wordWrap ? GTK_POLICY_NEVER : GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
}

auto pSourceEdit::text() const -> string {
  GtkTextIter startIter;
  gtk_text_buffer_get_start_iter(gtkTextBuffer, &startIter);

  GtkTextIter endIter;
  gtk_text_buffer_get_end_iter(gtkTextBuffer, &endIter);

  char* textBuffer = gtk_text_buffer_get_text(gtkTextBuffer, &startIter, &endIter, true);
  string text = textBuffer;
  g_free(textBuffer);
  return text;
}

auto pSourceEdit::textCursor() const -> TextCursor {
  TextCursor cursor;
  int offset = 0;
  g_object_get(G_OBJECT(gtkSourceBuffer), "cursor-position", &offset, nullptr);
  cursor.setOffset(offset);
  GtkTextIter start, end;
  if(gtk_text_buffer_get_selection_bounds(gtkTextBuffer, &start, &end)) {
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
