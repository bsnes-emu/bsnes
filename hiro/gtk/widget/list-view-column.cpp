#if defined(Hiro_ListView)

namespace hiro {

auto pListViewColumn::construct() -> void {
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

  if(offset == 0) {
    gtkCellToggle = gtk_cell_renderer_toggle_new();
    gtk_tree_view_column_pack_start(gtkColumn, gtkCellToggle, false);
    gtk_tree_view_column_set_attributes(gtkColumn, gtkCellToggle, "active", 0, nullptr);
    gtk_tree_view_column_set_cell_data_func(gtkColumn, GTK_CELL_RENDERER(gtkCellToggle), (GtkTreeCellDataFunc)ListView_cellRendererToggleDataFunc, (gpointer)_parent(), nullptr);
  }

  gtkCellIcon = gtk_cell_renderer_pixbuf_new();
  gtk_tree_view_column_pack_start(gtkColumn, gtkCellIcon, false);
  gtk_tree_view_column_set_attributes(gtkColumn, gtkCellIcon, "pixbuf", 1 + offset * 2 + 0, nullptr);

  gtkCellText = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(gtkColumn, gtkCellText, false);
  gtk_tree_view_column_set_attributes(gtkColumn, gtkCellText, "text", 1 + offset * 2 + 1, nullptr);

  g_signal_connect(G_OBJECT(gtkColumn), "clicked", G_CALLBACK(ListView_headerActivate), (gpointer)_parent());
  g_signal_connect(G_OBJECT(gtkCellText), "edited", G_CALLBACK(ListView_edit), (gpointer)_parent());
  if(gtkCellToggle) g_signal_connect(G_OBJECT(gtkCellToggle), "toggled", G_CALLBACK(ListView_toggle), (gpointer)_parent());
}

auto pListViewColumn::destruct() -> void {
}

auto pListViewColumn::setActive() -> void {
  if(auto parent = _parent()) {
    gtk_tree_view_set_search_column(parent->gtkTreeView, 1 + self().offset() * 2 + 1);
  }
}

auto pListViewColumn::setBackgroundColor(Color color) -> void {
  if(color) {
    GdkColor gdkColor = CreateColor(color);
    if(gtkCellToggle) g_object_set(G_OBJECT(gtkCellToggle), "cell-background-gdk", &gdkColor, nullptr);
    g_object_set(G_OBJECT(gtkCellIcon), "cell-background-gdk", &gdkColor, nullptr);
    g_object_set(G_OBJECT(gtkCellText), "cell-background-gdk", &gdkColor, nullptr);
  } else {
    if(gtkCellToggle) g_object_set(G_OBJECT(gtkCellToggle), "cell-background-set", FALSE, nullptr);
    g_object_set(G_OBJECT(gtkCellIcon), "cell-background-set", FALSE, nullptr);
    g_object_set(G_OBJECT(gtkCellText), "cell-background-set", FALSE, nullptr);
  }
}

auto pListViewColumn::setEditable(bool editable) -> void {
  g_object_set(G_OBJECT(gtkCellText), "editable", editable ? TRUE : FALSE, nullptr);
}

auto pListViewColumn::setExpandable(bool expandable) -> void {
  if(auto parent = _parent()) {
    parent->resizeColumns();
  }
}

auto pListViewColumn::setFont(const string& font) -> void {
  pFont::setFont(gtkHeaderText, font);
  auto fontDescription = pFont::create(font);
  g_object_set(G_OBJECT(gtkCellText), "font-desc", fontDescription, nullptr);
  pango_font_description_free(fontDescription);
}

auto pListViewColumn::setForegroundColor(Color color) -> void {
  if(color) {
    GdkColor gdkColor = CreateColor(color);
    g_object_set(G_OBJECT(gtkCellText), "foreground-gdk", &gdkColor, nullptr);
  } else {
    g_object_set(G_OBJECT(gtkCellText), "foreground-set", FALSE, nullptr);
  }
}

auto pListViewColumn::setHorizontalAlignment(double alignment) -> void {
  _setAlignment();
}

auto pListViewColumn::setIcon(const image& icon) -> void {
  if(icon) {
    gtk_image_set_from_pixbuf(GTK_IMAGE(gtkHeaderIcon), CreatePixbuf(icon));
  } else {
    gtk_image_clear(GTK_IMAGE(gtkHeaderIcon));
  }
}

auto pListViewColumn::setResizable(bool resizable) -> void {
  gtk_tree_view_column_set_resizable(gtkColumn, resizable);
}

auto pListViewColumn::setText(const string& text) -> void {
  gtk_label_set_text(GTK_LABEL(gtkHeaderText), text);
}

auto pListViewColumn::setVerticalAlignment(double alignment) -> void {
  _setAlignment();
}

auto pListViewColumn::setVisible(bool visible) -> void {
  gtk_tree_view_column_set_visible(gtkColumn, visible);
}

auto pListViewColumn::setWidth(signed width) -> void {
  if(auto parent = _parent()) {
    parent->resizeColumns();
  }
}

auto pListViewColumn::_parent() -> pListView* {
  if(auto parent = self().parentListView()) return parent->self();
  return nullptr;
}

auto pListViewColumn::_setAlignment() -> void {
  gtk_tree_view_column_set_alignment(gtkColumn, state().horizontalAlignment);
  gtk_cell_renderer_set_alignment(GTK_CELL_RENDERER(gtkCellText), state().horizontalAlignment, state().verticalAlignment);
  //set multi-line text alignment
  auto pangoAlignment = PANGO_ALIGN_CENTER;
  if(state().horizontalAlignment < 0.333) pangoAlignment = PANGO_ALIGN_LEFT;
  if(state().horizontalAlignment > 0.666) pangoAlignment = PANGO_ALIGN_RIGHT;
  g_object_set(G_OBJECT(gtkCellText), "alignment", pangoAlignment, nullptr);
}

}

#endif
