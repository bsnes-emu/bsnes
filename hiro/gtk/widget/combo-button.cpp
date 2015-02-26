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

  for(auto& item : state().items) {
    append(item);
    if(item->selected()) item->setSelected();
  }

  g_signal_connect(G_OBJECT(gtkWidget), "changed", G_CALLBACK(ComboButton_change), (gpointer)this);

  pWidget::construct();
}

auto pComboButton::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pComboButton::append(sComboButtonItem item) -> void {
  lock();
  if(auto delegate = item->self()) {
    gtk_list_store_append(gtkListStore, &delegate->gtkIter);
    item->setIcon(item->state.icon);
    item->setText(item->state.text);
  }
  unlock();
}

auto pComboButton::minimumSize() const -> Size {
  string font = self().font(true);
  signed maximumWidth = 0;
  for(auto& item : state().items) {
    maximumWidth = max(maximumWidth,
      (item->state.icon ? 2 + pFont::size(font, " ").height() : 0)
    + pFont::size(font, item->state.text).width()
    );
  }
  Size size = pFont::size(font, " ");
  return {maximumWidth + 40, size.height() + 12};
}

auto pComboButton::remove(sComboButtonItem item) -> void {
  lock();
  if(auto delegate = item->self()) {
    gtk_list_store_remove(gtkListStore, &delegate->gtkIter);
    //if the currently selected item is removed; GTK+ deselects everything
    //detect this behavior and select the first item instead of nothing
    if(gtk_combo_box_get_active(GTK_COMBO_BOX(gtkWidget)) < 0) {
      if(gtk_tree_model_iter_n_children(gtkTreeModel, nullptr) > 0) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(gtkWidget), 0);
        state().selected = 0;
      }
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
  signed selected = gtk_combo_box_get_active(gtkComboBox);
  if(selected >= 0) {
    state().selected = selected;
    if(!locked()) self().doChange();
  }
}

}
