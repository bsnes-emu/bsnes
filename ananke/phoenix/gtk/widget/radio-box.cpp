static void RadioBox_activate(RadioBox *self) {
  if(self->p.locked == false && self->checked() && self->onActivate) self->onActivate();
}

bool pRadioBox::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkWidget));
}

Geometry pRadioBox::minimumGeometry() {
  Geometry geometry = pFont::geometry(widget.state.font, radioBox.state.text);
//Font &font = pWidget::font();
//Geometry geometry = font.geometry(radioBox.state.text);
  return { 0, 0, geometry.width + 28, geometry.height + 4 };
}

void pRadioBox::setChecked() {
  locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), true);
  locked = false;
}

void pRadioBox::setGroup(const set<RadioBox&> &group) {
  for(unsigned n = 0; n < group.size(); n++) {
    if(n == 0) continue;
    GSList *currentGroup = gtk_radio_button_get_group(GTK_RADIO_BUTTON(group[0].p.gtkWidget));
    if(currentGroup != gtk_radio_button_get_group(GTK_RADIO_BUTTON(gtkWidget))) {
      gtk_radio_button_set_group(GTK_RADIO_BUTTON(gtkWidget), currentGroup);
    }
  }
}

void pRadioBox::setText(const string &text) {
  gtk_button_set_label(GTK_BUTTON(gtkWidget), text);
}

void pRadioBox::constructor() {
  gtkWidget = gtk_radio_button_new_with_label(0, "");
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(RadioBox_activate), (gpointer)&radioBox);

  setText(radioBox.state.text);
}

void pRadioBox::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pRadioBox::orphan() {
  destructor();
  constructor();
}
