#if defined(Hiro_MenuRadioItem)

namespace hiro {

auto pMenuRadioItem::construct() -> void {
  setGroup(state().group);
}

auto pMenuRadioItem::destruct() -> void {
}

auto pMenuRadioItem::setChecked() -> void {
  if(auto& group = state().group) {
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto menuRadioItem = dynamic_cast<mMenuRadioItem*>(object.data())) {
          if(auto self = menuRadioItem->self()) {
            if(auto menu = self->_parentMenu()) {
              //CheckMenuRadioItem takes: lo, hi, id; checking only id when lo <= id <= hi
              //hiro does not force IDs to be linear, so to uncheck id, we use: lo == hi == id + 1 (out of range)
              //to check id, we use: lo == hi == id (only ID, but in range)
              CheckMenuRadioItem(
                menu->hmenu,
                self->position, self->position, self->position + (position != self->position),
                MF_BYPOSITION
              );
            }
          }
        }
      }
    }
  }
}

auto pMenuRadioItem::setGroup(sGroup group) -> void {
  bool first = true;
  if(auto& group = state().group) {
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto menuRadioItem = dynamic_cast<mMenuRadioItem*>(object.data())) {
          if(auto self = menuRadioItem->self()) {
            menuRadioItem->state.checked = first;
            first = false;
          }
        }
      }
    }
  }
  setChecked();
}

auto pMenuRadioItem::setText(const string& text) -> void {
  _synchronize();
}

auto pMenuRadioItem::onActivate() -> void {
  if(state().checked) return;
  self().setChecked();
  self().doActivate();
}

}

#endif
