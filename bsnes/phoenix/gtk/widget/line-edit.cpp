static void LineEdit_activate(LineEdit *self) {
  if(self->onActivate) self->onActivate();
}

static void LineEdit_change(LineEdit *self) {
  if(self->p.locked == false && self->onChange) self->onChange();
}

void pLineEdit::setEditable(bool editable) {
  gtk_entry_set_editable(GTK_ENTRY(gtkWidget), editable);
}

void pLineEdit::setText(const string &text) {
  locked = true;
  gtk_entry_set_text(GTK_ENTRY(gtkWidget), text);
  locked = false;
}

string pLineEdit::text() {
  return gtk_entry_get_text(GTK_ENTRY(gtkWidget));
}

void pLineEdit::constructor() {
  gtkWidget = gtk_entry_new();
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "activate", G_CALLBACK(LineEdit_activate), (gpointer)&lineEdit);
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "changed", G_CALLBACK(LineEdit_change), (gpointer)&lineEdit);
}
