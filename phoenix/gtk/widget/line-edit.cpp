namespace phoenix {

static void LineEdit_activate(LineEdit* self) {
  if(self->onActivate) self->onActivate();
}

static void LineEdit_change(LineEdit* self) {
  self->state.text = self->text();
  if(self->p.locked == false && self->onChange) self->onChange();
}

Size pLineEdit::minimumSize() {
  Size size = pFont::size(widget.state.font, lineEdit.state.text);
  return {size.width + 10, size.height + 10};
}

void pLineEdit::setBackgroundColor(Color color) {
  GdkColor gdkColor = CreateColor(color.red, color.green, color.blue);
  gtk_widget_modify_base(gtkWidget, GTK_STATE_NORMAL, &gdkColor);
}

void pLineEdit::setEditable(bool editable) {
  gtk_editable_set_editable(GTK_EDITABLE(gtkWidget), editable);
}

void pLineEdit::setForegroundColor(Color color) {
  GdkColor gdkColor = CreateColor(color.red, color.green, color.blue);
  gtk_widget_modify_text(gtkWidget, GTK_STATE_NORMAL, &gdkColor);
}

void pLineEdit::setText(string text) {
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

  setEditable(lineEdit.state.editable);
  setText(lineEdit.state.text);
}

void pLineEdit::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pLineEdit::orphan() {
  destructor();
  constructor();
}

}
