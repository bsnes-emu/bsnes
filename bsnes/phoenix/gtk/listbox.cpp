static void ListBox_change(ListBox *self) {
  signed selection = -1;
  if(auto position = self->selection()) selection = position();
  if(selection == self->listBox->selection) return;
  self->listBox->selection = selection;
  if(self->onChange) self->onChange();
}

static void ListBox_activate(ListBox *self) {
  signed selection = -1;
  if(auto position = self->selection()) selection = position();
  self->listBox->selection = selection;
  if(self->onActivate) self->onActivate();
}

void ListBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  listBox->selection = -1;
  object->widget = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(object->widget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(object->widget), GTK_SHADOW_ETCHED_IN);
  gtk_widget_set_size_request(object->widget, width, height);

  lstring list;
  list.split("\t", text);

  GType *v = (GType*)malloc(list.size() * sizeof(GType));
  for(unsigned i = 0; i < list.size(); i++) v[i] = G_TYPE_STRING;
  listBox->store = gtk_list_store_newv(list.size(), v);
  free(v);

  object->subWidget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(listBox->store));
  gtk_container_add(GTK_CONTAINER(object->widget), object->subWidget);
  g_object_unref(G_OBJECT(listBox->store));

  //alternate color of each row if there is more than one column
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(object->subWidget), list.size() >= 2);
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(object->subWidget), false);

  for(unsigned i = 0; i < list.size(); i++) {
    listBox->column[i].renderer = gtk_cell_renderer_text_new();
    listBox->column[i].column = gtk_tree_view_column_new_with_attributes(
      list[i], listBox->column[i].renderer, "text", i, (void*)0
    );
    listBox->column[i].label = gtk_label_new(list[i]);
    gtk_tree_view_column_set_widget(GTK_TREE_VIEW_COLUMN(listBox->column[i].column), listBox->column[i].label);
    gtk_tree_view_append_column(GTK_TREE_VIEW(object->subWidget), listBox->column[i].column);
    gtk_widget_show(listBox->column[i].label);
  }

  g_signal_connect_swapped(G_OBJECT(object->subWidget), "cursor-changed", G_CALLBACK(ListBox_change), (gpointer)this);
  g_signal_connect_swapped(G_OBJECT(object->subWidget), "row-activated", G_CALLBACK(ListBox_activate), (gpointer)this);

  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->subWidget);
  gtk_widget_show(object->widget);
}

void ListBox::setFocused() {
  gtk_widget_grab_focus(object->subWidget);
}

void ListBox::setHeaderVisible(bool visible) {
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(object->subWidget), visible);
}

void ListBox::setFont(Font &font) {
  Widget::setFont(font);
  unsigned columns = 1;
  while(true) {
    if(gtk_tree_view_get_column(GTK_TREE_VIEW(object->subWidget), columns) == 0) break;
    columns++;
  }
  for(unsigned i = 0; i < columns; i++) {
    gtk_widget_modify_font(listBox->column[i].label, font.font->font);
  }
}

void ListBox::reset() {
  listBox->selection = -1;
  gtk_list_store_clear(GTK_LIST_STORE(listBox->store));
  gtk_tree_view_set_model(GTK_TREE_VIEW(object->subWidget), GTK_TREE_MODEL(listBox->store));
  //reset gtk_scrolled_window scrollbar position to 0,0 (top-left), as ListBox is now empty
  gtk_scrolled_window_set_hadjustment(GTK_SCROLLED_WINDOW(object->widget), 0);
  gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(object->widget), 0);
}

void ListBox::resizeColumnsToContent() {
  gtk_tree_view_columns_autosize(GTK_TREE_VIEW(object->subWidget));
}

void ListBox::addItem(const char *text) {
  lstring list;
  list.split("\t", text);
  GtkTreeIter iter;
  gtk_list_store_append(listBox->store, &iter);
  for(unsigned i = 0; i < list.size(); i++) {
    gtk_list_store_set(listBox->store, &iter, i, (const char*)list[i], -1);
  }
}

void ListBox::setItem(unsigned row, const char *text) {
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(object->subWidget));
  GtkTreeIter iter;
  for(unsigned i = 0; i <= row; i++) {
    if(i == 0) gtk_tree_model_get_iter_first(model, &iter);
    else gtk_tree_model_iter_next(model, &iter);
  }

  lstring list;
  list.split("\t", text);
  for(unsigned i = 0; i < list.size(); i++) {
    gtk_list_store_set(listBox->store, &iter, i, (const char*)list[i], -1);
  }
}

optional<unsigned> ListBox::selection() {
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(object->subWidget));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(object->subWidget));
  GtkTreeIter iter;
  if(gtk_tree_model_get_iter_first(model, &iter) == false) return { false, 0 };
  if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) return { true, 0 };
  for(unsigned i = 1;; i++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) return { false, 0 };
    if(gtk_tree_selection_iter_is_selected(selection, &iter) == true) return { true, i };
  }
  return { false, 0 };
}

void ListBox::setSelection(unsigned row) {
  signed current = -1;
  if(auto position = selection()) current = position();
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(object->subWidget));
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(object->subWidget));
  gtk_tree_selection_unselect_all(selection);

  GtkTreeIter iter;
  if(gtk_tree_model_get_iter_first(model, &iter) == false) return;
  if(row == 0) {
    gtk_tree_selection_select_iter(selection, &iter);
    return;
  }
  for(unsigned i = 1;; i++) {
    if(gtk_tree_model_iter_next(model, &iter) == false) return;
    if(row == i) {
      gtk_tree_selection_select_iter(selection, &iter);
      return;
    }
  }
}

ListBox::ListBox() {
  listBox = new ListBox::Data;
}
