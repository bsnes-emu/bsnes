#if defined(Hiro_LineEdit)

namespace hiro {

static auto LineEdit_activate(GtkEntry*, pLineEdit* p) -> void {
  p->self().doActivate();
}

static auto LineEdit_change(GtkEditable*, pLineEdit* p) -> void {
  p->state().text = gtk_entry_get_text(GTK_ENTRY(p->gtkWidget));
  if(!p->locked()) p->self().doChange();
}

auto pLineEdit::construct() -> void {
  gtkWidget = gtk_entry_new();

  setBackgroundColor(state().backgroundColor);
  setEditable(state().editable);
  setForegroundColor(state().foregroundColor);
  setText(state().text);

  g_signal_connect(G_OBJECT(gtkWidget), "activate", G_CALLBACK(LineEdit_activate), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "changed", G_CALLBACK(LineEdit_change), (gpointer)this);

  pWidget::construct();
}

auto pLineEdit::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pLineEdit::minimumSize() const -> Size {
  Size size = pFont::size(self().font(true), state().text);
  return {size.width() + 10, size.height() + 10};
}

auto pLineEdit::setBackgroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_base(gtkWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pLineEdit::setEditable(bool editable) -> void {
  gtk_editable_set_editable(GTK_EDITABLE(gtkWidget), editable);
}

auto pLineEdit::setForegroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_text(gtkWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pLineEdit::setText(const string& text) -> void {
  lock();
  gtk_entry_set_text(GTK_ENTRY(gtkWidget), text);
  unlock();
}

}

#endif
