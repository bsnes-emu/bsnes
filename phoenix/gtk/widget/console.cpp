namespace phoenix {

static bool Console_keyPress(GtkWidget* widget, GdkEventKey* event, Console* self) {
  return self->p.keyPress(event->keyval, event->state);
}

void pConsole::print(string text) {
  //insert text before prompt and command
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_line_offset(textBuffer, &iter, gtk_text_buffer_get_line_count(textBuffer), 0);
  gtk_text_buffer_insert(textBuffer, &iter, text, -1);
  seekToEnd();
}

void pConsole::reset() {
  //flush history and redraw prompt
  gtk_text_buffer_set_text(textBuffer, console.prompt(), -1);
  seekToEnd();
}

void pConsole::setBackgroundColor(Color color) {
  GdkColor gdkColor = CreateColor(color.red, color.green, color.blue);
  gtk_widget_modify_base(subWidget, GTK_STATE_NORMAL, &gdkColor);
}

void pConsole::setForegroundColor(Color color) {
  GdkColor gdkColor = CreateColor(color.red, color.green, color.blue);
  gtk_widget_modify_text(subWidget, GTK_STATE_NORMAL, &gdkColor);
}

void pConsole::setPrompt(string prompt) {
  //erase previous prompt and replace it with new prompt
  GtkTextIter lhs, rhs;
  gtk_text_buffer_get_iter_at_line_offset(textBuffer, &lhs, gtk_text_buffer_get_line_count(textBuffer), 0);
  gtk_text_buffer_get_iter_at_line_offset(textBuffer, &rhs, gtk_text_buffer_get_line_count(textBuffer), previousPrompt.size());
  gtk_text_buffer_delete(textBuffer, &lhs, &rhs);
  gtk_text_buffer_get_iter_at_line_offset(textBuffer, &lhs, gtk_text_buffer_get_line_count(textBuffer), 0);
  gtk_text_buffer_insert(textBuffer, &lhs, previousPrompt = prompt, -1);
  seekToEnd();
}

void pConsole::constructor() {
  gtkWidget = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkWidget), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(gtkWidget), GTK_SHADOW_ETCHED_IN);

  subWidget = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(subWidget), false);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(subWidget), GTK_WRAP_NONE);
  gtk_container_add(GTK_CONTAINER(gtkWidget), subWidget);

  textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(subWidget));

  g_signal_connect(G_OBJECT(subWidget), "key-press-event", G_CALLBACK(Console_keyPress), (gpointer)&console);

  gtk_widget_show(subWidget);
}

void pConsole::destructor() {
  gtk_widget_destroy(subWidget);
  gtk_widget_destroy(gtkWidget);
}

void pConsole::orphan() {
  destructor();
  constructor();
}

bool pConsole::keyPress(unsigned scancode, unsigned mask) {
  if(mask & (GDK_CONTROL_MASK | GDK_MOD1_MASK | GDK_SUPER_MASK)) return false;  //allow actions such as Ctrl+C (copy)

  GtkTextMark* mark = gtk_text_buffer_get_mark(textBuffer, "insert");
  GtkTextIter start, cursor, end;
  gtk_text_buffer_get_iter_at_line_offset(textBuffer, &start, gtk_text_buffer_get_line_count(textBuffer), console.prompt().size());
  gtk_text_buffer_get_iter_at_mark(textBuffer, &cursor, mark);
  gtk_text_buffer_get_end_iter(textBuffer, &end);

  if(scancode == GDK_KEY_Return || scancode == GDK_KEY_KP_Enter) {
    char* temp = gtk_text_buffer_get_text(textBuffer, &start, &end, true);
    string s = temp;
    g_free(temp);
    gtk_text_buffer_insert(textBuffer, &end, string{"\n", console.prompt()}, -1);
    if(console.onActivate) console.onActivate(s);
    if(s) history.prepend(s);
    if(history.size() > 128) history.removeLast();
    historyOffset = 0;
    seekToEnd();
    return true;
  }

  if(scancode == GDK_KEY_Up) {
    gtk_text_buffer_delete(textBuffer, &start, &end);
    gtk_text_buffer_get_end_iter(textBuffer, &end);
    if(historyOffset < history.size()) {
      gtk_text_buffer_insert(textBuffer, &end, history[historyOffset++], -1);
    }
    seekToEnd();
    return true;
  }

  if(scancode == GDK_KEY_Down) {
    gtk_text_buffer_delete(textBuffer, &start, &end);
    gtk_text_buffer_get_end_iter(textBuffer, &end);
    if(historyOffset > 0) {
      gtk_text_buffer_insert(textBuffer, &end, history[--historyOffset], -1);
    }
    seekToEnd();
    return true;
  }

  if(scancode == GDK_KEY_Left) {
    if(gtk_text_iter_get_offset(&cursor) <= gtk_text_iter_get_offset(&start)) {
      gtk_text_buffer_place_cursor(textBuffer, &start);
    } else {
      gtk_text_iter_set_offset(&cursor, gtk_text_iter_get_offset(&cursor) - 1);
      gtk_text_buffer_place_cursor(textBuffer, &cursor);
    }
    seekToMark();
    return true;
  }

  if(scancode == GDK_KEY_Right) {
    if(gtk_text_iter_get_offset(&cursor) < gtk_text_iter_get_offset(&start)) {
      gtk_text_buffer_place_cursor(textBuffer, &end);
    } else if(gtk_text_iter_get_offset(&cursor) < gtk_text_iter_get_offset(&end)) {
      gtk_text_iter_set_offset(&cursor, gtk_text_iter_get_offset(&cursor) + 1);
      gtk_text_buffer_place_cursor(textBuffer, &cursor);
    }
    seekToMark();
    return true;
  }

  if(scancode == GDK_KEY_Home) {
    gtk_text_buffer_place_cursor(textBuffer, &start);
    seekToMark();
    return true;
  }

  if(scancode == GDK_KEY_End) {
    gtk_text_buffer_place_cursor(textBuffer, &end);
    seekToMark();
    return true;
  }

  if(scancode == GDK_KEY_BackSpace) {
    if(gtk_text_iter_get_offset(&cursor) <= gtk_text_iter_get_offset(&start)) return true;
    GtkTextIter lhs = cursor;
    gtk_text_iter_set_offset(&lhs, gtk_text_iter_get_offset(&cursor) - 1);
    gtk_text_buffer_delete(textBuffer, &lhs, &cursor);
    seekToMark();
    return true;
  }

  if(scancode == GDK_KEY_Delete) {
    if(gtk_text_iter_get_offset(&cursor) < gtk_text_iter_get_offset(&start)) return true;
    if(gtk_text_iter_get_offset(&cursor) == gtk_text_iter_get_offset(&end)) return true;
    GtkTextIter rhs = cursor;
    gtk_text_iter_set_offset(&rhs, gtk_text_iter_get_offset(&cursor) + 1);
    gtk_text_buffer_delete(textBuffer, &cursor, &rhs);
    seekToMark();
    return true;
  }

  if(scancode >= 0x20 && scancode <= 0x7e) {
    if(gtk_text_iter_get_offset(&cursor) < gtk_text_iter_get_offset(&start)) return true;
    gtk_text_buffer_insert(textBuffer, &cursor, string{(char)scancode}, -1);
    seekToMark();
    return true;
  }

  return false;
}

void pConsole::seekToEnd() {
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(textBuffer, &iter);
  gtk_text_buffer_place_cursor(textBuffer, &iter);
  seekToMark();
}

void pConsole::seekToMark() {
  GtkTextMark* mark = gtk_text_buffer_get_mark(textBuffer, "insert");
  gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(subWidget), mark);
}

}
