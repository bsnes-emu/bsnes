#if defined(Hiro_ListView)

namespace hiro {

auto pListViewColumn::construct() -> void {
  if(auto grandparent = _grandparent()) {
    auto handle = grandparent.data();
    unsigned offset = self().offset();

    gtkHeader = gtk_hbox_new(false, 0);

    gtkHeaderIcon = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(gtkHeader), gtkHeaderIcon, false, false, 0);

    gtkHeaderText = gtk_label_new(state().text);
    gtk_box_pack_start(GTK_BOX(gtkHeader), gtkHeaderText, true, false, 2);

    gtkColumn = gtk_tree_view_column_new();
    gtk_tree_view_column_set_sizing(gtkColumn, GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_title(gtkColumn, "");
    gtk_tree_view_column_set_widget(gtkColumn, gtkHeader);

    gtkCellToggle = gtk_cell_renderer_toggle_new();
    gtk_tree_view_column_pack_start(gtkColumn, gtkCellToggle, false);
    gtk_tree_view_column_set_attributes(gtkColumn, gtkCellToggle, "active", 3 * offset + 0, nullptr);
    gtk_tree_view_column_set_cell_data_func(gtkColumn, GTK_CELL_RENDERER(gtkCellToggle), (GtkTreeCellDataFunc)ListView_dataFunc, (gpointer)handle, nullptr);

    gtkCellIcon = gtk_cell_renderer_pixbuf_new();
    gtk_tree_view_column_pack_start(gtkColumn, gtkCellIcon, false);
    gtk_tree_view_column_set_attributes(gtkColumn, gtkCellIcon, "pixbuf", 3 * offset + 1, nullptr);
    gtk_tree_view_column_set_cell_data_func(gtkColumn, GTK_CELL_RENDERER(gtkCellIcon), (GtkTreeCellDataFunc)ListView_dataFunc, (gpointer)handle, nullptr);

    gtkCellText = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(gtkColumn, gtkCellText, true);  //text must expand to cell width for horizontal alignment to work
    gtk_tree_view_column_set_attributes(gtkColumn, gtkCellText, "text", 3 * offset + 2, nullptr);
    gtk_tree_view_column_set_cell_data_func(gtkColumn, GTK_CELL_RENDERER(gtkCellText), (GtkTreeCellDataFunc)ListView_dataFunc, (gpointer)handle, nullptr);

    g_signal_connect(G_OBJECT(gtkColumn), "clicked", G_CALLBACK(ListView_headerActivate), (gpointer)handle);
    g_signal_connect(G_OBJECT(gtkCellText), "edited", G_CALLBACK(ListView_edit), (gpointer)handle);
    g_signal_connect(G_OBJECT(gtkCellToggle), "toggled", G_CALLBACK(ListView_toggle), (gpointer)handle);

    gtk_tree_view_append_column(grandparent->gtkTreeView, gtkColumn);
    gtk_widget_show_all(gtkHeader);
    grandparent->_createModel();

    _setState();
  }
}

auto pListViewColumn::destruct() -> void {
  if(auto grandparent = _grandparent()) {
    gtk_tree_view_remove_column(grandparent->gtkTreeView, gtkColumn);
    gtkColumn = nullptr;
    grandparent->_createModel();
  }
}

auto pListViewColumn::setActive() -> void {
  _setState();
}

auto pListViewColumn::setAlignment(Alignment alignment) -> void {
}

auto pListViewColumn::setBackgroundColor(Color color) -> void {
}

auto pListViewColumn::setEditable(bool editable) -> void {
  g_object_set(G_OBJECT(gtkCellText), "editable", editable ? true : false, nullptr);
}

auto pListViewColumn::setExpandable(bool expandable) -> void {
  if(auto grandparent = _grandparent()) {
    grandparent->resizeColumns();
  }
}

auto pListViewColumn::setFont(const Font& font) -> void {
}

auto pListViewColumn::setForegroundColor(Color color) -> void {
}

auto pListViewColumn::setImage(const Image& image) -> void {
  if(image) {
    gtk_image_set_from_pixbuf(GTK_IMAGE(gtkHeaderIcon), CreatePixbuf(image));
  } else {
    gtk_image_clear(GTK_IMAGE(gtkHeaderIcon));
  }
}

auto pListViewColumn::setResizable(bool resizable) -> void {
  _setState();
}

auto pListViewColumn::setSortable(bool sortable) -> void {
  _setState();
}

auto pListViewColumn::setText(const string& text) -> void {
  _setState();
}

auto pListViewColumn::setVisible(bool visible) -> void {
  _setState();
}

auto pListViewColumn::setWidth(signed width) -> void {
  if(auto grandparent = _grandparent()) {
    grandparent->resizeColumns();
  }
}

auto pListViewColumn::_grandparent() -> maybe<pListView&> {
  if(auto parent = _parent()) return parent->_parent();
  return nothing;
}

auto pListViewColumn::_parent() -> maybe<pListViewHeader&> {
  if(auto parent = self().parentListViewHeader()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pListViewColumn::_setState() -> void {
  if(auto grandparent = _grandparent()) {
    gtk_tree_view_set_search_column(grandparent->gtkTreeView, 3 * self().offset() + 2);
    gtk_tree_view_column_set_resizable(gtkColumn, state().resizable);
    gtk_tree_view_column_set_clickable(gtkColumn, state().sortable);
    gtk_label_set_text(GTK_LABEL(gtkHeaderText), state().text);
    gtk_tree_view_column_set_visible(gtkColumn, self().visible());
  }
}

}

#endif
