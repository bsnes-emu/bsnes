static void ComboBox_change(ComboBox *self) {
  if(self->p.locked == false) {
    self->state.selection = self->selection();
    if(self->onChange) self->onChange();
  }
}

void pComboBox::append(const string &text) {
  gtk_combo_box_append_text(GTK_COMBO_BOX(gtkWidget), text);
  if(itemCounter++ == 0) setSelection(0);
}

Geometry pComboBox::minimumGeometry() {
  unsigned maximumWidth = 0;
  for(auto &item : comboBox.state.text) maximumWidth = max(maximumWidth, pFont::geometry(widget.state.font, item).width);

  Geometry geometry = pFont::geometry(widget.state.font, " ");
  return { 0, 0, maximumWidth + 44, geometry.height + 12 };
}

void pComboBox::reset() {
  locked = true;
  gtk_list_store_clear(GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(gtkWidget))));
  itemCounter = 0;
  locked = false;
}

unsigned pComboBox::selection() {
  return gtk_combo_box_get_active(GTK_COMBO_BOX(gtkWidget));
}

void pComboBox::setSelection(unsigned row) {
  locked = true;
  gtk_combo_box_set_active(GTK_COMBO_BOX(gtkWidget), row);
  locked = false;
}

void pComboBox::constructor() {
  itemCounter = 0;
  gtkWidget = gtk_combo_box_new_text();
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "changed", G_CALLBACK(ComboBox_change), (gpointer)&comboBox);

  locked = true;
  for(auto &text : comboBox.state.text) append(text);
  locked = false;
  setSelection(comboBox.state.selection);
}

void pComboBox::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pComboBox::orphan() {
  destructor();
  constructor();
}
