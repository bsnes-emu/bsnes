static void ListView_activate(ListView *self) {
  signed selection = -1;
  if(self->onActivate) self->onActivate();
}

static void ListView_change(ListView *self) {
  signed selection = -1;
  self->state.selection = self->selection();
  if(self->onChange) self->onChange();
}

static void ListView_tick(GtkCellRendererToggle *cell, gchar *path_string, ListView *self) {
  unsigned row = decimal(path_string);
  self->setChecked(row, !self->checked(row));
  if(self->onTick) self->onTick(row);
}

void pListView::append(const lstring &text) {
  GtkTreeIter iter;
  gtk_list_store_append(store, &iter);
  foreach(item, text, n) gtk_list_store_set(store, &iter, 1 + n, (const char*)item, -1);
}

void pListView::autosizeColumns() {
  gtk_tree_view_columns_autosize(GTK_TREE_VIEW(subWidget));
}

bool pListView::checked(unsigned row) {
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreePath *path = gtk_tree_path_new_from_string(string(row));
  GtkTreeIter iter;
  bool state;
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter, 0, &state, -1);
  gtk_tree_path_free(path);
  return state;
}

void pListView::modify(unsigned row, const lstring &text) {
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreeIter iter;
  for(unsigned i = 0; i <= row; i++) {
    if(i == 0) gtk_tree_model_get_iter_first(model, &iter);
    else gtk_tree_model_iter_next(model, &iter);
  }
  foreach(item, text, n) gtk_list_store_set(store, &iter, 1 + n, (const char*)item, -1);
}

void pListView::modify(unsigned row, unsigned column, const string &text) {
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreeIter iter;
  for(unsigned i = 0; i <= row; i++) {
    if(i == 0) gtk_tree_model_get_iter_first(model, &iter);
    else gtk_tree_model_iter_next(model, &iter);
  }
  gtk_list_store_set(store, &iter, 1 + column, (const char*)text, -1);
}

void pListView::reset() {
  listView.state.selection = { false, 0 };
  gtk_list_store_clear(GTK_LIST_STORE(store));
  gtk_tree_view_set_model(GTK_TREE_VIEW(subWidget), GTK_TREE_MODEL(store));
  //reset gtk_scrolled_window scrollbar position to 0,0 (top-left), as ListView is now empty
  gtk_scrolled_window_set_hadjustment(GTK_SCROLLED_WINDOW(gtkWidget), 0);
  gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(gtkWidget), 0);
}

optional<unsigned> pListView::selection() {
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(subWidget));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreeIter iter;
  if(gtk_tree_model_get_iter_first(model, &iter) == false) return { false, 0 };
  if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) return { true, 0 };
  for(unsigned n = 1;; n++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) return { false, 0 };
    if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) return { true, n };
  }
  return { false, 0 };
}

void pListView::setCheckable(bool checkable) {
  gtk_tree_view_column_set_visible(column[0].column, checkable);
}

void pListView::setChecked(unsigned row, bool checked) {
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  GtkTreePath *path = gtk_tree_path_new_from_string(string(row));
  GtkTreeIter iter;
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, checked, -1);
  gtk_tree_path_free(path);
}

void pListView::setHeaderText(const lstring &text) {
  create();
}

void pListView::setHeaderVisible(bool visible) {
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(subWidget), visible);
}

void pListView::setSelection(unsigned row) {
  signed current = -1;
  if(auto position = selection()) current = position();
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(subWidget));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
  gtk_tree_selection_unselect_all(selection);

  GtkTreeIter iter;
  if(gtk_tree_model_get_iter_first(model, &iter) == false) return;
  if(row == 0) {
    gtk_tree_selection_select_iter(selection, &iter);
    return;
  }
  for(unsigned n = 1;; n++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) return;
    if(row == n) {
      gtk_tree_selection_select_iter(selection, &iter);
      return;
    }
  }
}

void pListView::constructor() {
  gtkWidget = 0;
  subWidget = 0;
  create();
}

void pListView::create() {
  if(subWidget) gtk_widget_destroy(subWidget);
  if(gtkWidget) gtk_widget_destroy(gtkWidget);

  gtkWidget = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkWidget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(gtkWidget), GTK_SHADOW_ETCHED_IN);

  lstring headerText;
  headerText.append("");  //checkbox column
  foreach(headerItem, listView.state.headerText) headerText.append(headerItem);
  if(headerText.size() == 1) headerText.append("");

  GType *v = (GType*)malloc(headerText.size() * sizeof(GType));
  foreach(header, headerText, n) v[n] = (n == 0 ? G_TYPE_BOOLEAN : G_TYPE_STRING);
  store = gtk_list_store_newv(headerText.size(), v);
  free(v);

  subWidget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_container_add(GTK_CONTAINER(gtkWidget), subWidget);
  g_object_unref(G_OBJECT(store));

  foreach(header, headerText, n) {
    if(n == 0) {
      column[n].renderer = gtk_cell_renderer_toggle_new();
      column[n].column = gtk_tree_view_column_new_with_attributes("", column[n].renderer, "active", n, (void*)0);
      gtk_tree_view_column_set_resizable(column[n].column, false);
      gtk_tree_view_column_set_visible(column[n].column, false);
      g_signal_connect(column[n].renderer, "toggled", G_CALLBACK(ListView_tick), (gpointer)&listView);
    } else {
      column[n].renderer = gtk_cell_renderer_text_new();
      column[n].column = gtk_tree_view_column_new_with_attributes("", column[n].renderer, "text", n, (void*)0);
      gtk_tree_view_column_set_resizable(column[n].column, true);
    }
    column[n].label = gtk_label_new(header);
    gtk_tree_view_column_set_widget(GTK_TREE_VIEW_COLUMN(column[n].column), column[n].label);
    gtk_tree_view_append_column(GTK_TREE_VIEW(subWidget), column[n].column);
    gtk_widget_show(column[n].label);
  }

  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(subWidget), headerText.size() >= 3);  //two or more columns + checkbox column
  gtk_tree_view_set_search_column(GTK_TREE_VIEW(subWidget), 1);

  g_signal_connect_swapped(G_OBJECT(subWidget), "cursor-changed", G_CALLBACK(ListView_change), (gpointer)&listView);
  g_signal_connect_swapped(G_OBJECT(subWidget), "row-activated", G_CALLBACK(ListView_activate), (gpointer)&listView);

  setHeaderVisible(listView.state.headerVisible);
  setCheckable(listView.state.checkable);
  foreach(text, listView.state.text) append(text);
  foreach(checked, listView.state.checked, n) setChecked(n, checked);
  if(auto selection = listView.state.selection) setSelection(selection());
  autosizeColumns();

  gtk_widget_show(subWidget);
}

void pListView::setFocused() {
  gtk_widget_grab_focus(subWidget);
}

void pListView::setFont(Font &font) {
  pWidget::setFont(font);
  for(unsigned n = 0; n < 1 + listView.state.headerText.size(); n++) {
    gtk_widget_modify_font(column[n].label, font.p.gtkFont);
  }
}
