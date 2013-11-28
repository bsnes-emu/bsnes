namespace phoenix {

static void ComboButton_change(ComboButton* self) {
  if(!self->p.locked) {
    self->state.selection = gtk_combo_box_get_active(GTK_COMBO_BOX(self->p.gtkWidget));
    if(self->onChange) self->onChange();
  }
}

void pComboButton::append(string text) {
  gtk_combo_box_append_text(GTK_COMBO_BOX(gtkWidget), text);
  if(itemCounter++ == 0) comboButton.setSelection(0);
}

Size pComboButton::minimumSize() {
  unsigned maximumWidth = 0;
  for(auto& item : comboButton.state.text) maximumWidth = max(maximumWidth, pFont::size(widget.state.font, item).width);

  Size size = pFont::size(widget.state.font, " ");
  return {maximumWidth + 44, size.height + 12};
}

void pComboButton::remove(unsigned selection) {
  locked = true;
  gtk_combo_box_remove_text(GTK_COMBO_BOX(gtkWidget), selection);
  itemCounter--;
  locked = false;

  //when removing the actively selected item, reset the selection to the first entry
  if(selection == comboButton.state.selection) comboButton.setSelection(0);
}

void pComboButton::reset() {
  locked = true;
  gtk_list_store_clear(GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(gtkWidget))));
  itemCounter = 0;
  locked = false;
}

void pComboButton::setSelection(unsigned selection) {
  locked = true;
  gtk_combo_box_set_active(GTK_COMBO_BOX(gtkWidget), selection);
  locked = false;
}

void pComboButton::setText(unsigned selection, string text) {
  locked = true;
  gtk_combo_box_remove_text(GTK_COMBO_BOX(gtkWidget), selection);
  gtk_combo_box_insert_text(GTK_COMBO_BOX(gtkWidget), selection, text);
  gtk_combo_box_set_active(GTK_COMBO_BOX(gtkWidget), comboButton.state.selection);
  locked = false;
}

void pComboButton::constructor() {
  itemCounter = 0;
  gtkWidget = gtk_combo_box_new_text();
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "changed", G_CALLBACK(ComboButton_change), (gpointer)&comboButton);

  locked = true;
  for(auto& text : comboButton.state.text) append(text);
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
