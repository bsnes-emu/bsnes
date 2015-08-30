#if defined(Hiro_TreeView)

namespace hiro {

//gtk_tree_view_collapse_all(gtkTreeView);
//gtk_tree_view_expand_all(gtkTreeView);

static auto TreeView_activate(GtkTreeView*, GtkTreePath* gtkPath, GtkTreeViewColumn*, pTreeView* p) -> void { p->_activatePath(gtkPath); }
static auto TreeView_buttonEvent(GtkTreeView*, GdkEventButton* gdkEvent, pTreeView* p) -> signed { return p->_buttonEvent(gdkEvent); }
static auto TreeView_change(GtkTreeSelection*, pTreeView* p) -> void { p->_updateSelected(); }
static auto TreeView_context(GtkTreeView*, pTreeView* p) -> void { p->self().doContext(); }
static auto TreeView_dataFunc(GtkTreeViewColumn* column, GtkCellRenderer* renderer, GtkTreeModel* model, GtkTreeIter* iter, pTreeView* p) -> void { return p->_doDataFunc(column, renderer, iter); }
static auto TreeView_toggle(GtkCellRendererToggle*, char* path, pTreeView* p) -> void { p->_togglePath(path); }

auto pTreeView::construct() -> void {
  gtkWidget = gtk_scrolled_window_new(0, 0);
  gtkScrolledWindow = GTK_SCROLLED_WINDOW(gtkWidget);
  gtk_scrolled_window_set_policy(gtkScrolledWindow, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(gtkScrolledWindow, GTK_SHADOW_ETCHED_IN);

  gtkTreeStore = gtk_tree_store_new(3, G_TYPE_BOOLEAN, GDK_TYPE_PIXBUF, G_TYPE_STRING);
  gtkTreeModel = GTK_TREE_MODEL(gtkTreeStore);

  gtkWidgetChild = gtk_tree_view_new_with_model(gtkTreeModel);
  gtkTreeView = GTK_TREE_VIEW(gtkWidgetChild);
  gtkTreeSelection = gtk_tree_view_get_selection(gtkTreeView);
  gtk_tree_view_set_headers_visible(gtkTreeView, false);
  gtk_container_add(GTK_CONTAINER(gtkWidget), gtkWidgetChild);
  gtk_widget_show(gtkWidgetChild);

  gtkTreeViewColumn = gtk_tree_view_column_new();

  gtkCellToggle = gtk_cell_renderer_toggle_new();
  gtk_tree_view_column_pack_start(gtkTreeViewColumn, gtkCellToggle, false);
  gtk_tree_view_column_set_attributes(gtkTreeViewColumn, gtkCellToggle, "active", 0, nullptr);
  gtk_tree_view_column_set_cell_data_func(gtkTreeViewColumn, GTK_CELL_RENDERER(gtkCellToggle), (GtkTreeCellDataFunc)TreeView_dataFunc, (gpointer)this, nullptr);

  gtkCellPixbuf = gtk_cell_renderer_pixbuf_new();
  gtk_tree_view_column_pack_start(gtkTreeViewColumn, gtkCellPixbuf, false);
  gtk_tree_view_column_set_attributes(gtkTreeViewColumn, gtkCellPixbuf, "pixbuf", 1, nullptr);
  gtk_tree_view_column_set_cell_data_func(gtkTreeViewColumn, GTK_CELL_RENDERER(gtkCellPixbuf), (GtkTreeCellDataFunc)TreeView_dataFunc, (gpointer)this, nullptr);

  gtkCellText = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(gtkTreeViewColumn, gtkCellText, true);
  gtk_tree_view_column_set_attributes(gtkTreeViewColumn, gtkCellText, "text", 2, nullptr);
  gtk_tree_view_column_set_cell_data_func(gtkTreeViewColumn, GTK_CELL_RENDERER(gtkCellText), (GtkTreeCellDataFunc)TreeView_dataFunc, (gpointer)this, nullptr);

  gtk_tree_view_append_column(gtkTreeView, gtkTreeViewColumn);
  gtk_tree_view_set_search_column(gtkTreeView, 2);

  setBackgroundColor(state().backgroundColor);
  setForegroundColor(state().foregroundColor);

  g_signal_connect(G_OBJECT(gtkWidgetChild), "button-press-event", G_CALLBACK(TreeView_buttonEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidgetChild), "button-release-event", G_CALLBACK(TreeView_buttonEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidgetChild), "popup-menu", G_CALLBACK(TreeView_context), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidgetChild), "row-activated", G_CALLBACK(TreeView_activate), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeSelection), "changed", G_CALLBACK(TreeView_change), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkCellToggle), "toggled", G_CALLBACK(TreeView_toggle), (gpointer)this);

  pWidget::construct();
}

auto pTreeView::destruct() -> void {
  gtk_widget_destroy(gtkWidgetChild);
  gtk_widget_destroy(gtkWidget);
}

//

auto pTreeView::append(sTreeViewItem item) -> void {
}

auto pTreeView::remove(sTreeViewItem item) -> void {
}

auto pTreeView::setBackgroundColor(Color color) -> void {
  auto gdkColor = CreateColor(color);
  gtk_widget_modify_base(gtkWidgetChild, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pTreeView::setForegroundColor(Color color) -> void {
  auto gdkColor = CreateColor(color);
  gtk_widget_modify_text(gtkWidgetChild, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

//

auto pTreeView::_activatePath(GtkTreePath* gtkPath) -> void {
  char* path = gtk_tree_path_to_string(gtkPath);
  if(auto item = self().item(string{path}.transform(":", "/"))) {
    if(!locked()) self().doActivate();
  }
  g_free(path);
}

auto pTreeView::_buttonEvent(GdkEventButton* gdkEvent) -> signed {
  GtkTreePath* gtkPath = nullptr;
  gtk_tree_view_get_path_at_pos(gtkTreeView, gdkEvent->x, gdkEvent->y, &gtkPath, nullptr, nullptr, nullptr);

  if(gdkEvent->type == GDK_BUTTON_PRESS) {
    //detect when the empty space of the GtkTreeView is clicked; and clear the selection
    if(gtkPath == nullptr && gtk_tree_selection_count_selected_rows(gtkTreeSelection) > 0) {
      gtk_tree_selection_unselect_all(gtkTreeSelection);
      state().selectedPath.reset();
      self().doChange();
      return true;
    }
  }

  if(gdkEvent->type == GDK_BUTTON_RELEASE && gdkEvent->button == 3) {
    //handle right-click context menu
    //have to detect on button release instead of press; as GTK+ does not update new selection prior to press event
    self().doContext();
    return false;
  }

  return false;
}

auto pTreeView::_doDataFunc(GtkTreeViewColumn* column, GtkCellRenderer* renderer, GtkTreeIter* iter) -> void {
  auto path = gtk_tree_model_get_string_from_iter(gtkTreeModel, iter);
  auto parts = string{path}.split(":");
  g_free(path);

  auto item = self().item(decimal(parts.takeFirst()));
  if(!item) return;
  while(parts) {
    item = item.item(decimal(parts.takeFirst()));
    if(!item) return;
  }

  if(renderer == GTK_CELL_RENDERER(gtkCellToggle)) {
    gtk_cell_renderer_set_visible(renderer, item->state.checkable);
  } else if(renderer == GTK_CELL_RENDERER(gtkCellPixbuf)) {
    gtk_cell_renderer_set_visible(renderer, (bool)item->state.image);
  } else if(renderer == GTK_CELL_RENDERER(gtkCellText)) {
    auto font = pFont::create(item->font(true));
    g_object_set(G_OBJECT(renderer), "font-desc", font, nullptr);
    pango_font_description_free(font);
    if(auto color = item->foregroundColor(true)) {
      auto gdkColor = CreateColor(color);
      g_object_set(G_OBJECT(renderer), "foreground-gdk", &gdkColor, nullptr);
    } else {
      g_object_set(G_OBJECT(renderer), "foreground-set", false, nullptr);
    }
  }
  if(auto color = item->backgroundColor(true)) {
    auto gdkColor = CreateColor(color);
    g_object_set(G_OBJECT(renderer), "cell-background-gdk", &gdkColor, nullptr);
  } else {
    g_object_set(G_OBJECT(renderer), "cell-background-set", false, nullptr);
  }
}

auto pTreeView::_togglePath(string path) -> void {
  if(auto item = self().item(path.transform(":", "/"))) {
    bool checked = !item->checked();
    gtk_tree_store_set(gtkTreeStore, &item->self()->gtkIter, 0, checked, -1);
    item->state.checked = checked;
    if(!locked()) self().doToggle(item);
  }
}

auto pTreeView::_updateSelected() -> void {
  GtkTreeIter iter;
  if(gtk_tree_selection_get_selected(gtkTreeSelection, &gtkTreeModel, &iter)) {
    char* gtkPath = gtk_tree_model_get_string_from_iter(gtkTreeModel, &iter);
    string path = string{gtkPath}.transform(":", "/");
    g_free(gtkPath);
    if(state().selectedPath != path) {
      state().selectedPath = path;
      if(!locked()) self().doChange();
    }
  } else if(state().selectedPath) {
    state().selectedPath.reset();
    if(!locked()) self().doChange();
  }
}

}

#endif
