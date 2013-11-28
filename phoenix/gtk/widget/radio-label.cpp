namespace phoenix {

static void RadioLabel_activate(GtkToggleButton* toggleButton, RadioLabel* self) {
  self->p.onActivate();
}

Size pRadioLabel::minimumSize() {
  Size size = pFont::size(widget.state.font, radioLabel.state.text);
  return {size.width + 28, size.height + 4};
}

void pRadioLabel::setChecked() {
  parent().locked = true;
  for(auto& item : radioLabel.state.group) item.state.checked = false;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), radioLabel.state.checked = true);
  parent().locked = false;
}

void pRadioLabel::setGroup(const group<RadioLabel>& group) {
  if(&parent() == this) return;
  parent().locked = true;
  gtk_radio_button_set_group(
    GTK_RADIO_BUTTON(gtkWidget),
    gtk_radio_button_get_group(GTK_RADIO_BUTTON(parent().gtkWidget))
  );
  for(auto& item : radioLabel.state.group) {
    if(item.state.checked) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item.p.gtkWidget), true);
      break;
    }
  }
  parent().locked = false;
}

void pRadioLabel::setText(string text) {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
}

void pRadioLabel::constructor() {
  gtkWidget = gtk_radio_button_new_with_label(nullptr, "");
  g_signal_connect(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(RadioLabel_activate), (gpointer)&radioLabel);

  setGroup(radioLabel.state.group);
  setText(radioLabel.state.text);
}

void pRadioLabel::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pRadioLabel::orphan() {
  destructor();
  constructor();
}

void pRadioLabel::onActivate() {
  if(parent().locked) return;
  bool wasChecked = radioLabel.state.checked;
  setChecked();
  if(wasChecked) return;
  if(radioLabel.onActivate) radioLabel.onActivate();
}

pRadioLabel& pRadioLabel::parent() {
  if(radioLabel.state.group.size()) return radioLabel.state.group.first().p;
  return *this;
}

}
