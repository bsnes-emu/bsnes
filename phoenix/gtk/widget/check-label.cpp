namespace phoenix {

static void CheckLabel_toggle(GtkToggleButton* toggleButton, CheckLabel* self) {
  self->state.checked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->p.gtkWidget));
  if(!self->p.locked && self->onToggle) self->onToggle();
}

Size pCheckLabel::minimumSize() {
  Size size = pFont::size(widget.state.font, checkLabel.state.text);
  return {size.width + 28, size.height + 4};
}

void pCheckLabel::setChecked(bool checked) {
  locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), checked);
  locked = false;
}

void pCheckLabel::setText(string text) {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
}

void pCheckLabel::constructor() {
  gtkWidget = gtk_check_button_new_with_label("");
  g_signal_connect(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(CheckLabel_toggle), (gpointer)&checkLabel);

  setChecked(checkLabel.state.checked);
  setText(checkLabel.state.text);
}

void pCheckLabel::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pCheckLabel::orphan() {
  destructor();
  constructor();
}

}
