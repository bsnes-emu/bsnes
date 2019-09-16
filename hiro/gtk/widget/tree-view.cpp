#if defined(Hiro_TreeView)

namespace hiro {

static const uint TreeViewIndentation = 20;

//gtk_tree_view_collapse_all(gtkTreeView);
//gtk_tree_view_expand_all(gtkTreeView);

static auto TreeView_activate(GtkTreeView*, GtkTreePath* gtkPath, GtkTreeViewColumn*, pTreeView* p) -> void { p->_activatePath(gtkPath); }
static auto TreeView_buttonEvent(GtkTreeView*, GdkEventButton* gdkEvent, pTreeView* p) -> signed { return p->_buttonEvent(gdkEvent); }
static auto TreeView_change(GtkTreeSelection*, pTreeView* p) -> void { p->_updateSelected(); }
static auto TreeView_context(GtkTreeView*, pTreeView* p) -> void { p->self().doContext(); }
static auto TreeView_dataFunc(GtkTreeViewColumn* column, GtkCellRenderer* renderer, GtkTreeModel* model, GtkTreeIter* iter, pTreeView* p) -> void { return p->_doDataFunc(column, renderer, iter); }
static auto TreeView_keyPress(GtkWidget*, GdkEventKey*, pTreeView* p) -> int { p->suppressActivate = false; return false; }
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
  gtk_tree_view_set_show_expanders(gtkTreeView, false);
  gtk_tree_view_set_level_indentation(gtkTreeView, TreeViewIndentation);
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

  setActivation(state().activation);
  setBackgroundColor(state().backgroundColor);
  setForegroundColor(state().foregroundColor);

  g_signal_connect(G_OBJECT(gtkWidgetChild), "button-press-event", G_CALLBACK(TreeView_buttonEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidgetChild), "button-release-event", G_CALLBACK(TreeView_buttonEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidgetChild), "key-press-event", G_CALLBACK(TreeView_keyPress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidgetChild), "popup-menu", G_CALLBACK(TreeView_context), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidgetChild), "row-activated", G_CALLBACK(TreeView_activate), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeSelection), "changed", G_CALLBACK(TreeView_change), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkCellToggle), "toggled", G_CALLBACK(TreeView_toggle), (gpointer)this);

  //Ctrl+F triggers a small popup window at the bottom of the GtkTreeView, which clears the currently selected item(s)
  //this is undesirable for amethyst, which uses the active item to display a document to edit, and binds Ctrl+F to a document find function
  //for now, disable GtkTreeView's interactive search: longer term, more thought will need to go into if this is ever desirable or not
  //gtk_tree_view_set_enable_search(gtkTreeView, false) does not work
  //gtk_tree_view_set_search_column(gtkTreeView, -1) does not work
  gtkEntry = (GtkEntry*)gtk_entry_new();
  gtk_tree_view_set_search_entry(gtkTreeView, gtkEntry);

  pWidget::construct();
}

auto pTreeView::destruct() -> void {
  gtk_widget_destroy(GTK_WIDGET(gtkEntry));
  gtk_widget_destroy(gtkWidgetChild);
  gtk_widget_destroy(gtkWidget);
}

//

auto pTreeView::append(sTreeViewItem item) -> void {
}

auto pTreeView::remove(sTreeViewItem item) -> void {
}

auto pTreeView::setActivation(Mouse::Click activation) -> void {
  //handled by callbacks
}

auto pTreeView::setBackgroundColor(Color color) -> void {
  auto gdkColor = CreateColor(color);
  gtk_widget_modify_base(gtkWidgetChild, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pTreeView::setFocused() -> void {
  //gtk_widget_grab_focus() will select the first item if nothing is currently selected
  //this behavior is undesirable. detect selection state first, and restore if required
  lock();
  bool selected = gtk_tree_selection_get_selected(gtkTreeSelection, nullptr, nullptr);
  gtk_widget_grab_focus(gtkWidgetChild);
  if(!selected) gtk_tree_selection_unselect_all(gtkTreeSelection);
  unlock();
}

auto pTreeView::setForegroundColor(Color color) -> void {
  auto gdkColor = CreateColor(color);
  gtk_widget_modify_text(gtkWidgetChild, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pTreeView::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  _updateScrollBars();
}

//

auto pTreeView::_activatePath(GtkTreePath* gtkPath) -> void {
  if(suppressActivate) {
    suppressActivate = false;
    return;
  }

  char* path = gtk_tree_path_to_string(gtkPath);
  if(auto item = self().item(string{path}.transform(":", "/"))) {
    if(!locked()) self().doActivate();
  }
  g_free(path);
}

auto pTreeView::_buttonEvent(GdkEventButton* gdkEvent) -> signed {
  if(gdkEvent->type == GDK_BUTTON_PRESS) {
    //detect when the empty space of the GtkTreeView is clicked; and clear the selection
    GtkTreePath* gtkPath = nullptr;
    gtk_tree_view_get_path_at_pos(gtkTreeView, gdkEvent->x, gdkEvent->y, &gtkPath, nullptr, nullptr, nullptr);
    if(!gtkPath) {
      //the first time a GtkTreeView widget is clicked, even if the empty space of the widget is clicked,
      //a "changed" signal will be sent after the "button-press-event", to activate the first item in the tree
      //this is undesirable, so set a flag to undo the next selection change during the "changed" signal
      suppressChange = true;
      if(gtk_tree_selection_count_selected_rows(gtkTreeSelection) > 0) {
        gtk_tree_selection_unselect_all(gtkTreeSelection);
        state().selectedPath.reset();
        self().doChange();
        return true;
      }
    }

    if(gdkEvent->button == 1) {
      //emulate activate-on-single-click, which is only available in GTK+ 3.8 and later
      if(gtkPath && self().activation() == Mouse::Click::Single) {
        //selectedPath must be updated for TreeView::doActivate() to act on the correct TreeViewItem.
        //as this will then cause "changed" to not see that the path has changed, we must handle that case here as well.
        char* path = gtk_tree_path_to_string(gtkPath);
        string selectedPath = string{path}.transform(":", "/");
        g_free(path);
        if(state().selectedPath != selectedPath) {
          state().selectedPath = selectedPath;
          self().doChange();
        }
        self().doActivate();
        //"row-activated" is sent before "button-press-event" (GDK_2BUTTON_PRESS);
        //so stop a double-click from calling TreeView::doActivate() twice by setting a flag after single-clicks
        suppressActivate = true;  //key presses will clear this flag to allow key-activations to work correctly
      }
    }

    if(gdkEvent->button == 3) {
      //multi-selection mode: (not implemented in TreeView yet ... but code is here anyway for future use)
      //if multiple items are selected, and one item is right-clicked on (for a context menu), GTK clears selection on all other items
      //block this behavior so that onContext() handler can work on more than one selected item at a time
      if(gtkPath && gtk_tree_selection_path_is_selected(gtkTreeSelection, gtkPath)) return true;
    }
  }

  if(gdkEvent->type == GDK_BUTTON_RELEASE) {
    suppressChange = false;
    if(gdkEvent->button == 3) {
      //handle action during right-click release; as button-press-event is sent prior to selection update
      //without this, the callback handler would see the previous selection state instead
      self().doContext();
      return false;
    }
  }

  return false;
}

auto pTreeView::_doDataFunc(GtkTreeViewColumn* column, GtkCellRenderer* renderer, GtkTreeIter* iter) -> void {
  auto path = gtk_tree_model_get_string_from_iter(gtkTreeModel, iter);
  auto parts = string{path}.split(":");
  g_free(path);

  auto item = self().item(parts.takeLeft().natural());
  if(!item) return;
  while(parts) {
    item = item.item(parts.takeLeft().natural());
    if(!item) return;
  }

  if(renderer == GTK_CELL_RENDERER(gtkCellToggle)) {
    gtk_cell_renderer_set_visible(renderer, item->state.checkable);
  } else if(renderer == GTK_CELL_RENDERER(gtkCellPixbuf)) {
    gtk_cell_renderer_set_visible(renderer, (bool)item->state.icon);
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

//it is necessary to compute the minimum width necessary to show all items in a tree,
//before a horizontal scroll bar must be shown. this is because GTK2 (and possibly GTK3)
//fail to subtract the tree view indentation level on items before determining if the
//horizontal scroll bar is necessary. as a result, without this, the scroll bar shows up
//far before it is necessary, and gets worse the more nested the tree is.
//
//this is called whenever the TreeView geometry changes, or whenever a TreeViewItem's
//checkability, icon, or text is updated. in other words, whenever the need for a horizontal
//scroll bar to show all items in the tree is necessary or not.
auto pTreeView::_updateScrollBars() -> void {
  int maximumWidth = self().geometry().width() - 6;
  if(auto scrollBar = gtk_scrolled_window_get_vscrollbar(gtkScrolledWindow)) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(scrollBar, &allocation);
    if(gtk_widget_get_visible(scrollBar)) maximumWidth -= allocation.width;
  }

  int minimumWidth = 0;
  for(auto& item : state().items) {
    if(auto self = item->self()) {
      minimumWidth = max(minimumWidth, self->_minimumWidth());
    }
  }

  gtk_scrolled_window_set_policy(gtkScrolledWindow,
    minimumWidth >= maximumWidth ? GTK_POLICY_ALWAYS : GTK_POLICY_NEVER,
    GTK_POLICY_AUTOMATIC);
}

auto pTreeView::_updateSelected() -> void {
  if(suppressChange) {
    suppressChange = false;
    gtk_tree_selection_unselect_all(gtkTreeSelection);
    return;
  }

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
