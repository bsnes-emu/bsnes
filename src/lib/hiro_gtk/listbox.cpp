void hiro_plistbox_change(pListbox *p) {
  if(p->listbox_selection == p->get_selection()) return;
  if(p->self.on_change) p->self.on_change(Event(Event::Change, p->listbox_selection = p->get_selection(), &p->self));
}

void hiro_plistbox_activate(pListbox *p) {
  if(p->self.on_activate) p->self.on_activate(Event(Event::Activate, p->listbox_selection = p->get_selection(), &p->self));
}

void pListbox::create(uint style, uint width, uint height, const char *columns, const char *text) {
bool header = style & Listbox::Header;
GtkPolicyType hscroll = (style & Listbox::HorizontalScrollAlways) ? GTK_POLICY_ALWAYS :
                        (style & Listbox::HorizontalScrollNever) ? GTK_POLICY_NEVER :
                        GTK_POLICY_AUTOMATIC;
GtkPolicyType vscroll = (style & Listbox::VerticalScrollAlways) ? GTK_POLICY_ALWAYS :
                        (style & Listbox::VerticalScrollNever) ? GTK_POLICY_NEVER :
                        GTK_POLICY_AUTOMATIC;

  scrollbox = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbox), hscroll, vscroll);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollbox), GTK_SHADOW_ETCHED_IN);

lstring list;
  split(list, "\t", columns);

GType *v = (GType*)malloc(count(list) * sizeof(GType));
  for(uint i = 0; i < count(list); i++) v[i] = G_TYPE_STRING;
  store = gtk_list_store_newv(count(list), v);
  free(v);

  listbox = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_container_add(GTK_CONTAINER(scrollbox), listbox);
  g_object_unref(G_OBJECT(store));
  gtk_widget_set_size_request(scrollbox, width, height);
  gtk_widget_show(listbox);
  gtk_widget_show(scrollbox);

//alternate colors for each listbox entry if there are multiple columns ...
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(listbox), count(list) >= 2 ? true : false);
  for(uint i = 0; i < count(list); i++) {
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(list[i], renderer, "text", i, (void*)0);
    column_list[column_list.size()] = column;
    gtk_tree_view_append_column(GTK_TREE_VIEW(listbox), column);
  }

  if(text && *text) {
    split(list, "\n", text);
    for(uint i = 0; i < count(list); i++) add_item(list[i]);
  }

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listbox), header);
  autosize_columns();

  g_signal_connect_swapped(G_OBJECT(listbox), "cursor-changed", G_CALLBACK(hiro_plistbox_change), (gpointer)this);
  g_signal_connect_swapped(G_OBJECT(listbox), "row-activated", G_CALLBACK(hiro_plistbox_activate), (gpointer)this);
}

void pListbox::autosize_columns() {
  gtk_tree_view_columns_autosize(GTK_TREE_VIEW(listbox));
}

void pListbox::set_column_width(uint column, uint width) {
  gtk_tree_view_column_set_min_width(column_list[column], width);
  gtk_tree_view_column_set_max_width(column_list[column], width);
}

void pListbox::add_item(const char *text) {
lstring list;
  split(list, "\t", text);
  gtk_list_store_append(store, &iter);
  for(uint i = 0; i < count(list); i++) {
    gtk_list_store_set(store, &iter, i, list[i](), -1);
  }
}

void pListbox::set_item(uint index, const char *text) {
GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(listbox));
  for(uint i = 0; i <= index; i++) {
    i == 0 ?
      gtk_tree_model_get_iter_first(model, &iter) :
      gtk_tree_model_iter_next(model, &iter);
  }

lstring list;
  split(list, "\t", text);
  for(uint i = 0; i < count(list); i++) {
    gtk_list_store_set(store, &iter, i, list[i](), -1);
  }
}

int pListbox::get_selection() {
GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(listbox));
GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(listbox));
  if(gtk_tree_model_get_iter_first(model, &iter) == false) { return -1; }
  if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) { return 0; }
  for(uint i = 1; i < 100000; i++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) { return -1; }
    if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) { return i; }
  }
  return -1;
}

void pListbox::set_selection(int index) {
int current = get_selection();
GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(listbox));
GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(listbox));
  gtk_tree_selection_unselect_all(selection);
  if(index < 0) { goto end; }
  if(gtk_tree_model_get_iter_first(model, &iter) == false) { goto end; }
  if(index == 0) { gtk_tree_selection_select_iter(selection, &iter); goto end; }
  for(uint i = 1; i < 100000; i++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) { goto end; }
    if(index == i) { gtk_tree_selection_select_iter(selection, &iter); goto end; }
  }
end:
  if(current != index) ;//{ owner->message(Message::Changed, (uintptr_t)this); }
}

void pListbox::reset() {
  gtk_list_store_clear(GTK_LIST_STORE(store));
  gtk_tree_view_set_model(GTK_TREE_VIEW(listbox), GTK_TREE_MODEL(store));
}

pListbox::pListbox(Listbox &self_) : pFormControl(self_), self(self_) {
  scrollbox = 0;
  listbox = 0;
  listbox_selection = -1;
}

/* internal */

GtkWidget* pListbox::gtk_handle() {
  return scrollbox;
}
