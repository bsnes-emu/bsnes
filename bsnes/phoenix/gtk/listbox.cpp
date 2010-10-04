static void ListBox_activate(ListBox *self) {
  signed selection = -1;
  if(auto position = self->selection()) selection = position();
  self->listBox->selection = selection;
  if(self->onActivate) self->onActivate();
}

static void ListBox_change(ListBox *self) {
  signed selection = -1;
  if(auto position = self->selection()) selection = position();
  if(selection == self->listBox->selection) return;
  self->listBox->selection = selection;
  if(self->onChange) self->onChange();
}

static void ListBox_tick(GtkCellRendererToggle *cell, gchar *path_string, ListBox *self) {
  unsigned index = strunsigned(path_string);
  self->setChecked(index, !self->checked(index));
  if(self->onTick) self->onTick(index);
}

void ListBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  listBox->selection = -1;
  object->widget = gtk_scrolled_window_new(0, 0);
  widget->parent = &parent;
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(object->widget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(object->widget), GTK_SHADOW_ETCHED_IN);
  gtk_widget_set_size_request(object->widget, width, height);

  lstring list;
  list.split("\t", string("\t", text));

  GType *v = (GType*)malloc(list.size() * sizeof(GType));
  for(unsigned i = 0; i < list.size(); i++) v[i] = (i == 0 ? G_TYPE_BOOLEAN : G_TYPE_STRING);
  listBox->store = gtk_list_store_newv(list.size(), v);
  free(v);

  object->subWidget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(listBox->store));
  gtk_container_add(GTK_CONTAINER(object->widget), object->subWidget);
  g_object_unref(G_OBJECT(listBox->store));

  for(unsigned i = 0; i < list.size(); i++) {
    if(i == 0) {
      listBox->column[i].renderer = gtk_cell_renderer_toggle_new();
      listBox->column[i].column = gtk_tree_view_column_new_with_attributes(
        "", listBox->column[i].renderer, "active", i, (void*)0
      );
      gtk_tree_view_column_set_resizable(listBox->column[i].column, false);
      gtk_tree_view_column_set_visible(listBox->column[i].column, listBox->checkable);
      g_signal_connect(listBox->column[i].renderer, "toggled", G_CALLBACK(ListBox_tick), (gpointer)this);
    } else {
      listBox->column[i].renderer = gtk_cell_renderer_text_new();
      listBox->column[i].column = gtk_tree_view_column_new_with_attributes(
        "", listBox->column[i].renderer, "text", i, (void*)0
      );
      gtk_tree_view_column_set_resizable(listBox->column[i].column, true);
    }
    listBox->column[i].label = gtk_label_new(list[i]);
    gtk_tree_view_column_set_widget(GTK_TREE_VIEW_COLUMN(listBox->column[i].column), listBox->column[i].label);
    gtk_tree_view_append_column(GTK_TREE_VIEW(object->subWidget), listBox->column[i].column);
    gtk_widget_show(listBox->column[i].label);
  }
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(object->subWidget), false);
  gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(object->subWidget), list.size() >= 3);  //>= 2 + one for the checkbox column
  gtk_tree_view_set_search_column(GTK_TREE_VIEW(object->subWidget), 1);

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

void ListBox::setCheckable(bool checkable) {
  listBox->checkable = checkable;
  if(object->subWidget) gtk_tree_view_column_set_visible(listBox->column[0].column, checkable);
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
  unsigned index = 1;
  foreach(item, list) gtk_list_store_set(listBox->store, &iter, index++, (const char*)item, -1);
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
  unsigned index = 1;
  foreach(item, list) gtk_list_store_set(listBox->store, &iter, index++, (const char*)item, -1);
}

bool ListBox::checked(unsigned row) {
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(object->subWidget));
  GtkTreePath *path = gtk_tree_path_new_from_string(string(row));
  GtkTreeIter iter;
  bool state;
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter, 0, &state, -1);
  gtk_tree_path_free(path);
  return state;
}

void ListBox::setChecked(unsigned row, bool checked) {
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(object->subWidget));
  GtkTreePath *path = gtk_tree_path_new_from_string(string(row));
  GtkTreeIter iter;
  gtk_tree_model_get_iter(model, &iter, path);
  gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, checked, -1);
  gtk_tree_path_free(path);
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
  listBox->checkable = false;
}
