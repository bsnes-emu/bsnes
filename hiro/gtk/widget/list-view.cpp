#if defined(Hiro_ListView)

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
  setBatchable(state().batchable);
  setCheckable(state().checkable);
  setFont(self().font(true));
  setForegroundColor(state().foregroundColor);
  setGridVisible(state().gridVisible);
  setHeaderVisible(state().headerVisible);
  setSortable(state().sortable);

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
  column->setResizable(column->resizable());
  column->setVerticalAlignment(column->verticalAlignment());
  setCheckable(state().checkable);
  setSortable(state().sortable);
  _createModel();
  resizeColumns();
  gtk_tree_view_set_rules_hint(gtkTreeView, self().columns() >= 2);  //two or more columns + checkbutton column
}

auto pListView::append(sListViewItem item) -> void {
  gtk_list_store_append(gtkListStore, &item->self()->gtkIter);

  item->setChecked(item->checked());
  item->setSelected(item->selected());
  for(auto column : range(self().columns())) {
    if(auto cell = item->cell(column)) {
      if(auto self = cell->self()) {
        self->setIcon(cell->state.icon);
        self->setText(cell->state.text);
      }
    }
  }
}

auto pListView::checkAll() -> void {
  for(auto& item : state().items) {
    if(auto delegate = item->self()) delegate->setChecked(true);
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

auto pListView::resizeColumns() -> void {
  lock();

  vector<signed> widths;
  signed minimumWidth = 0;
  signed expandable = 0;
  for(auto column : range(state().columns)) {
    signed width = _width(column);
    widths.append(width);
    minimumWidth += width;
    if(state().columns[column]->expandable()) expandable++;
  }

  signed maximumWidth = self().geometry().width() - 6;
  if(auto scrollBar = gtk_scrolled_window_get_vscrollbar(gtkScrolledWindow)) {
    if(gtk_widget_get_visible(scrollBar)) maximumWidth -= scrollBar->allocation.width;
  }

  signed expandWidth = 0;
  if(expandable && maximumWidth > minimumWidth) {
    expandWidth = (maximumWidth - minimumWidth) / expandable;
  }

  for(auto column : range(state().columns)) {
    if(auto self = state().columns[column]->self()) {
      signed width = widths[column];
      if(self->state().expandable) width += expandWidth;
      gtk_tree_view_column_set_fixed_width(self->gtkColumn, width);
    }
  }

  unlock();
}

auto pListView::selectAll() -> void {
  for(auto& item : state().items) {
    if(auto delegate = item->self()) delegate->setSelected(true);
  }
}

auto pListView::setBackgroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_base(gtkWidgetChild, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pListView::setBatchable(bool batchable) -> void {
  gtk_tree_selection_set_mode(gtkTreeSelection, batchable ? GTK_SELECTION_MULTIPLE : GTK_SELECTION_SINGLE);
}

auto pListView::setCheckable(bool checkable) -> void {
  if(auto delegate = _column(0)) {
    gtk_cell_renderer_set_visible(delegate->gtkCellToggle, checkable);
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

auto pListView::setSortable(bool sortable) -> void {
  for(auto& column : state().columns) {
    if(auto delegate = column->self()) {
      gtk_tree_view_column_set_clickable(delegate->gtkColumn, sortable);
    }
  }
}

auto pListView::uncheckAll() -> void {
  for(auto& item : state().items) {
    if(auto delegate = item->self()) delegate->setChecked(false);
  }
}

auto pListView::unselectAll() -> void {
  for(auto& item : state().items) {
    if(auto delegate = item->self()) delegate->setSelected(false);
  }
}

auto pListView::_cellWidth(unsigned _row, unsigned _column) -> unsigned {
  unsigned width = 8;  //margin
  if(state().checkable && _column == 0) width += 32;  //checkbox
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(auto& icon = cell->state.icon) {
        width += icon.width + 2;
      }
      if(auto& text = cell->state.text) {
        width += Font::size(cell->font(true), text).width();
      }
    }
  }
  return width;
}

auto pListView::_column(unsigned column) -> pListViewColumn* {
  if(auto delegate = self().column(column)) return delegate->self();
  return nullptr;
}

auto pListView::_columnWidth(unsigned _column) -> unsigned {
  unsigned width = 8;  //margin
  if(auto column = self().column(_column)) {
    if(auto& icon = column->state.icon) {
      width += icon.width + 2;
    }
    if(auto& text = column->state.text) {
      width += Font::size(column->font(true), text).width();
    }
  }
  return width;
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
      if(gtkCellRendererText == GTK_CELL_RENDERER_TEXT(delegate->gtkCellText)) {
        auto row = decimal(path);
        if(auto item = self().item(row)) {
          if(auto cell = item->cell(column->offset())) {
            if(string{text} != cell->state.text) {
              cell->setText(text);
              if(!locked()) self().doEdit(cell);
            }
            return;
          }
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
      self().unselectAll();
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

auto pListView::_width(unsigned column) -> unsigned {
  if(auto width = state().columns[column]->width()) return width;
  unsigned width = 1;
  if(!state().columns[column]->visible()) return width;
  if(state().headerVisible) {
    width = max(width, _columnWidth(column));
  }
  for(auto row : range(state().items)) {
    width = max(width, _cellWidth(row, column));
  }
  return width;
}

}

#endif
