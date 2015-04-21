namespace hiro {

static auto ListView_activate(GtkTreeView*, GtkTreePath*, GtkTreeViewColumn*, pListView* p) -> void { return p->_doActivate(); }
static auto ListView_buttonEvent(GtkTreeView* treeView, GdkEventButton* event, pListView* p) -> signed { return p->_doEvent(event); }
static auto ListView_change(GtkTreeSelection*, pListView* p) -> void { return p->_doChange(); }
static auto ListView_edit(GtkCellRendererText* renderer, const char* path, const char* text, pListView* p) -> void { return p->_doEdit(renderer, path, text); }
static auto ListView_headerActivate(GtkTreeViewColumn* column, pListView* p) -> void { return p->_doHeaderActivate(column); }
static auto ListView_mouseMoveEvent(GtkWidget*, GdkEvent*, pListView* p) -> signed { return p->_doMouseMove(); }
static auto ListView_popup(GtkTreeView*, pListView* p) -> void { return p->_doContext(); }
static auto ListView_toggle(GtkCellRendererToggle*, const char* path, pListView* p) -> void { return p->_doToggle(path); }

auto pListView::construct() -> void {
  gtkWidget = gtk_scrolled_window_new(0, 0);
  gtkScrolledWindow = GTK_SCROLLED_WINDOW(gtkWidget);
  gtk_scrolled_window_set_policy(gtkScrolledWindow, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(gtkScrolledWindow, GTK_SHADOW_ETCHED_IN);

  gtkWidgetChild = gtk_tree_view_new();
  gtkTreeView = GTK_TREE_VIEW(gtkWidgetChild);
  gtkTreeSelection = gtk_tree_view_get_selection(gtkTreeView);
  gtk_container_add(GTK_CONTAINER(gtkWidget), gtkWidgetChild);
  gtk_tree_view_set_rubber_banding(gtkTreeView, true);

  gtk_widget_show(gtkWidgetChild);

  setBackgroundColor(state().backgroundColor);
  setCheckable(state().checkable);
  setFont(self().font(true));
  setForegroundColor(state().foregroundColor);
  setGridVisible(state().gridVisible);
  setHeaderVisible(state().headerVisible);
  setMultiSelect(state().multiSelect);

  g_signal_connect(G_OBJECT(gtkTreeView), "button-press-event", G_CALLBACK(ListView_buttonEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeView), "button-release-event", G_CALLBACK(ListView_buttonEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeView), "motion-notify-event", G_CALLBACK(ListView_mouseMoveEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeView), "popup-menu", G_CALLBACK(ListView_popup), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeView), "row-activated", G_CALLBACK(ListView_activate), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeSelection), "changed", G_CALLBACK(ListView_change), (gpointer)this);

  pWidget::construct();
}

auto pListView::destruct() -> void {
  gtk_widget_destroy(gtkWidgetChild);
  gtk_widget_destroy(gtkWidget);
}

auto pListView::append(sListViewColumn column) -> void {
  gtk_tree_view_append_column(gtkTreeView, column->self()->gtkColumn);
  gtk_widget_show_all(column->self()->gtkHeader);
  column->setBackgroundColor(column->backgroundColor());
  column->setEditable(column->editable());
  column->setFont(column->font());
  column->setForegroundColor(column->foregroundColor());
  column->setHorizontalAlignment(column->horizontalAlignment());
  column->setVerticalAlignment(column->verticalAlignment());
  setCheckable(state().checkable);
  _createModel();
  gtk_tree_view_set_rules_hint(gtkTreeView, self().columns() >= 2);  //two or more columns + checkbutton column
}

auto pListView::append(sListViewItem item) -> void {
  gtk_list_store_append(gtkListStore, &item->self()->gtkIter);

  item->setChecked(item->checked());
  item->setSelected(item->selected());
  for(auto column : range(self().columns())) {
    item->setIcon(column, item->state.icon(column, {}));
    item->setText(column, item->state.text(column, ""));
  }
}

auto pListView::focused() -> bool {
  return GTK_WIDGET_HAS_FOCUS(gtkTreeView);
}

auto pListView::remove(sListViewColumn column) -> void {
  if(auto delegate = column->self()) {
    gtk_tree_view_remove_column(gtkTreeView, delegate->gtkColumn);
    delegate->gtkColumn = nullptr;
  }
  _createModel();
  gtk_tree_view_set_rules_hint(gtkTreeView, self().columns() >= 2);  //two or more columns + checkbutton column
}

auto pListView::remove(sListViewItem item) -> void {
  lock();
  if(auto delegate = item->self()) {
    gtk_list_store_remove(gtkListStore, &delegate->gtkIter);
    _updateSelected();
  }
  unlock();
}

auto pListView::reset() -> void {
  GList* list = gtk_tree_view_get_columns(gtkTreeView), *p = list;
  while(p && p->data) {
    gtk_tree_view_remove_column(gtkTreeView, (GtkTreeViewColumn*)p->data);
    p = p->next;
  }
  g_list_free(list);
  _createModel();
  gtk_tree_view_set_rules_hint(gtkTreeView, false);
}

//column widths:
//< 0 = expanding (consume all remaining space)
//  0 = auto (resize to contents
//> 0 = fixed width
auto pListView::resizeColumns() -> void {
  lock();

  //compute the minimum width required for each column based upon the contents of all rows
  vector<signed> minimumWidths;
  for(auto column : range(self().columns())) {
    signed maximumWidth = 1;
    if(self().headerVisible()) {
      maximumWidth = max(maximumWidth, 8 //margin
      + state().columns[column]->state.icon.width
      + Font::size(state().columns[column]->font(true), state().columns[column]->state.text).width()
      );
    }
    for(auto row : range(self().items())) {
      maximumWidth = max(maximumWidth, 8  //margin
      + (row == 0 && state().checkable ? 32 : 0)  //check box
      + state().items[row]->state.icon(column, {}).width
      + Font::size(state().columns[column]->font(true), state().items[row]->state.text(column, "")).width()
      );
    }
    if(!state().columns[column]->visible()) maximumWidth = 1;
    minimumWidths.append(maximumWidth);
  }

  //subtract the widths of all non-expanding columns from the available widget space
  signed expansions = 0;  //count the number of expanded columns
  signed emptyWidth = pSizable::state().geometry.width() - 5;  //margin
  for(auto column : range(self().columns())) {
    signed width = state().columns[column]->width();
    if(!state().columns[column]->visible()) width = 1;
    if(width < 0) { expansions++; continue; }
    if(width == 0) width = minimumWidths[column];
    emptyWidth -= width;
  }

  //the vertical scroll bar consumes header space when visible; subtract it from available space if needed
  auto scrollBar = gtk_scrolled_window_get_vscrollbar(gtkScrolledWindow);
  if(scrollBar && gtk_widget_get_visible(scrollBar)) {
    emptyWidth -= scrollBar->allocation.width;
  }

  //divide remaining space among all expanded columns
  if(expansions && emptyWidth >= expansions) emptyWidth /= expansions;
  else emptyWidth = 1;

  for(auto column : range(self().columns())) {
    signed width = state().columns[column]->width();
    if(!state().columns[column]->visible()) width = 1;
    if(width < 0) width = emptyWidth;
    if(width == 0) width = minimumWidths[column];
    gtk_tree_view_column_set_fixed_width(_column(column)->gtkColumn, width);
  }

  unlock();
}

auto pListView::setBackgroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_base(gtkWidgetChild, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pListView::setCheckable(bool checkable) -> void {
  if(auto delegate = _column(0)) {
    gtk_cell_renderer_set_visible(delegate->gtkCellToggle, checkable);
  }
}

auto pListView::setChecked(bool checked) -> void {
  for(auto& item : state().items) {
    if(auto delegate = item->self()) delegate->setChecked(checked);
  }
}

auto pListView::setFocused() -> void {
  gtk_widget_grab_focus(gtkWidgetChild);
}

auto pListView::setFont(const string& font) -> void {
  for(auto& column : state().columns) {
    if(auto delegate = column->self()) delegate->setFont(column->font(true));
  }
}

auto pListView::setForegroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_text(gtkWidgetChild, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pListView::setGridVisible(bool visible) -> void {
  gtk_tree_view_set_grid_lines(gtkTreeView, visible ? GTK_TREE_VIEW_GRID_LINES_BOTH : GTK_TREE_VIEW_GRID_LINES_NONE);
}

auto pListView::setHeaderVisible(bool visible) -> void {
  gtk_tree_view_set_headers_visible(gtkTreeView, visible);
}

auto pListView::setMultiSelect(bool multiSelect) -> void {
  gtk_tree_selection_set_mode(gtkTreeSelection, multiSelect ? GTK_SELECTION_MULTIPLE : GTK_SELECTION_SINGLE);
}

auto pListView::setSelected(bool selected) -> void {
  for(auto& item : state().items) {
    if(auto delegate = item->self()) delegate->setSelected(selected);
  }
}

auto pListView::_column(unsigned column) -> pListViewColumn* {
  if(auto delegate = self().column(column)) return delegate->self();
  return nullptr;
}

auto pListView::_createModel() -> void {
  gtk_tree_view_set_model(gtkTreeView, nullptr);
  gtkListStore = nullptr;
  gtkTreeModel = nullptr;

  vector<GType> types;
  unsigned position = 0;
  for(auto column : state().columns) {
    if(!column->self()->gtkColumn) continue;  //column is being removed
    if(position++ == 0) types.append(G_TYPE_BOOLEAN);
    types.append(GDK_TYPE_PIXBUF);
    types.append(G_TYPE_STRING);
  }
  if(!types) return;  //no columns available

  gtkListStore = gtk_list_store_newv(types.size(), types.data());
  gtkTreeModel = GTK_TREE_MODEL(gtkListStore);
  gtk_tree_view_set_model(gtkTreeView, gtkTreeModel);
}

auto pListView::_doActivate() -> void {
  if(!locked()) self().doActivate();
}

auto pListView::_doChange() -> void {
  if(!locked()) _updateSelected();
}

auto pListView::_doContext() -> void {
  if(!locked()) self().doContext();
}

auto pListView::_doEdit(GtkCellRendererText* gtkCellRendererText, const char* path, const char* text) -> void {
  for(auto& column : state().columns) {
    if(auto delegate = column->self()) {
      if(gtkCellRendererText = GTK_CELL_RENDERER_TEXT(delegate->gtkCellText)) {
        if(auto item = self().item(decimal(path))) {
          if(string{text} != item->text(column->offset())) {
            item->setText(column->offset(), text);
            if(!locked()) self().doEdit(item, column);
          }
          return;
        }
      }
    }
  }
}

auto pListView::_doEvent(GdkEventButton* event) -> signed {
  GtkTreePath* path = nullptr;
  gtk_tree_view_get_path_at_pos(gtkTreeView, event->x, event->y, &path, nullptr, nullptr, nullptr);

  if(event->type == GDK_BUTTON_PRESS) {
    //when clicking in empty space below the last list view item; GTK+ does not deselect all items;
    //below code enables this functionality, to match behavior with all other UI toolkits (and because it's very convenient to have)
    if(path == nullptr && gtk_tree_selection_count_selected_rows(gtkTreeSelection) > 0) {
      self().setSelected({});
      self().doChange();
      return true;
    }
  }

  if(event->type == GDK_BUTTON_PRESS && event->button == 3) {
    //this check prevents the loss of selection on other items if the item under the mouse cursor is currently selected
    if(path && gtk_tree_selection_path_is_selected(gtkTreeSelection, path)) return true;
  }

  if(event->type == GDK_BUTTON_RELEASE && event->button == 3) {
    //handle action during right-click release; as button-press-event is sent prior to selection update
    //without this, the callback handler would see the previous selection state instead
    self().doContext();
    return false;
  }

  return false;
}

auto pListView::_doHeaderActivate(GtkTreeViewColumn* gtkTreeViewColumn) -> void {
  for(auto& column : state().columns) {
    if(auto delegate = column->self()) {
      if(gtkTreeViewColumn == delegate->gtkColumn) {
        if(!locked()) self().doSort(column);
        return;
      }
    }
  }
}

//GtkTreeView::cursor-changed and GtkTreeSelection::changed do not send signals for changes during rubber-banding selection
//so here we capture motion-notify-event, and if the selections have changed, invoke ListView::onChange
auto pListView::_doMouseMove() -> signed {
  if(gtk_tree_view_is_rubber_banding_active(gtkTreeView)) {
    if(!locked()) _updateSelected();
  }
  return false;
}

auto pListView::_doToggle(const char* path) -> void {
  if(auto item = self().item(decimal(path))) {
    if(auto delegate = item->self()) {
      item->state.checked = !item->state.checked;
      delegate->setChecked(item->state.checked);
      if(!locked()) self().doToggle(item);
    }
  }
}

//compare currently selected items to previously selected items
//if different, invoke the onChange callback unless locked, and cache current selection
//this prevents firing an onChange event when the actual selection has not changed
//this is particularly important for the motion-notify-event binding
auto pListView::_updateSelected() -> void {
  vector<unsigned> selected;

  GList* list = gtk_tree_selection_get_selected_rows(gtkTreeSelection, &gtkTreeModel);
  GList* p = list;

  while(p) {
    GtkTreeIter iter;
    if(gtk_tree_model_get_iter(gtkTreeModel, &iter, (GtkTreePath*)p->data)) {
      char* pathname = gtk_tree_model_get_string_from_iter(gtkTreeModel, &iter);
      unsigned selection = decimal(pathname);
      g_free(pathname);
      selected.append(selection);
    }
    p = p->next;
  }

  g_list_foreach(list, (GFunc)gtk_tree_path_free, nullptr);
  g_list_free(list);

  bool identical = selected.size() == currentSelection.size();
  if(identical) {
    for(auto n : range(selected)) {
      if(selected[n] != currentSelection[n]) {
        identical = false;
        break;
      }
    }
  }
  if(identical) return;

  currentSelection = selected;
  for(auto& item : state().items) item->state.selected = false;
  for(auto& position : currentSelection) {
    if(position >= self().items()) continue;
    self().item(position)->state.selected = true;
  }

  if(!locked()) self().doChange();
}

}
