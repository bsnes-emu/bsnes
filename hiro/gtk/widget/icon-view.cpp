#if defined(Hiro_IconView)

namespace hiro {

static auto IconView_activate(GtkIconView* iconView, GtkTreePath* path, pIconView* p) -> void {
  if(!p->locked()) p->self().doActivate();
}

static auto IconView_buttonEvent(GtkTreeView* treeView, GdkEventButton* event, pIconView* p) -> signed {
  if(event->type == GDK_BUTTON_RELEASE && event->button == 3) {
    if(!p->locked()) p->self().doContext();
    return false;
  }

  return false;
}

static auto IconView_change(GtkIconView* iconView, pIconView* p) -> void {
  p->_updateSelected();
}

auto pIconView::construct() -> void {
  gtkWidget = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkWidget), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(gtkWidget), GTK_SHADOW_ETCHED_IN);

  store = gtk_list_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
  subWidget = gtk_icon_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(subWidget), 0);
  gtk_icon_view_set_text_column(GTK_ICON_VIEW(subWidget), 1);
  gtk_icon_view_set_reorderable(GTK_ICON_VIEW(subWidget), false);
  gtk_icon_view_set_margin(GTK_ICON_VIEW(subWidget), 0);
  gtk_icon_view_set_spacing(GTK_ICON_VIEW(subWidget), 0);
  gtk_icon_view_set_column_spacing(GTK_ICON_VIEW(subWidget), 0);
  gtk_icon_view_set_row_spacing(GTK_ICON_VIEW(subWidget), 0);
  gtk_icon_view_set_item_padding(GTK_ICON_VIEW(subWidget), 0);
  gtk_container_add(GTK_CONTAINER(gtkWidget), subWidget);

  gtk_widget_show(subWidget);

  setBackgroundColor(state().backgroundColor);
  setBatchable(state().batchable);
  setFlow(state().flow);
  setForegroundColor(state().foregroundColor);
  setOrientation(state().orientation);
  for(auto position : range(self().items())) {
    auto& item = state().items[position];
    append(item);
  }
  _updateSelected();

  g_signal_connect(G_OBJECT(subWidget), "button-press-event", G_CALLBACK(IconView_buttonEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(subWidget), "button-release-event", G_CALLBACK(IconView_buttonEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(subWidget), "item-activated", G_CALLBACK(IconView_activate), (gpointer)this);
  g_signal_connect(G_OBJECT(subWidget), "selection-changed", G_CALLBACK(IconView_change), (gpointer)this);

  pWidget::construct();
}

auto pIconView::destruct() -> void {
  gtk_widget_destroy(subWidget);
  gtk_widget_destroy(gtkWidget);
}

auto pIconView::append(sIconViewItem item) -> void {
  GtkTreeIter iter;
  gtk_list_store_append(store, &iter);
  setItemIcon(item->offset(), item->state.icon);
  setItemSelected(item->offset(), item->state.selected);
  setItemText(item->offset(), item->state.text);
}

auto pIconView::remove(sIconViewItem item) -> void {
  lock();
  GtkTreeIter iter;
  if(gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(store), &iter, string{item->offset()})) {
    gtk_list_store_remove(store, &iter);
  }
  _updateSelected();
  unlock();
}

auto pIconView::reset() -> void {
  lock();
  gtk_list_store_clear(store);
  _updateSelected();
  unlock();
}

auto pIconView::setBackgroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_base(subWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pIconView::setBatchable(bool batchable) -> void {
  gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(subWidget),
    batchable ? GTK_SELECTION_MULTIPLE : GTK_SELECTION_SINGLE
  );
}

auto pIconView::setFlow(Orientation flow) -> void {
  //GTK+ does not support vertical flow ... the closest we can get is a horizontal flow with only one column
  if(flow == Orientation::Horizontal) {
    gtk_icon_view_set_columns(GTK_ICON_VIEW(subWidget), -1);
    gtk_icon_view_set_item_width(GTK_ICON_VIEW(subWidget), -1);
  } else {
    gtk_icon_view_set_columns(GTK_ICON_VIEW(subWidget), 1);
    gtk_icon_view_set_item_width(GTK_ICON_VIEW(subWidget), max(128, pSizable::state().geometry.width() - 64));
  }
}

auto pIconView::setForegroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_text(subWidget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
}

auto pIconView::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  if(state().flow == Orientation::Vertical) {
    gtk_icon_view_set_item_width(GTK_ICON_VIEW(subWidget), max(128, pSizable::state().geometry.width() - 64));
  }
}

auto pIconView::setItemIcon(unsigned position, const image& icon) -> void {
  if(position >= self().itemCount()) return;
  GtkTreeIter iter;
  if(gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(store), &iter, string{position})) {
    if(icon) {
      GdkPixbuf* pixbuf = CreatePixbuf(icon);
      gtk_list_store_set(store, &iter, 0, pixbuf, -1);
    } else {
      gtk_list_store_set(store, &iter, 0, nullptr, -1);
    }
  }
}

auto pIconView::setItemSelected(unsigned position, bool selected) -> void {
  if(position >= self().itemCount()) return;
  lock();
  GtkTreePath* path = gtk_tree_path_new_from_string(string{position});
  if(selected) {
    gtk_icon_view_select_path(GTK_ICON_VIEW(subWidget), path);
  } else {
    gtk_icon_view_unselect_path(GTK_ICON_VIEW(subWidget), path);
  }
  gtk_tree_path_free(path);
  _updateSelected();
  unlock();
}

auto pIconView::setItemSelected(const vector<signed>& selections) -> void {
  lock();
  setItemSelectedNone();
  for(auto& position : selections) setItemSelected(position, true);
  _updateSelected();
  unlock();
}

auto pIconView::setItemSelectedAll() -> void {
  lock();
  gtk_icon_view_select_all(GTK_ICON_VIEW(subWidget));
  _updateSelected();
  unlock();
}

auto pIconView::setItemSelectedNone() -> void {
  lock();
  gtk_icon_view_unselect_all(GTK_ICON_VIEW(subWidget));
  _updateSelected();
  unlock();
}

auto pIconView::setItemText(unsigned position, const string& text) -> void {
  if(position >= self().itemCount()) return;
  GtkTreeIter iter;
  if(gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(store), &iter, string{position})) {
    gtk_list_store_set(store, &iter, 1, (const char*)text, -1);
  }
}

auto pIconView::setOrientation(Orientation orientation) -> void {
  gtk_icon_view_set_item_orientation(GTK_ICON_VIEW(subWidget),
    orientation == Orientation::Horizontal ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL
  );
}

auto pIconView::_updateSelected() -> void {
  vector<unsigned> selected;

  GList* list = gtk_icon_view_get_selected_items(GTK_ICON_VIEW(subWidget));
  GList* p = list;

  while(p) {
    auto path = (GtkTreePath*)p->data;
    char* pathString = gtk_tree_path_to_string(path);
    unsigned position = toNatural(pathString);
    g_free(pathString);
    selected.append(position);
    p = p->next;
  }

  g_list_foreach(list, (GFunc)gtk_tree_path_free, nullptr);
  g_list_free(list);

  bool identical = selected.size() == currentSelection.size();
  if(identical) {
    for(unsigned n = 0; n < selected.size(); n++) {
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
    state().items[position]->state.selected = true;
  }

  if(!locked()) self().doChange();
}

}

#endif
