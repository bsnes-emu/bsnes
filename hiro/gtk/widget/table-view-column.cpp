#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewColumn::construct() -> void {
  if(auto grandparent = _grandparent()) {
    auto handle = grandparent.data();
    unsigned offset = self().offset();

    #if HIRO_GTK==2
    gtkHeader = gtk_hbox_new(false, 0);
    #elif HIRO_GTK==3
    gtkHeader = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    #endif

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
    gtk_tree_view_column_set_cell_data_func(gtkColumn, GTK_CELL_RENDERER(gtkCellToggle), (GtkTreeCellDataFunc)TableView_dataFunc, (gpointer)handle, nullptr);

    gtkCellIcon = gtk_cell_renderer_pixbuf_new();
    gtk_tree_view_column_pack_start(gtkColumn, gtkCellIcon, false);
    gtk_tree_view_column_set_attributes(gtkColumn, gtkCellIcon, "pixbuf", 3 * offset + 1, nullptr);
    gtk_tree_view_column_set_cell_data_func(gtkColumn, GTK_CELL_RENDERER(gtkCellIcon), (GtkTreeCellDataFunc)TableView_dataFunc, (gpointer)handle, nullptr);

    gtkCellText = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(gtkColumn, gtkCellText, true);  //text must expand to cell width for horizontal alignment to work
    gtk_tree_view_column_set_attributes(gtkColumn, gtkCellText, "text", 3 * offset + 2, nullptr);
    gtk_tree_view_column_set_cell_data_func(gtkColumn, GTK_CELL_RENDERER(gtkCellText), (GtkTreeCellDataFunc)TableView_dataFunc, (gpointer)handle, nullptr);

    g_signal_connect(G_OBJECT(gtkColumn), "clicked", G_CALLBACK(TableView_headerActivate), (gpointer)handle);
    g_signal_connect(G_OBJECT(gtkCellText), "edited", G_CALLBACK(TableView_edit), (gpointer)handle);
    g_signal_connect(G_OBJECT(gtkCellToggle), "toggled", G_CALLBACK(TableView_toggle), (gpointer)handle);

    gtk_tree_view_append_column(grandparent->gtkTreeView, gtkColumn);
    gtk_widget_show_all(gtkHeader);
    grandparent->_createModel();

    _setState();
  }
}

auto pTableViewColumn::destruct() -> void {
  if(auto grandparent = _grandparent()) {
    gtk_tree_view_remove_column(grandparent->gtkTreeView, gtkColumn);
    gtkColumn = nullptr;
    grandparent->_createModel();
  }
}

auto pTableViewColumn::setActive() -> void {
  _setState();
}

auto pTableViewColumn::setAlignment(Alignment alignment) -> void {
}

auto pTableViewColumn::setBackgroundColor(Color color) -> void {
}

auto pTableViewColumn::setEditable(bool editable) -> void {
  g_object_set(G_OBJECT(gtkCellText), "editable", editable ? true : false, nullptr);
}

auto pTableViewColumn::setExpandable(bool expandable) -> void {
  if(auto grandparent = _grandparent()) {
    grandparent->resizeColumns();
  }
}

auto pTableViewColumn::setFont(const Font& font) -> void {
}

auto pTableViewColumn::setForegroundColor(Color color) -> void {
}

auto pTableViewColumn::setIcon(const image& icon) -> void {
  if(icon) {
    gtk_image_set_from_pixbuf(GTK_IMAGE(gtkHeaderIcon), CreatePixbuf(icon));
  } else {
    gtk_image_clear(GTK_IMAGE(gtkHeaderIcon));
  }
}

auto pTableViewColumn::setResizable(bool resizable) -> void {
  _setState();
}

auto pTableViewColumn::setSortable(bool sortable) -> void {
  _setState();
}

auto pTableViewColumn::setText(const string& text) -> void {
  _setState();
}

auto pTableViewColumn::setVisible(bool visible) -> void {
  _setState();
}

auto pTableViewColumn::setWidth(signed width) -> void {
  if(auto grandparent = _grandparent()) {
    grandparent->resizeColumns();
  }
}

auto pTableViewColumn::_grandparent() -> maybe<pTableView&> {
  if(auto parent = _parent()) return parent->_parent();
  return nothing;
}

auto pTableViewColumn::_parent() -> maybe<pTableViewHeader&> {
  if(auto parent = self().parentTableViewHeader()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pTableViewColumn::_setState() -> void {
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
