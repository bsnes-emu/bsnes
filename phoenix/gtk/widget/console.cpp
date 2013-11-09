namespace phoenix {

static bool Console_keyPress(GtkWidget* widget, GdkEventKey* event, Console* self) {
  return self->p.keyPress(event->keyval, event->state);
}

void pConsole::print(string text) {
  seekCursorToEnd();
  gtk_text_buffer_insert_at_cursor(textBuffer, text, -1);
  GtkTextMark* mark = gtk_text_buffer_get_mark(textBuffer, "insert");
  gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(subWidget), mark);
}

void pConsole::reset() {
  gtk_text_buffer_set_text(textBuffer, "", -1);
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

  GdkColor background = CreateColor(48, 24, 24);
  gtk_widget_modify_base(subWidget, GTK_STATE_NORMAL, &background);
  GdkColor foreground = CreateColor(255, 255, 255);
  gtk_widget_modify_text(subWidget, GTK_STATE_NORMAL, &foreground);

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

  if(scancode == GDK_KEY_Return || scancode == GDK_KEY_KP_Enter) {
    print("\n");
    if(console.onActivate) console.onActivate(command);
    command.reset();
    return true;
  }

  if(scancode == GDK_KEY_BackSpace) {
    if(command.size()) {
      command.resize(command.size() - 1);
      GtkTextIter lhs, rhs;
      gtk_text_buffer_get_end_iter(textBuffer, &lhs);
      gtk_text_buffer_get_end_iter(textBuffer, &rhs);
      gtk_text_iter_set_offset(&lhs, gtk_text_iter_get_offset(&lhs) - 1);
      gtk_text_buffer_delete(textBuffer, &lhs, &rhs);
      seekCursorToEnd();
    }
    return true;
  }

  if(scancode >= 0x20 && scancode <= 0x7e) {
    print({(char)scancode});
    command.append((char)scancode);
    return true;
  }

  return false;
}

void pConsole::seekCursorToEnd() {
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(textBuffer, &iter);
  gtk_text_buffer_place_cursor(textBuffer, &iter);
}

}
