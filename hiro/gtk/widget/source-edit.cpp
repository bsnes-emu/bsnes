namespace hiro {

static auto SourceEdit_change(GtkTextBuffer*, pSourceEdit* p) -> void {
  if(!p->locked()) p->self().doChange();
}

static auto SourceEdit_move(GObject*, GParamSpec*, pSourceEdit* p) -> void {
  signed position = 0;
  g_object_get(G_OBJECT(p->gtkSourceBuffer), "cursor-position", &position, nullptr);

  if(p->state().position != position) {
    p->state().position = position;
    if(!p->locked()) p->self().doMove();
  }
}

auto pSourceEdit::construct() -> void {
  gtkScrolledWindow = (GtkScrolledWindow*)gtk_scrolled_window_new(0, 0);
  gtkContainer = GTK_CONTAINER(gtkScrolledWindow);
  gtkWidget = GTK_WIDGET(gtkScrolledWindow);
  gtk_scrolled_window_set_policy(gtkScrolledWindow, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(gtkScrolledWindow, GTK_SHADOW_ETCHED_IN);

  gtkSourceLanguageManager = gtk_source_language_manager_get_default();
  gtkSourceLanguage = gtk_source_language_manager_get_language(gtkSourceLanguageManager, "cpp");

  gtkSourceStyleSchemeManager = gtk_source_style_scheme_manager_get_default();
  gtkSourceStyleScheme = gtk_source_style_scheme_manager_get_scheme(gtkSourceStyleSchemeManager, "oblivion");

  gtkSourceBuffer = gtk_source_buffer_new(nullptr);
  gtkTextBuffer = GTK_TEXT_BUFFER(gtkSourceBuffer);
  gtk_source_buffer_set_highlight_matching_brackets(gtkSourceBuffer, true);
  gtk_source_buffer_set_highlight_syntax(gtkSourceBuffer, true);
//gtk_source_buffer_set_language(gtkSourceBuffer, gtkSourceLanguage);
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

  setText(state().text);

  g_signal_connect(G_OBJECT(gtkSourceBuffer), "changed", G_CALLBACK(SourceEdit_change), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkSourceBuffer), "notify::cursor-position", G_CALLBACK(SourceEdit_move), (gpointer)this);

  pWidget::construct();
}

auto pSourceEdit::destruct() -> void {
  state().text = text();
  gtk_widget_destroy(gtkWidgetSourceView);
  gtk_widget_destroy(gtkWidget);
}

auto pSourceEdit::setFocused() -> void {
  gtk_widget_grab_focus(gtkWidgetSourceView);
}

auto pSourceEdit::setPosition(signed position) -> void {
  lock();
  GtkTextIter iter;
  //note: iterators must be initialized via get_iter() before calling set_offset()
  gtk_text_buffer_get_end_iter(gtkTextBuffer, &iter);
  if(position >= 0) {
    gtk_text_iter_set_offset(&iter, position);
  } else {
    state().position = gtk_text_iter_get_offset(&iter);
  }
  gtk_text_buffer_place_cursor(gtkTextBuffer, &iter);
  auto mark = gtk_text_buffer_get_mark(gtkTextBuffer, "insert");
  gtk_text_view_scroll_mark_onscreen(gtkTextView, mark);
  unlock();
}

auto pSourceEdit::setSelected(Position selected) -> void {
  lock();
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(gtkTextBuffer, &iter);
  signed offset = gtk_text_iter_get_offset(&iter);
  if(selected.x() < 0 || selected.x() > offset) selected.setX(offset);
  if(selected.y() < 0 || selected.y() > offset) selected.setY(offset);
  state().selected = selected;
  GtkTextIter startIter;
  gtk_text_buffer_get_start_iter(gtkTextBuffer, &startIter);
  gtk_text_iter_set_offset(&startIter, selected.x());
  GtkTextIter endIter;
  gtk_text_buffer_get_end_iter(gtkTextBuffer, &endIter);
  gtk_text_iter_set_offset(&endIter, selected.y());
  gtk_text_buffer_select_range(gtkTextBuffer, &startIter, &endIter);
  unlock();
}

auto pSourceEdit::setText(const string& text) -> void {
  lock();
  gtk_text_buffer_set_text(gtkTextBuffer, text, -1);
  unlock();
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

}
