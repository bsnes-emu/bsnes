namespace phoenix {

static void RadioButton_activate(RadioButton *self) {
  self->p.onActivate();
}

bool pRadioButton::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkWidget));
}

Size pRadioButton::minimumSize() {
  Size size = pFont::size(widget.state.font, radioButton.state.text);
  return {size.width + 28, size.height + 4};
}

void pRadioButton::setChecked() {
  parent().locked = true;
  for(auto &item : radioButton.state.group) item.state.checked = false;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), radioButton.state.checked = true);
  parent().locked = false;
}

void pRadioButton::setGroup(const set<RadioButton&> &group) {
  parent().locked = true;
  if(radioButton.state.group.size() == 0 || &radioButton.state.group[0].p == this) return;
  gtk_radio_button_set_group(
    GTK_RADIO_BUTTON(gtkWidget),
    gtk_radio_button_get_group(GTK_RADIO_BUTTON(radioButton.state.group[0].p.gtkWidget))
  );
  for(auto &item : radioButton.state.group) {
    if(item.state.checked) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item.p.gtkWidget), true);
      break;
    }
  }
  parent().locked = false;
}

void pRadioButton::setText(const string &text) {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
}

void pRadioButton::onActivate() {
  if(parent().locked == false) {
    bool wasChecked = radioButton.state.checked;
    setChecked();
    if(wasChecked == false) {
      if(radioButton.onActivate) radioButton.onActivate();
    }
  }
}

pRadioButton& pRadioButton::parent() {
  if(radioButton.state.group.size()) return radioButton.state.group[0].p;
  return *this;
}

void pRadioButton::constructor() {
  gtkWidget = gtk_radio_button_new_with_label(nullptr, "");
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(RadioButton_activate), (gpointer)&radioButton);

  setGroup(radioButton.state.group);
  setText(radioButton.state.text);
}

void pRadioButton::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pRadioButton::orphan() {
  destructor();
  constructor();
}

}
