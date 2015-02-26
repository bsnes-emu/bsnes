namespace hiro {

static auto MenuRadioItem_activate(GtkCheckMenuItem* gtkCheckMenuItem, pMenuRadioItem* p) -> void {
  p->_doActivate();
}

auto pMenuRadioItem::construct() -> void {
  widget = gtk_radio_menu_item_new_with_mnemonic(0, "");
  setGroup(state().group);
  setText(state().text);
  for(auto& weak : state().group) {
    if(auto item = weak.acquire()) {
      if(item->self()) gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item->self()->widget), item->checked());
    }
  }
  g_signal_connect(G_OBJECT(widget), "toggled", G_CALLBACK(MenuRadioItem_activate), (gpointer)this);
}

auto pMenuRadioItem::destruct() -> void {
  if(widget) gtk_widget_destroy(widget), widget = nullptr;
}

auto pMenuRadioItem::setChecked() -> void {
  _parent().lock();
  for(auto& weak : state().group) {
    if(auto item = weak.acquire()) {
      if(item->self()) gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item->self()->widget), false);
    }
  }
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), true);
  _parent().unlock();
}

auto pMenuRadioItem::setGroup(const vector<shared_pointer_weak<mMenuRadioItem>>& group) -> void {
  _parent().lock();
  shared_pointer<mMenuRadioItem> first;
  for(auto& weak : group) {
    if(!first) {
      first = weak.acquire();
      continue;
    }
    if(auto item = weak.acquire()) {
      if(item->self() && first->self()) {
        GSList* currentGroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(first->self()->widget));
        if(currentGroup != gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item->self()->widget))) {
          gtk_radio_menu_item_set_group(GTK_RADIO_MENU_ITEM(item->self()->widget), currentGroup);
        }
      }
    }
  }
  _parent().unlock();
}

auto pMenuRadioItem::setText(const string& text) -> void {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), _mnemonic(text));
}

auto pMenuRadioItem::_doActivate() -> void {
  if(!_parent().locked()) {
    bool wasChecked = state().checked;
    self().setChecked();
    if(!wasChecked) self().doActivate();
  }
}

auto pMenuRadioItem::_parent() -> pMenuRadioItem& {
  if(state().group.size()) {
    if(auto item = state().group.first().acquire()) {
      if(item->self()) return *item->self();
    }
  }
  return *this;
}

}
