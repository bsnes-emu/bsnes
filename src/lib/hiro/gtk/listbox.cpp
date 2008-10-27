static void hiro_plistbox_change(pListbox *p) {
  //only send message when active item changes
  if(p->listbox_selection == p->get_selection()) return;

  p->listbox_selection = p->get_selection();
  if(p->self.on_change) {
    p->self.on_change(event_t(event_t::Change, p->listbox_selection, &p->self));
  }
}

static void hiro_plistbox_activate(pListbox *p) {
  p->listbox_selection = p->get_selection();
  if(p->self.on_activate) {
    p->self.on_activate(event_t(event_t::Activate, p->listbox_selection, &p->self));
  }
}

void pListbox::create(unsigned style, unsigned width, unsigned height, const char *columns, const char *text) {
  bool header = style & Listbox::Header;
  GtkPolicyType hscroll = (style & Listbox::HorizontalScrollAlways) ? GTK_POLICY_ALWAYS :
                          (style & Listbox::HorizontalScrollNever ) ? GTK_POLICY_NEVER  :
                          GTK_POLICY_AUTOMATIC;
  GtkPolicyType vscroll = (style & Listbox::VerticalScrollAlways) ? GTK_POLICY_ALWAYS :
                          (style & Listbox::VerticalScrollNever ) ? GTK_POLICY_NEVER  :
                          GTK_POLICY_AUTOMATIC;

  scrollbox = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbox), hscroll, vscroll);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollbox), GTK_SHADOW_ETCHED_IN);

  lstring list;
  split(list, "\t", columns);

  GType *v = (GType*)malloc(count(list) * sizeof(GType));
  for(unsigned i = 0; i < count(list); i++) v[i] = G_TYPE_STRING;
  store = gtk_list_store_newv(count(list), v);
  free(v);

  listbox = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_container_add(GTK_CONTAINER(scrollbox), listbox);
  g_object_unref(G_OBJECT(store));
  gtk_widget_set_size_request(scrollbox, width, height);
  gtk_widget_show(listbox);
  gtk_widget_show(scrollbox);

  //alternate colors for each listbox entry if there are multiple columns
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(listbox), count(list) >= 2 ? true : false);
  for(unsigned i = 0; i < count(list); i++) {
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(list[i], renderer, "text", i, (void*)0);
    column_list[column_list.size()] = column;
    gtk_tree_view_append_column(GTK_TREE_VIEW(listbox), column);
  }

  if(text && *text) {
    split(list, "\n", text);
    for(unsigned i = 0; i < count(list); i++) add_item(list[i]);
  }

  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listbox), header);
  autosize_columns();

  g_signal_connect_swapped(G_OBJECT(listbox), "cursor-changed", G_CALLBACK(hiro_plistbox_change), (gpointer)this);
  g_signal_connect_swapped(G_OBJECT(listbox), "row-activated", G_CALLBACK(hiro_plistbox_activate), (gpointer)this);

  set_default_font(listbox);
}

void pListbox::autosize_columns() {
  gtk_tree_view_columns_autosize(GTK_TREE_VIEW(listbox));
}

void pListbox::set_column_width(unsigned column, unsigned width) {
  gtk_tree_view_column_set_min_width(column_list[column], width);
  gtk_tree_view_column_set_max_width(column_list[column], width);
}

void pListbox::add_item(const char *text) {
  lstring list;
  split(list, "\t", text);
  gtk_list_store_append(store, &iter);
  for(unsigned i = 0; i < count(list); i++) {
    gtk_list_store_set(store, &iter, i, (const char*)list[i], -1);
  }
}

void pListbox::set_item(unsigned index, const char *text) {
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(listbox));
  for(unsigned i = 0; i <= index; i++) {
    i == 0 ?
      gtk_tree_model_get_iter_first(model, &iter) :
      gtk_tree_model_iter_next(model, &iter);
  }

  lstring list;
  split(list, "\t", text);
  for(unsigned i = 0; i < count(list); i++) {
    gtk_list_store_set(store, &iter, i, (const char*)list[i], -1);
  }
}

int pListbox::get_selection() {
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(listbox));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(listbox));
  if(gtk_tree_model_get_iter_first(model, &iter) == false) return -1;
  if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) return 0;
  for(unsigned i = 1; i < 100000; i++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) return -1;
    if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) return i;
  }
  return -1;
}

void pListbox::set_selection(int index) {
  int current = get_selection();
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(listbox));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(listbox));
  gtk_tree_selection_unselect_all(selection);
  if(index < 0) return;  //nothing to select?

  if(gtk_tree_model_get_iter_first(model, &iter)) {
    if(index == 0) {
      gtk_tree_selection_select_iter(selection, &iter);
    } else {
      for(unsigned i = 1; i < 100000; i++) {
        if(gtk_tree_model_iter_next(model, &iter) == false) break;
        if(index == i) {
          gtk_tree_selection_select_iter(selection, &iter);
          break;
        }
      }
    }
  }
}

void pListbox::reset() {
  listbox_selection = -1;
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
