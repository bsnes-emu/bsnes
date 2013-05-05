namespace phoenix {

static void CheckButton_toggle(CheckButton* self) {
  self->state.checked = self->checked();
  if(self->p.locked == false && self->onToggle) self->onToggle();
}

bool pCheckButton::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkWidget));
}

Size pCheckButton::minimumSize() {
  Size size = pFont::size(widget.state.font, checkButton.state.text);
  return {size.width + 28, size.height + 4};
}

void pCheckButton::setChecked(bool checked) {
  locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), checked);
  locked = false;
}

void pCheckButton::setText(string text) {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
}

void pCheckButton::constructor() {
  gtkWidget = gtk_check_button_new_with_label("");
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(CheckButton_toggle), (gpointer)&checkButton);

  setChecked(checkButton.state.checked);
  setText(checkButton.state.text);
}

void pCheckButton::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pCheckButton::orphan() {
  destructor();
  constructor();
}

}
