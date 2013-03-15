namespace phoenix {

static void ComboButton_change(ComboButton *self) {
  if(self->p.locked == false) {
    self->state.selection = self->selection();
    if(self->onChange) self->onChange();
  }
}

void pComboButton::append(const string &text) {
  gtk_combo_box_append_text(GTK_COMBO_BOX(gtkWidget), text);
  if(itemCounter++ == 0) setSelection(0);
}

Size pComboButton::minimumSize() {
  unsigned maximumWidth = 0;
  for(auto &item : comboButton.state.text) maximumWidth = max(maximumWidth, pFont::size(widget.state.font, item).width);

  Size size = pFont::size(widget.state.font, " ");
  return {maximumWidth + 44, size.height + 12};
}

void pComboButton::modify(unsigned row, const string &text) {
  locked = true;
  unsigned position = selection();
  gtk_combo_box_remove_text(GTK_COMBO_BOX(gtkWidget), row);
  gtk_combo_box_insert_text(GTK_COMBO_BOX(gtkWidget), row, text);
  gtk_combo_box_set_active(GTK_COMBO_BOX(gtkWidget), position);
  locked = false;
}

void pComboButton::remove(unsigned row) {
  locked = true;
  unsigned position = selection();
  gtk_combo_box_remove_text(GTK_COMBO_BOX(gtkWidget), row);
  if(position == row) gtk_combo_box_set_active(GTK_COMBO_BOX(gtkWidget), 0);
  locked = false;
}

void pComboButton::reset() {
  locked = true;
  gtk_list_store_clear(GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(gtkWidget))));
  itemCounter = 0;
  locked = false;
}

unsigned pComboButton::selection() {
  return gtk_combo_box_get_active(GTK_COMBO_BOX(gtkWidget));
}

void pComboButton::setSelection(unsigned row) {
  locked = true;
  gtk_combo_box_set_active(GTK_COMBO_BOX(gtkWidget), row);
  locked = false;
}

void pComboButton::constructor() {
  itemCounter = 0;
  gtkWidget = gtk_combo_box_new_text();
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "changed", G_CALLBACK(ComboButton_change), (gpointer)&comboButton);

  locked = true;
  for(auto &text : comboButton.state.text) append(text);
  locked = false;
  setSelection(comboButton.state.selection);
}

void pComboButton::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pComboButton::orphan() {
  destructor();
  constructor();
}

}
