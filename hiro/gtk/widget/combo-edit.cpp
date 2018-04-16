#if defined(Hiro_ComboEdit)

namespace hiro {

static auto ComboEdit_activate(GtkComboBox* comboBox, pComboEdit* p) -> void { p->self().doActivate(); }
static auto ComboEdit_change(GtkComboBox* comboBox, pComboEdit* p) -> void { p->_updateText(); }

auto pComboEdit::construct() -> void {
  gtkListStore = gtk_list_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
  gtkTreeModel = GTK_TREE_MODEL(gtkListStore);
  gtkWidget = gtk_combo_box_new_with_model_and_entry(gtkTreeModel);
  gtkComboBox = GTK_COMBO_BOX(gtkWidget);

  gtk_cell_layout_clear(GTK_CELL_LAYOUT(gtkWidget));
  gtkCellIcon = gtk_cell_renderer_pixbuf_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(gtkWidget), gtkCellIcon, false);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(gtkWidget), gtkCellIcon, "pixbuf", 0, nullptr);
  gtkCellText = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(gtkWidget), gtkCellText, true);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(gtkWidget), gtkCellText, "text", 1, nullptr);

  //we must call gtk_combo_box_set_entry_text_column(1) in order for GtkComboBoxEntry to pull text
  //however, this API call throws a Gtk-CRITICAL assertion that column 1 is not a cell renderer
  //this is, however, incorrect. it *is* a renderer, and the API call works as expected
  //so in order to suppress the error message, we redirect stderr to /dev/null temporarily
  int stdErr = dup(STDERR_FILENO);
  int devNull = open("/dev/null", 0);
  dup2(devNull, STDERR_FILENO);
  gtk_combo_box_set_entry_text_column(gtkComboBox, 1);
  dup2(stdErr, STDERR_FILENO);
  close(devNull);
  close(stdErr);

  setBackgroundColor(state().backgroundColor);
  setEditable(state().editable);
  setForegroundColor(state().foregroundColor);
  for(auto& item : state().items) append(item);

  g_signal_connect(G_OBJECT(gtk_bin_get_child(GTK_BIN(gtkComboBox))), "activate", G_CALLBACK(ComboEdit_activate), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "changed", G_CALLBACK(ComboEdit_change), (gpointer)this);

  pWidget::construct();
}

auto pComboEdit::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pComboEdit::append(sComboEditItem item) -> void {
  lock();
  if(auto self = item->self()) {
    gtk_list_store_append(gtkListStore, &self->gtkIter);
    self->setIcon(item->state.icon);
    self->setText(item->state.text);
  }
  unlock();
}

auto pComboEdit::minimumSize() const -> Size {
  auto font = self().font(true);
  int maximumWidth = 0;
  for(auto& item : state().items) {
    maximumWidth = max(maximumWidth,
      (item->state.icon ? pFont::size(font, " ").height() + 2 : 0)
    + pFont::size(font, item->state.text).width()
    );
  }
  return {maximumWidth + 40, pFont::size(font, " ").height() + 12};
}

auto pComboEdit::remove(sComboEditItem item) -> void {
  lock();
  if(auto delegate = item->self()) {
    gtk_list_store_remove(gtkListStore, &delegate->gtkIter);
  }
  unlock();
}

auto pComboEdit::reset() -> void {
  lock();
  gtk_list_store_clear(gtkListStore);
  unlock();
}

auto pComboEdit::setBackgroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_base(gtk_bin_get_child(GTK_BIN(gtkComboBox)), GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pComboEdit::setEditable(bool editable) -> void {
  gtk_editable_set_editable(GTK_EDITABLE(gtk_bin_get_child(GTK_BIN(gtkComboBox))), editable);
}

auto pComboEdit::setForegroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_text(gtk_bin_get_child(GTK_BIN(gtkComboBox)), GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pComboEdit::setFont(const Font& font) -> void {
  pWidget::setFont(font);
  auto fontDescription = pFont::create(font);
  g_object_set(G_OBJECT(gtkCellText), "font-desc", fontDescription, nullptr);
}

auto pComboEdit::setText(const string& text) -> void {
  lock();
  gtk_entry_set_text(GTK_ENTRY(gtk_bin_get_child(GTK_BIN(gtkComboBox))), text);
  unlock();
}

auto pComboEdit::_updateText() -> void {
  string text = gtk_entry_get_text(GTK_ENTRY(gtk_bin_get_child(GTK_BIN(gtkComboBox))));
  if(text == state().text) return;
  state().text = text;
  if(!locked()) self().doChange();
}

}

#endif
