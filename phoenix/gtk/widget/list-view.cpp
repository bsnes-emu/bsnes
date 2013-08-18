namespace phoenix {

static void ListView_activate(ListView* self) {
  if(self->onActivate) self->onActivate();
}

static void ListView_change(ListView* self) {
  if(self->state.selected == false || self->state.selection != self->selection()) {
    self->state.selected = true;
    self->state.selection = self->selection();
    if(self->onChange) self->onChange();
  }
}

static void ListView_toggle(GtkCellRendererToggle* cell, gchar* path, ListView* self) {
  unsigned row = decimal(path);
  self->setChecked(row, !self->checked(row));
  if(self->onToggle) self->onToggle(row);
}

void pListView::append(const lstring& text) {
  GtkTreeIter iter;
  gtk_list_store_append(store, &iter);
  for(unsigned n = 0; n < text.size(); n++) gtk_list_store_set(store, &iter, 1 + n * 2 + 1, (const char*)text[n], -1);
}

void pListView::autoSizeColumns() {
  gtk_tree_view_columns_autosize(GTK_TREE_VIEW(subWidget));
}

bool pListView::checked(unsigned row) {
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreeIter iter;
  bool state;
  if(gtk_tree_model_get_iter_from_string(model, &iter, string(row)) == false) return false;
  gtk_tree_model_get(model, &iter, 0, &state, -1);
  return state;
}

bool pListView::focused() {
  return GTK_WIDGET_HAS_FOCUS(subWidget);
}

void pListView::modify(unsigned row, const lstring& text) {
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreeIter iter;
  gtk_tree_model_get_iter_from_string(model, &iter, string(row));
  for(unsigned n = 0; n < text.size(); n++) gtk_list_store_set(store, &iter, 1 + n * 2 + 1, (const char*)text[n], -1);
}

void pListView::remove(unsigned row) {
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreeIter iter;
  gtk_tree_model_get_iter_from_string(model, &iter, string(row));
  gtk_list_store_remove(store, &iter);
}

void pListView::reset() {
  listView.state.selected = false;
  listView.state.selection = 0;
  gtk_list_store_clear(GTK_LIST_STORE(store));
  gtk_tree_view_set_model(GTK_TREE_VIEW(subWidget), GTK_TREE_MODEL(store));
  //reset gtk_scrolled_window scrollbar position to 0,0 (top-left), as ListView is now empty
  gtk_scrolled_window_set_hadjustment(GTK_SCROLLED_WINDOW(gtkWidget), 0);
  gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(gtkWidget), 0);
}

bool pListView::selected() {
  GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(subWidget));
  return gtk_tree_selection_get_selected(selection, 0, 0);
}

unsigned pListView::selection() {
  GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(subWidget));
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreeIter iter;
  if(gtk_tree_selection_get_selected(selection, 0, &iter) == false) return listView.state.selection;
  char* path = gtk_tree_model_get_string_from_iter(model, &iter);
  unsigned row = decimal(path);
  g_free(path);
  return row;
}

void pListView::setCheckable(bool checkable) {
  gtk_cell_renderer_set_visible(column(0).checkbutton, checkable);
}

void pListView::setChecked(unsigned row, bool checked) {
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreeIter iter;
  gtk_tree_model_get_iter_from_string(model, &iter, string(row));
  gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, checked, -1);
}

void pListView::setHeaderText(const lstring& text) {
  destructor();
  constructor();
}

void pListView::setHeaderVisible(bool visible) {
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(subWidget), visible);
}

void pListView::setImage(unsigned row, unsigned column, const nall::image& image) {
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreeIter iter;
  gtk_tree_model_get_iter_from_string(model, &iter, string(row));
  if(image.empty() == false) {
    GdkPixbuf* pixbuf = CreatePixbuf(image, true);
    gtk_list_store_set(store, &iter, 1 + column * 2, pixbuf, -1);
  } else {
    gtk_list_store_set(store, &iter, 1 + column * 2, nullptr, -1);
  }
}

void pListView::setSelected(bool selected) {
  if(selected == false) {
    GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(subWidget));
    gtk_tree_selection_unselect_all(selection);
  } else {
    setSelection(listView.state.selection);
  }
}

void pListView::setSelection(unsigned row) {
  GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(subWidget));
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  gtk_tree_selection_unselect_all(selection);
  GtkTreeIter iter;
  if(gtk_tree_model_get_iter_from_string(model, &iter, string(row)) == false) return;
  gtk_tree_selection_select_iter(selection, &iter);

  //scroll window to selected item
  char* path = gtk_tree_model_get_string_from_iter(model, &iter);
  GtkTreePath* treePath = gtk_tree_path_new_from_string(path);
  gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(subWidget), treePath, nullptr, true, 0.5, 0.0);
  gtk_tree_path_free(treePath);
  g_free(path);
}

void pListView::constructor() {
  gtkWidget = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkWidget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(gtkWidget), GTK_SHADOW_ETCHED_IN);

  lstring headerText = listView.state.headerText;
  if(headerText.size() == 0) headerText.append("");  //ListView must have at least one column

  column.reset();
  vector<GType> gtype;
  for(auto& text : headerText) {
    GtkColumn cell;
    cell.label = gtk_label_new(text);
    cell.column = gtk_tree_view_column_new();
    gtk_tree_view_column_set_resizable(cell.column, true);
    gtk_tree_view_column_set_title(cell.column, "");

    if(column.size() == 0) {  //first column checkbutton
      cell.checkbutton = gtk_cell_renderer_toggle_new();
      gtk_tree_view_column_pack_start(cell.column, cell.checkbutton, false);
      gtk_tree_view_column_set_attributes(cell.column, cell.checkbutton, "active", gtype.size(), nullptr);
      gtype.append(G_TYPE_BOOLEAN);
      g_signal_connect(cell.checkbutton, "toggled", G_CALLBACK(ListView_toggle), (gpointer)&listView);
    }

    cell.icon = gtk_cell_renderer_pixbuf_new();
    gtk_tree_view_column_pack_start(cell.column, cell.icon, false);
    gtk_tree_view_column_set_attributes(cell.column, cell.icon, "pixbuf", gtype.size(), nullptr);
    gtype.append(GDK_TYPE_PIXBUF);

    cell.text = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(cell.column, cell.text, false);
    gtk_tree_view_column_set_attributes(cell.column, cell.text, "text", gtype.size(), nullptr);
    gtype.append(G_TYPE_STRING);

    column.append(cell);
  }

  store = gtk_list_store_newv(gtype.size(), gtype.data());
  subWidget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_container_add(GTK_CONTAINER(gtkWidget), subWidget);
  g_object_unref(G_OBJECT(store));

  for(auto& cell : column) {
    gtk_tree_view_column_set_widget(GTK_TREE_VIEW_COLUMN(cell.column), cell.label);
    gtk_tree_view_append_column(GTK_TREE_VIEW(subWidget), cell.column);
    gtk_widget_show(cell.label);
  }

  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(subWidget), headerText.size() >= 2);  //two or more columns + checkbutton column
  gtk_tree_view_set_search_column(GTK_TREE_VIEW(subWidget), 2);

  g_signal_connect_swapped(G_OBJECT(subWidget), "cursor-changed", G_CALLBACK(ListView_change), (gpointer)&listView);
  g_signal_connect_swapped(G_OBJECT(subWidget), "row-activated", G_CALLBACK(ListView_activate), (gpointer)&listView);

  gtk_widget_show(subWidget);

  setHeaderVisible(listView.state.headerVisible);
  setCheckable(listView.state.checkable);
  for(auto& text : listView.state.text) append(text);
  for(unsigned n = 0; n < listView.state.checked.size(); n++) setChecked(n, listView.state.checked[n]);
  if(listView.state.selected) setSelection(listView.state.selection);
  autoSizeColumns();
}

void pListView::destructor() {
  gtk_widget_destroy(subWidget);
  gtk_widget_destroy(gtkWidget);
}

void pListView::orphan() {
  destructor();
  constructor();
}

void pListView::setFocused() {
  gtk_widget_grab_focus(subWidget);
}

void pListView::setFont(string font) {
  pFont::setFont(gtkWidget, font);
  for(auto& cell : column) pFont::setFont(cell.label, font);
}

}
