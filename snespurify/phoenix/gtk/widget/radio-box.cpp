static void RadioBox_tick(RadioBox *self) {
  if(self->p.locked == false && self->checked() && self->onTick) self->onTick();
}

bool pRadioBox::checked() {
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkWidget));
}

void pRadioBox::setChecked() {
  locked = true;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget), true);
  locked = false;
}

void pRadioBox::setGroup(const reference_array<RadioBox&> &group) {
  foreach(item, group, n) {
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
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "toggled", G_CALLBACK(RadioBox_tick), (gpointer)&radioBox);
}
