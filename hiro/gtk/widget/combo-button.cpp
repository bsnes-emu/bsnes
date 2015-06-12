#if defined(Hiro_ComboButton)

namespace hiro {

static auto ComboButton_change(GtkComboBox* comboBox, pComboButton* p) -> void { p->_updateSelected(); }

auto pComboButton::construct() -> void {
  gtkListStore = gtk_list_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
  gtkTreeModel = GTK_TREE_MODEL(gtkListStore);
  gtkWidget = gtk_combo_box_new_with_model(gtkTreeModel);
  gtkComboBox = GTK_COMBO_BOX(gtkWidget);

  gtkCellIcon = gtk_cell_renderer_pixbuf_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(gtkWidget), gtkCellIcon, false);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(gtkWidget), gtkCellIcon, "pixbuf", 0, nullptr);
  gtkCellText = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(gtkWidget), gtkCellText, true);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(gtkWidget), gtkCellText, "text", 1, nullptr);

  for(auto& item : state().items) append(item);

  g_signal_connect(G_OBJECT(gtkWidget), "changed", G_CALLBACK(ComboButton_change), (gpointer)this);

  pWidget::construct();
}

auto pComboButton::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pComboButton::append(sComboButtonItem item) -> void {
  lock();
  if(auto self = item->self()) {
    gtk_list_store_append(gtkListStore, &self->gtkIter);
    self->setIcon(item->state.icon);
    if(item->state.selected) self->setSelected();
    self->setText(item->state.text);
  }
  if(gtk_combo_box_get_active(gtkComboBox) < 0) item->setSelected();
  unlock();
}

auto pComboButton::minimumSize() const -> Size {
  string font = self().font(true);
  signed maximumWidth = 0;
  for(auto& item : state().items) {
    maximumWidth = max(maximumWidth,
      (item->state.icon ? pFont::size(font, " ").height() + 2 : 0)
    + pFont::size(font, item->state.text).width()
    );
  }
  return {maximumWidth + 40, pFont::size(font, " ").height() + 12};
}

auto pComboButton::remove(sComboButtonItem item) -> void {
  lock();
  if(auto delegate = item->self()) {
    gtk_list_store_remove(gtkListStore, &delegate->gtkIter);
    if(gtk_combo_box_get_active(gtkComboBox) < 0) {
      if(auto item = self().item(0)) item->setSelected();
    }
  }
  unlock();
}

auto pComboButton::reset() -> void {
  lock();
  gtk_list_store_clear(gtkListStore);
  unlock();
}

auto pComboButton::setFont(const string& font) -> void {
  pWidget::setFont(font);
  auto fontDescription = pFont::create(font);
  g_object_set(G_OBJECT(gtkCellText), "font-desc", fontDescription, nullptr);
}

auto pComboButton::_updateSelected() -> void {
  for(auto& item : state().items) item->state.selected = false;
  signed selected = gtk_combo_box_get_active(gtkComboBox);
  if(selected >= 0) {
    if(auto item = self().item(selected)) {
      item->state.selected = true;
      if(!locked()) self().doChange();
    }
  }
}

}

#endif
