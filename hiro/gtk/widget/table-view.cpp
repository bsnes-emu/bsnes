#if defined(Hiro_TableView)

namespace hiro {

static auto TableView_activate(GtkTreeView*, GtkTreePath* gtkRow, GtkTreeViewColumn* gtkColumn, pTableView* p) -> void { return p->_doActivate(gtkRow, gtkColumn); }
static auto TableView_buttonEvent(GtkTreeView* treeView, GdkEventButton* event, pTableView* p) -> signed { return p->_doEvent(event); }
static auto TableView_change(GtkTreeSelection*, pTableView* p) -> void { return p->_doChange(); }
static auto TableView_edit(GtkCellRendererText* renderer, const char* path, const char* text, pTableView* p) -> void { return p->_doEdit(renderer, path, text); }
static auto TableView_headerActivate(GtkTreeViewColumn* column, pTableView* p) -> void { return p->_doHeaderActivate(column); }
static auto TableView_keyPressEvent(GtkTreeView* treeView, GdkEventKey* event, pTableView* p) -> bool { return p->_doKeyPress(event); }
static auto TableView_mouseMoveEvent(GtkWidget*, GdkEvent*, pTableView* p) -> signed { return p->_doMouseMove(); }
static auto TableView_popup(GtkTreeView*, pTableView* p) -> void { return p->_doContext(); }

static auto TableView_dataFunc(GtkTreeViewColumn* column, GtkCellRenderer* renderer, GtkTreeModel* model, GtkTreeIter* iter, pTableView* p) -> void { return p->_doDataFunc(column, renderer, iter); }
static auto TableView_toggle(GtkCellRendererToggle* toggle, const char* path, pTableView* p) -> void { return p->_doToggle(toggle, path); }

static auto TableView_realize(GtkTreeView*, pTableView* p) -> void {
  //the initial geometry for column sizing is most likely wrong at this point:
  //having to call processEvents() twice is very heavy-handed, but necessary here.
  for(uint repeat : range(2)) {
    Application::processEvents();
    p->resizeColumns();
  }
}

auto pTableView::construct() -> void {
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
  setBordered(state().bordered);
  setFont(self().font(true));
  setForegroundColor(state().foregroundColor);
  setHeadered(state().headered);
  setSortable(state().sortable);

  g_signal_connect(G_OBJECT(gtkTreeView), "button-press-event", G_CALLBACK(TableView_buttonEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeView), "button-release-event", G_CALLBACK(TableView_buttonEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeView), "key-press-event", G_CALLBACK(TableView_keyPressEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeView), "motion-notify-event", G_CALLBACK(TableView_mouseMoveEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeView), "popup-menu", G_CALLBACK(TableView_popup), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeView), "realize", G_CALLBACK(TableView_realize), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeView), "row-activated", G_CALLBACK(TableView_activate), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkTreeSelection), "changed", G_CALLBACK(TableView_change), (gpointer)this);

  //searching doesn't currently work anyway ...
  gtkEntry = (GtkEntry*)gtk_entry_new();
  gtk_tree_view_set_search_entry(gtkTreeView, gtkEntry);

  pWidget::construct();
}

auto pTableView::destruct() -> void {
  gtk_widget_destroy(GTK_WIDGET(gtkEntry));
  gtk_widget_destroy(gtkWidgetChild);
  gtk_widget_destroy(gtkWidget);
}

auto pTableView::append(sTableViewColumn column) -> void {
  _updateRulesHint();
}

auto pTableView::append(sTableViewItem item) -> void {
}

auto pTableView::focused() const -> bool {
  return gtk_widget_has_focus(GTK_WIDGET(gtkTreeView));
}

auto pTableView::remove(sTableViewColumn column) -> void {
  _updateRulesHint();
}

auto pTableView::remove(sTableViewItem item) -> void {
}

auto pTableView::resizeColumns() -> void {
  auto lock = acquire();

  vector<signed> widths;
  signed minimumWidth = 0;
  signed expandable = 0;
  for(uint position : range(self().columnCount())) {
    signed width = _width(position);
    widths.append(width);
    minimumWidth += width;
    if(self().column(position).expandable()) expandable++;
  }

  signed maximumWidth = self().geometry().width() - 6;
  if(auto scrollBar = gtk_scrolled_window_get_vscrollbar(gtkScrolledWindow)) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(scrollBar, &allocation);
    if(gtk_widget_get_visible(scrollBar)) maximumWidth -= allocation.width;
  }

  signed expandWidth = 0;
  if(expandable && maximumWidth > minimumWidth) {
    expandWidth = (maximumWidth - minimumWidth) / expandable;
  }

  for(uint position : range(self().columnCount())) {
    if(auto column = self().column(position)->self()) {
      signed width = widths[position];
      if(column->state().expandable) width += expandWidth;
      gtk_tree_view_column_set_fixed_width(column->gtkColumn, width);
    }
  }
}

auto pTableView::setAlignment(Alignment alignment) -> void {
  _updateRulesHint();
}

auto pTableView::setBackgroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_base(gtkWidgetChild, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
  _updateRulesHint();
}

auto pTableView::setBatchable(bool batchable) -> void {
  gtk_tree_selection_set_mode(gtkTreeSelection, batchable ? GTK_SELECTION_MULTIPLE : GTK_SELECTION_SINGLE);
}

auto pTableView::setBordered(bool bordered) -> void {
  gtk_tree_view_set_grid_lines(gtkTreeView, bordered ? GTK_TREE_VIEW_GRID_LINES_BOTH : GTK_TREE_VIEW_GRID_LINES_NONE);
}

auto pTableView::setFocused() -> void {
  lock();
  //gtk_widget_grab_focus() will select the first item if nothing is currently selected
  //this behavior is undesirable. detect selection state first, and restore if required
  if(!state().batchable) {
    //gtk_tree_selection_get_selected() will throw a critical exception in batchable mode
    bool selected = gtk_tree_selection_get_selected(gtkTreeSelection, nullptr, nullptr);
    gtk_widget_grab_focus(gtkWidgetChild);
    if(!selected) gtk_tree_selection_unselect_all(gtkTreeSelection);
  } else {
    gtk_widget_grab_focus(gtkWidgetChild);
  }
  unlock();
}

auto pTableView::setFont(const Font& font) -> void {
}

auto pTableView::setForegroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_text(gtkWidgetChild, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
  _updateRulesHint();
}

auto pTableView::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  for(auto& column : state().columns) {
    if(column->expandable()) {
      Application::processEvents();
      return resizeColumns();
    }
  }
}

auto pTableView::setHeadered(bool headered) -> void {
  gtk_tree_view_set_headers_visible(gtkTreeView, headered);
}

auto pTableView::setSortable(bool sortable) -> void {
  for(auto& column : state().columns) {
    if(auto self = column->self()) gtk_tree_view_column_set_clickable(self->gtkColumn, sortable);
  }
}

auto pTableView::_cellWidth(unsigned _row, unsigned _column) -> unsigned {
  unsigned width = 8;
  if(auto item = self().item(_row)) {
    if(auto cell = item->cell(_column)) {
      if(cell->state.checkable) {
        width += 16 + (cell->state.icon || cell->state.text ? 4 : 0);
      }
      if(auto& icon = cell->state.icon) {
        width += icon.width() + 2;
      }
      if(auto& text = cell->state.text) {
        width += pFont::size(cell->font(true), text).width();
      }
    }
  }
  return width;
}

auto pTableView::_columnWidth(unsigned _column) -> unsigned {
  unsigned width = 8;
  if(auto column = self().column(_column)) {
    if(auto& icon = column->state.icon) {
      width += icon.width() + 2;
    }
    if(auto& text = column->state.text) {
      width += pFont::size(column->font(true), text).width();
    }
    if(column->state.sorting != Sort::None) {
      width += 20;
    }
  }
  return width;
}

auto pTableView::_createModel() -> void {
  auto lock = acquire();

  gtk_tree_view_set_model(gtkTreeView, nullptr);
  gtkListStore = nullptr;
  gtkTreeModel = nullptr;

  vector<GType> types;
  for(auto& column : state().columns) {
    if(auto self = column->self()) {
      if(!self->gtkColumn) continue;  //may not have been created yet; or recently destroyed
      types.append(G_TYPE_BOOLEAN);
      types.append(GDK_TYPE_PIXBUF);
      types.append(G_TYPE_STRING);
    }
  }
  if(!types) return;  //no columns available

  gtkListStore = gtk_list_store_newv(types.size(), types.data());
  gtkTreeModel = GTK_TREE_MODEL(gtkListStore);
  gtk_tree_view_set_model(gtkTreeView, gtkTreeModel);
}

auto pTableView::_doActivate(GtkTreePath* gtkRow, GtkTreeViewColumn* gtkColumn) -> void {
  if(locked()) return;

  if(gtkRow && gtkColumn) {
    auto path = gtk_tree_path_to_string(gtkRow);
    auto item = self().item(toNatural(path));
    auto cell = item.cell(0);
    for(auto& column : state().columns) {
      if(auto self = column->self()) {
        if(self->gtkColumn == gtkColumn) {
          cell = item.cell(column->offset());
          break;
        }
      }
    }
    g_free(path);
    self().doActivate(cell);
  } else {
    self().doActivate({});
  }
}

auto pTableView::_doChange() -> void {
  if(!locked()) _updateSelected();
}

auto pTableView::_doContext() -> void {
  if(!locked()) self().doContext();
}

auto pTableView::_doDataFunc(GtkTreeViewColumn* gtkColumn, GtkCellRenderer* renderer, GtkTreeIter* iter) -> void {
  auto path = gtk_tree_model_get_string_from_iter(gtkTreeModel, iter);
  auto row = toNatural(path);
  g_free(path);

  for(auto& column : state().columns) {
    if(auto p = column->self()) {
      if(renderer != GTK_CELL_RENDERER(p->gtkCellToggle)
      && renderer != GTK_CELL_RENDERER(p->gtkCellIcon)
      && renderer != GTK_CELL_RENDERER(p->gtkCellText)
      ) continue;
      if(auto item = self().item(row)) {
        if(auto cell = item->cell(column->offset())) {
          if(renderer == GTK_CELL_RENDERER(p->gtkCellToggle)) {
            gtk_cell_renderer_set_visible(renderer, cell->state.checkable);
          } else if(renderer == GTK_CELL_RENDERER(p->gtkCellText)) {
            auto alignment = cell->alignment(true);
            if(!alignment) alignment = {0.0, 0.5};
          //note: below line will center column header text; but causes strange glitches
          //(specifically, windows fail to respond to the close button ... some kind of heap corruption inside GTK+)
          //gtk_tree_view_column_set_alignment(gtkColumn, alignment.horizontal());
            gtk_cell_renderer_set_alignment(renderer, alignment.horizontal(), alignment.vertical());
            auto pangoAlignment = PANGO_ALIGN_CENTER;
            if(alignment.horizontal() < 0.333) pangoAlignment = PANGO_ALIGN_LEFT;
            if(alignment.horizontal() > 0.666) pangoAlignment = PANGO_ALIGN_RIGHT;
            g_object_set(G_OBJECT(renderer), "alignment", pangoAlignment, nullptr);
            auto font = pFont::create(cell->font(true));
            g_object_set(G_OBJECT(renderer), "font-desc", font, nullptr);
            pango_font_description_free(font);
            if(auto color = cell->foregroundColor(true)) {
              auto gdkColor = CreateColor(color);
              if(settings.theme.widgetColors) g_object_set(G_OBJECT(renderer), "foreground-gdk", &gdkColor, nullptr);
            } else {
              g_object_set(G_OBJECT(renderer), "foreground-set", false, nullptr);
            }
          }
          if(auto color = cell->backgroundColor(true)) {
            auto gdkColor = CreateColor(color);
            if(settings.theme.widgetColors) g_object_set(G_OBJECT(renderer), "cell-background-gdk", &gdkColor, nullptr);
          } else {
            g_object_set(G_OBJECT(renderer), "cell-background-set", false, nullptr);
          }
        }
      }
    }
  }
}

auto pTableView::_doEdit(GtkCellRendererText* gtkCellRendererText, const char* path, const char* text) -> void {
  for(auto& column : state().columns) {
    if(auto delegate = column->self()) {
      if(gtkCellRendererText == GTK_CELL_RENDERER_TEXT(delegate->gtkCellText)) {
        auto row = toNatural(path);
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

auto pTableView::_doEvent(GdkEventButton* gdkEvent) -> signed {
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
        for(auto& item : state().items) item->setSelected(false);
        self().doChange();
        return true;
      }
    }

    if(gdkEvent->button == 3) {
      //multi-selection mode:
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

auto pTableView::_doHeaderActivate(GtkTreeViewColumn* gtkTreeViewColumn) -> void {
  for(auto& column : state().columns) {
    if(auto delegate = column->self()) {
      if(gtkTreeViewColumn == delegate->gtkColumn) {
        if(!locked()) self().doSort(column);
        return;
      }
    }
  }
}

auto pTableView::_doKeyPress(GdkEventKey* event) -> bool {
  //TODO: GCC 8.2.0 on FreeBSD 12.0 (amd64) seems to miscompile this function periodically to always return true.
  return false;

  if(state().batchable && event->type == GDK_KEY_PRESS) {
    //when using keyboard to activate tree view items in GTK_SELECTION_MULTIPLE mode, GTK will deselect all but the last item
    //this code detects said case, blocks the key from being propagated, and calls the activate callback directly
    //the result is that the enter key can be used to activate multiple selected items at a time
    //there are four ways to activate items via the keyboard in GTK, so we have to detect all of them here
    uint modifiers = event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_SUPER_MASK);  //ignore other modifiers (eg mouse buttons)
    if((event->keyval == GDK_KEY_Return && !modifiers)
    || (event->keyval == GDK_KEY_KP_Enter && !modifiers)
    || (event->keyval == GDK_KEY_space && !modifiers)
    || (event->keyval == GDK_KEY_space && modifiers == GDK_SHIFT_MASK)
    ) {
      _doActivate();
      return true;  //block GTK from handling this keypress
    }
  }
  return false;  //allow GTK to handle this keypress
}

//GtkTreeView::cursor-changed and GtkTreeSelection::changed do not send signals for changes during rubber-banding selection
//so here we capture motion-notify-event, and if the selections have changed, invoke TableView::onChange
auto pTableView::_doMouseMove() -> signed {
  if(gtk_tree_view_is_rubber_banding_active(gtkTreeView)) {
    if(!locked()) _updateSelected();
  }
  return false;
}

auto pTableView::_doToggle(GtkCellRendererToggle* gtkCellRendererToggle, const char* path) -> void {
  for(auto& column : state().columns) {
    if(auto delegate = column->self()) {
      if(gtkCellRendererToggle == GTK_CELL_RENDERER_TOGGLE(delegate->gtkCellToggle)) {
        auto row = toNatural(path);
        if(auto item = self().item(row)) {
          if(auto cell = item->cell(column->offset())) {
            cell->setChecked(!cell->checked());
            if(!locked()) self().doToggle(cell);
            return;
          }
        }
      }
    }
  }
}

//the rules hint draws each row with alternating background colors
//this isn't currently exposed as a hiro API call, so try and determine if we should apply it here
//basically, if there's two or more columns and no custom colors applied, then we do so
auto pTableView::_updateRulesHint() -> void {
  bool rules = false;  //true
  if(state().backgroundColor) rules = false;
  if(state().foregroundColor) rules = false;
  if(state().columns.size() <= 1) rules = false;
  gtk_tree_view_set_rules_hint(gtkTreeView, rules);
}

//compare currently selected items to previously selected items
//if different, invoke the onChange callback unless locked, and cache current selection
//this prevents firing an onChange event when the actual selection has not changed
//this is particularly important for the motion-notify-event binding
auto pTableView::_updateSelected() -> void {
  if(suppressChange) {
    suppressChange = false;
    gtk_tree_selection_unselect_all(gtkTreeSelection);
    return;
  }

  vector<unsigned> selected;

  GList* list = gtk_tree_selection_get_selected_rows(gtkTreeSelection, &gtkTreeModel);
  GList* p = list;

  while(p) {
    GtkTreeIter iter;
    if(gtk_tree_model_get_iter(gtkTreeModel, &iter, (GtkTreePath*)p->data)) {
      char* pathname = gtk_tree_model_get_string_from_iter(gtkTreeModel, &iter);
      unsigned selection = toNatural(pathname);
      g_free(pathname);
      selected.append(selection);
    }
    p = p->next;
  }

  g_list_foreach(list, (GFunc)gtk_tree_path_free, nullptr);
  g_list_free(list);

  bool identical = selected.size() == currentSelection.size();
  if(identical) {
    for(auto n : range(selected.size())) {
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
    if(position >= self().itemCount()) continue;
    self().item(position)->state.selected = true;
  }

  if(!locked()) self().doChange();
}

auto pTableView::_width(unsigned column) -> unsigned {
  if(auto width = self().column(column).width()) return width;
  unsigned width = 1;
  if(!self().column(column).visible()) return width;
  if(self().headered()) width = max(width, _columnWidth(column));
  for(auto row : range(self().itemCount())) {
    width = max(width, _cellWidth(row, column));
  }
  return width;
}

}

#endif
