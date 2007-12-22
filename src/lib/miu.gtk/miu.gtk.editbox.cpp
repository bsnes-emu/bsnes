void pEditbox::create(uint style, uint width, uint height, const char *text) {
  multiline = bool(style & Editbox::Multiline);

  if(multiline == false) {
    editbox = gtk_entry_new();
    if(style & Editbox::Readonly) { gtk_entry_set_editable(GTK_ENTRY(editbox), false); }
    gtk_entry_set_text(GTK_ENTRY(editbox), text ? text : "");
    gtk_widget_set_size_request(editbox, width, height);
    gtk_widget_show(editbox);
  } else {
  GtkPolicyType hscroll = (style & Editbox::HorizontalScrollAlways) ? GTK_POLICY_ALWAYS :
                          (style & Editbox::HorizontalScrollNever)  ? GTK_POLICY_NEVER  :
                          GTK_POLICY_AUTOMATIC;
  GtkPolicyType vscroll = (style & Editbox::VerticalScrollAlways) ? GTK_POLICY_ALWAYS :
                          (style & Editbox::VerticalScrollNever)  ? GTK_POLICY_NEVER  :
                          GTK_POLICY_AUTOMATIC;
    scrollbox = gtk_scrolled_window_new(0, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbox), hscroll, vscroll);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollbox), GTK_SHADOW_ETCHED_IN);
    gtk_widget_set_size_request(scrollbox, width, height);
    editbox = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrollbox), editbox);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editbox));
    if(style & Editbox::Readonly) { gtk_text_view_set_editable(GTK_TEXT_VIEW(editbox), false); }
    gtk_text_buffer_set_text(buffer, text ? text : "", -1);
    gtk_widget_show(editbox);
    gtk_widget_show(scrollbox);
  }
}

void pEditbox::set_text(const char *text) {
  if(multiline == false) {
    gtk_entry_set_text(GTK_ENTRY(editbox), text);
  } else {
    gtk_text_buffer_set_text(buffer, text, -1);
  }
}

uint pEditbox::get_text(char *text, uint length) {
  if(multiline == false) {
  const char *temp = gtk_entry_get_text(GTK_ENTRY(editbox));
    return strlcpy(text, temp ? temp : "", length);
  } else {
  GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    return strlcpy(text, gtk_text_buffer_get_text(buffer, &start, &end, true), length);
  }
}

pEditbox::pEditbox(Editbox &self_) : pFormControl(self_), self(self_) {
  scrollbox = 0;
  editbox = 0;
  buffer = 0;
  multiline = false;
}

/* internal */

GtkWidget* pEditbox::gtk_handle() {
  return multiline ? scrollbox : editbox;
}
