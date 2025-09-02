#if defined(Hiro_TabFrame)

namespace hiro {

static auto TabFrame_change(GtkNotebook* notebook, GtkWidget* page, unsigned position, pTabFrame* p) -> void {
  for(auto& item : p->state().items) item->state.selected = false;
  if(auto item = p->self().item(position)) item->state.selected = true;

  p->_synchronizeLayout();
  if(!p->locked()) p->self().doChange();
}

static auto TabFrame_close(GtkButton* button, pTabFrame* p) -> void {
  maybe<unsigned> position;
  for(auto n : range(p->tabs.size())) {
    if(button == (GtkButton*)p->tabs[n].close) {
      position = n;
      break;
    }
  }
  if(position) {
    if(!p->locked()) p->self().doClose(p->self().item(*position));
  }
}

static auto TabFrame_move(GtkNotebook* notebook, GtkWidget* page, unsigned moveTo, pTabFrame* p) -> void {
  unsigned position = gtk_notebook_get_current_page(notebook);
  for(auto& item : p->state().items) item->state.selected = false;
  if(auto item = p->self().item(position)) item->state.selected = true;

  maybe<unsigned> moveFrom;
  for(auto n : range(p->tabs.size())) {
    if(page == p->tabs[n].child) {
      moveFrom = n;
      break;
    }
  }
  if(moveFrom) {
    p->state().items.insert(moveTo, p->state().items.take(*moveFrom));
    p->tabs.insert(moveTo, p->tabs.take(*moveFrom));
    if(!p->locked()) p->self().doMove(p->self().item(*moveFrom), p->self().item(moveTo));
  }
}

auto pTabFrame::construct() -> void {
  gtkWidget = gtk_notebook_new();
  gtk_notebook_set_show_border(GTK_NOTEBOOK(gtkWidget), false);
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(gtkWidget), GTK_POS_TOP);

  tabs.reset();  //todo: memory leak, need to release each tab
  for(auto& item : state().items) append(item);
  setNavigation(state().navigation);

  g_signal_connect(G_OBJECT(gtkWidget), "page-reordered", G_CALLBACK(TabFrame_move), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "switch-page", G_CALLBACK(TabFrame_change), (gpointer)this);

  pWidget::construct();
}

auto pTabFrame::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pTabFrame::append(sTabFrameItem item) -> void {
  lock();
  setFont(self().font(true));
  setItemMovable(item->offset(), item->movable());
  if(item->selected()) setItemSelected(item->offset());
  _synchronizeTab(tabs.size() - 1);
  setGeometry(self().geometry());
  unlock();
}

auto pTabFrame::container(mWidget& widget) -> GtkWidget* {
  //TabFrame holds multiple TabFrameItem controls
  //each TabFrameItem has its own GtkWindow; plus its own layout
  //we need to recurse up from the widget to its topmost layout before the TabFrameItem
  //once we know the topmost layout, we search through all TabFrameItems for a match
  mObject* object = &widget;
  while(object) {
    if(object->parentTabFrameItem()) break;
    if(auto parent = object->parent()) { object = parent; continue; }
    break;
  }

  uint position = 0;
  for(auto& item : state().items) {
    if(item->state.sizable.data() == object) return tabs[position].child;
    position++;
  }

  return nullptr;
}

auto pTabFrame::remove(sTabFrameItem item) -> void {
  lock();
  //if we are removing the current tab, we have to select another tab manually
  if(item->offset() == gtk_notebook_get_current_page(GTK_NOTEBOOK(gtkWidget))) {
    //the new tab will be the one after this one
    unsigned displacement = 1;
    //... unless it's the last tab, in which case it's the one before it
    if(item->offset() == self().itemCount() - 1) displacement = -1;
    //... unless there are no tabs left, in which case nothing is selected
    if(self().itemCount() > 1) {
      setItemSelected(item->offset() + displacement);
    }
  }
  tabs.remove(item->offset());
  gtk_notebook_remove_page(GTK_NOTEBOOK(gtkWidget), item->offset());

  unsigned position = gtk_notebook_get_current_page(GTK_NOTEBOOK(gtkWidget));
  for(auto& item : state().items) item->state.selected = false;
  if(auto item = self().item(position)) item->state.selected = true;
  unlock();
}

auto pTabFrame::setFont(const Font& font) -> void {
  for(auto n : range(tabs.size())) {
    pFont::setFont(tabs[n].title, font);
    if(auto sizable = state().items[n]->state.sizable) {
      if(auto self = sizable->self()) {
        self->setFont(sizable->font(true));
      }
    }
  }
}

auto pTabFrame::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  geometry.setPosition();
  if(state().navigation == Navigation::Top || state().navigation == Navigation::Bottom) {
    geometry.setWidth(geometry.width() - 6);
    geometry.setHeight(geometry.height() - (15 + _tabHeight()));
  } else {
    geometry.setWidth(geometry.width() - (17 + _tabWidth()));
    geometry.setHeight(geometry.height() - 6);
  }
  for(auto& item : state().items) {
    if(item->state.sizable) item->state.sizable->setGeometry(geometry);
  }
}

auto pTabFrame::setItemClosable(unsigned position, bool closable) -> void {
  _synchronizeTab(position);
}

auto pTabFrame::setItemIcon(unsigned position, const image& icon) -> void {
  _synchronizeTab(position);
}

auto pTabFrame::setItemMovable(unsigned position, bool movable) -> void {
  lock();
  gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(gtkWidget), tabs[position].child, movable);
  unlock();
}

auto pTabFrame::setItemSelected(unsigned position) -> void {
  lock();
  gtk_notebook_set_current_page(GTK_NOTEBOOK(gtkWidget), position);
  unlock();
}

auto pTabFrame::setItemSizable(unsigned position, sSizable sizable) -> void {
//if(layout->self()) layout->self()->setParent();
}

auto pTabFrame::setItemText(unsigned position, const string& text) -> void {
  _synchronizeTab(position);
}

auto pTabFrame::setNavigation(Navigation navigation) -> void {
  GtkPositionType type;
  switch(navigation) { default:
  case Navigation::Top: type = GTK_POS_TOP; break;
  case Navigation::Bottom: type = GTK_POS_BOTTOM; break;
  case Navigation::Left: type = GTK_POS_LEFT; break;
  case Navigation::Right: type = GTK_POS_RIGHT; break;
  }
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(gtkWidget), type);
  setGeometry(self().geometry());
}

//called by pTabFrameItem::construct(), before pTabFrame::append()
//tab needs to be created early, so that pTabFrameItem child widgets calling pWidget::container() can find the tab
auto pTabFrame::_append() -> void {
  lock();
  Tab tab;
  tab.child = gtk_fixed_new();
  tab.container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  tab.image = gtk_image_new();
  tab.title = gtk_label_new("");
  gtk_misc_set_alignment(GTK_MISC(tab.title), 0.0, 0.5);
  tab.close = gtk_button_new_with_label("\u00d7");  //Unicode multiplication sign (looks better than 'X')
  gtk_button_set_focus_on_click(GTK_BUTTON(tab.close), false);
  gtk_button_set_relief(GTK_BUTTON(tab.close), GTK_RELIEF_NONE);
  pFont::setFont(tab.close, Font("sans", 9).setBold());
  auto color = CreateColor({255, 0, 0});
  gtk_widget_modify_fg(gtk_bin_get_child(GTK_BIN(tab.close)), GTK_STATE_PRELIGHT, &color);
  tabs.append(tab);

  gtk_widget_show(tab.child);
  gtk_widget_show(tab.container);
  gtk_widget_show(tab.image);
  gtk_widget_show(tab.title);
  gtk_widget_show(tab.close);
  gtk_box_pack_start(GTK_BOX(tab.container), tab.image, false, false, 0);
  gtk_box_pack_start(GTK_BOX(tab.container), tab.title, true, true, 0);
  gtk_box_pack_start(GTK_BOX(tab.container), tab.close, false, false, 0);

  g_signal_connect(G_OBJECT(tab.close), "clicked", G_CALLBACK(TabFrame_close), (gpointer)this);
  gtk_notebook_append_page(GTK_NOTEBOOK(gtkWidget), tab.child, tab.container);
  unlock();
}

auto pTabFrame::_synchronizeLayout() -> void {
  for(auto& item : state().items) {
    if(auto& sizable = item->state.sizable) {
      if(auto self = sizable->self()) {
        self->setVisible(sizable->visible() && item->selected());
      }
    }
  }
}

auto pTabFrame::_synchronizeTab(unsigned position) -> void {
  auto& item = state().items[position];
  auto& tab = tabs[position];
  gtk_widget_set_visible(tab.close, item->closable());
  if(auto& icon = item->state.icon) {
    uint size = pFont::size(self().font(true), " ").height();
    auto pixbuf = CreatePixbuf(icon, true);
    gtk_image_set_from_pixbuf(GTK_IMAGE(tab.image), pixbuf);
  } else {
    gtk_image_clear(GTK_IMAGE(tab.image));
  }
  string text = {
    item->state.icon && item->state.text ? " " : "",
    item->state.text,
    item->state.text && item->state.closable ? " " : ""
  };
  gtk_label_set_text(GTK_LABEL(tab.title), text);
}

//compute the height of the tallest tab for child layout geometry calculations
auto pTabFrame::_tabHeight() -> unsigned {
  signed height = 1;

  for(auto n : range(self().itemCount())) {
    GtkAllocation imageAllocation, titleAllocation, closeAllocation;
    gtk_widget_get_allocation(tabs[n].image, &imageAllocation);
    gtk_widget_get_allocation(tabs[n].title, &titleAllocation);
    gtk_widget_get_allocation(tabs[n].close, &closeAllocation);
    height = max(height, imageAllocation.height);
    height = max(height, titleAllocation.height);
    if(!state().items[n]->closable()) continue;
    height = max(height, closeAllocation.height);
  }

  return height;
}

auto pTabFrame::_tabWidth() -> unsigned {
  signed width = 1;

  for(auto n : range(self().itemCount())) {
    GtkAllocation imageAllocation, titleAllocation, closeAllocation;
    gtk_widget_get_allocation(tabs[n].image, &imageAllocation);
    gtk_widget_get_allocation(tabs[n].title, &titleAllocation);
    gtk_widget_get_allocation(tabs[n].close, &closeAllocation);
    width = max(width, imageAllocation.width + titleAllocation.width +
      (state().items[n]->closable() ? closeAllocation.width : 0)
    );
  }

  return width;
}

}

#endif
