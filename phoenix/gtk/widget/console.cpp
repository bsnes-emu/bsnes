namespace phoenix {

static bool Console_keyPress(GtkWidget* widget, GdkEventKey* event, Console* self) {
  return self->p.keyPress(event->keyval, event->state);
}

void pConsole::print(string text) {
  //insert text before prompt and command, so as not to interrupt the current command
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(textBuffer, &iter);
  gtk_text_iter_set_offset(&iter, gtk_text_iter_get_offset(&iter) - console.prompt().size() - command.size());
  gtk_text_buffer_insert(textBuffer, &iter, text, -1);
  seekCursorToEnd();
}

void pConsole::reset() {
  //flush history and command; draw prompt
  command.reset();
  gtk_text_buffer_set_text(textBuffer, console.prompt(), -1);
  seekCursorToEnd();
}

void pConsole::setPrompt(string prompt) {
  //erase old prompt; insert new prompt in its place
  GtkTextIter lhs, rhs, iter;
  gtk_text_buffer_get_end_iter(textBuffer, &lhs);
  gtk_text_buffer_get_end_iter(textBuffer, &rhs);
  gtk_text_iter_set_offset(&lhs, gtk_text_iter_get_offset(&lhs) - previousPrompt.size() - command.size());
  gtk_text_iter_set_offset(&rhs, gtk_text_iter_get_offset(&rhs) - command.size());
  gtk_text_buffer_delete(textBuffer, &lhs, &rhs);
  gtk_text_buffer_get_end_iter(textBuffer, &iter);
  gtk_text_iter_set_offset(&iter, gtk_text_iter_get_offset(&iter) - command.size());
  gtk_text_buffer_insert(textBuffer, &iter, prompt, -1);
  seekCursorToEnd();
  previousPrompt = prompt;
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
    //add current prompt and command to history; print new prompt; execute command
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(textBuffer, &iter);
    gtk_text_buffer_insert(textBuffer, &iter, string{"\n", console.prompt()}, -1);
    string s = command;
    command.reset();
    if(console.onActivate) console.onActivate(s);
    seekCursorToEnd();
    return true;
  }

  if(scancode == GDK_KEY_BackSpace) {
    if(command.size()) {
      //delete last character of command
      command.resize(command.size() - 1);
      GtkTextIter lhs, rhs;
      gtk_text_buffer_get_end_iter(textBuffer, &lhs);
      gtk_text_buffer_get_end_iter(textBuffer, &rhs);
      gtk_text_iter_set_offset(&lhs, gtk_text_iter_get_offset(&lhs) - 1);
      gtk_text_buffer_delete(textBuffer, &lhs, &rhs);
    }
    seekCursorToEnd();
    return true;
  }

  if(scancode >= 0x20 && scancode <= 0x7e) {
    //add character to end of command
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(textBuffer, &iter);
    gtk_text_buffer_insert(textBuffer, &iter, string{(char)scancode}, -1);
    seekCursorToEnd();
    command.append((char)scancode);
    return true;
  }

  return false;
}

void pConsole::seekCursorToEnd() {
  //place cursor at end of text buffer; scroll text view to the cursor to ensure it is visible
  GtkTextIter iter;
  gtk_text_buffer_get_end_iter(textBuffer, &iter);
  gtk_text_buffer_place_cursor(textBuffer, &iter);
  GtkTextMark* mark = gtk_text_buffer_get_mark(textBuffer, "insert");
  gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(subWidget), mark);
}

}
