#if defined(Hiro_MenuRadioItem)

namespace hiro {

static auto MenuRadioItem_activate(GtkCheckMenuItem* gtkCheckMenuItem, pMenuRadioItem* p) -> void {
  p->_doActivate();
}

auto pMenuRadioItem::construct() -> void {
  widget = gtk_radio_menu_item_new_with_mnemonic(0, "");
  gtkCheckMenuItem = GTK_CHECK_MENU_ITEM(widget);
  gtkRadioMenuItem = GTK_RADIO_MENU_ITEM(widget);

  setText(state().text);

  g_signal_connect(G_OBJECT(widget), "toggled", G_CALLBACK(MenuRadioItem_activate), (gpointer)this);
}

auto pMenuRadioItem::destruct() -> void {
  if(widget) gtk_widget_destroy(widget), widget = nullptr;
}

auto pMenuRadioItem::setChecked() -> void {
  lock();
  gtk_check_menu_item_set_active(gtkCheckMenuItem, true);
  unlock();
}

auto pMenuRadioItem::setGroup(sGroup group) -> void {
  if(!group) return;

  maybe<GtkRadioMenuItem*> gtkRadioMenuItem;
  for(auto& weak : group->state.objects) {
    if(auto object = weak.acquire()) {
      if(auto menuRadioItem = dynamic_cast<mMenuRadioItem*>(object.data())) {
        if(auto self = menuRadioItem->self()) {
          self->lock();
          gtk_radio_menu_item_set_group(self->gtkRadioMenuItem, nullptr);
          if(!gtkRadioMenuItem) gtkRadioMenuItem = self->gtkRadioMenuItem;
          else gtk_radio_menu_item_set_group(self->gtkRadioMenuItem, gtk_radio_menu_item_get_group(*gtkRadioMenuItem));
          gtk_check_menu_item_set_active(self->gtkCheckMenuItem, menuRadioItem->checked());
          self->unlock();
        }
      }
    }
  }
}

auto pMenuRadioItem::setText(const string& text) -> void {
  gtk_menu_item_set_label(GTK_MENU_ITEM(widget), _mnemonic(text));
}

auto pMenuRadioItem::groupLocked() const -> bool {
  if(auto group = state().group) {
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto self = object->self()) {
          if(self->locked()) return true;
        }
      }
    }
    return false;
  }
  return locked();
}

auto pMenuRadioItem::_doActivate() -> void {
  if(groupLocked()) return;
  bool wasChecked = state().checked;
  self().setChecked();
  if(!wasChecked) self().doActivate();
}

}

#endif
