#if defined(Hiro_MenuRadioItem)

namespace hiro {

auto pMenuRadioItem::construct() -> void {
  qtMenuRadioItem = new QtMenuRadioItem(*this);
  qtActionGroup = new QActionGroup(nullptr);
  qtMenuRadioItem->setCheckable(true);
  qtMenuRadioItem->setActionGroup(qtActionGroup);
  qtMenuRadioItem->setChecked(true);
  qtMenuRadioItem->connect(qtMenuRadioItem, SIGNAL(triggered()), SLOT(onActivate()));

  if(auto parent = _parentMenu()) {
    parent->qtMenu->addAction(qtMenuRadioItem);
  }

  if(auto parent = _parentPopupMenu()) {
    parent->qtPopupMenu->addAction(qtMenuRadioItem);
  }

  setGroup(state().group);
  _setState();
}

auto pMenuRadioItem::destruct() -> void {
if(Application::state().quit) return;  //TODO: hack
  delete qtMenuRadioItem;
  delete qtActionGroup;
  qtMenuRadioItem = nullptr;
  qtActionGroup = nullptr;
}

auto pMenuRadioItem::setChecked() -> void {
  _setState();
}

auto pMenuRadioItem::setGroup(sGroup group) -> void {
  bool first = true;
  if(auto& group = state().group) {
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto menuRadioItem = dynamic_cast<mMenuRadioItem*>(object.data())) {
          if(auto self = menuRadioItem->self()) {
            self->qtMenuRadioItem->setChecked(menuRadioItem->state.checked = first);
            first = false;
          }
        }
      }
    }
  }
  _setState();
}

auto pMenuRadioItem::setText(const string& text) -> void {
  _setState();
}

auto pMenuRadioItem::_setState() -> void {
  if(auto& group = state().group) {
    if(auto object = group->object(0)) {
      if(auto menuRadioItem = dynamic_cast<mMenuRadioItem*>(object.data())) {
        if(auto self = menuRadioItem->self()) {
          qtMenuRadioItem->setActionGroup(self->qtActionGroup);
        }
      }
    }
  }
  qtMenuRadioItem->setChecked(state().checked);
  qtMenuRadioItem->setText(QString::fromUtf8(state().text));
}

auto QtMenuRadioItem::onActivate() -> void {
  if(p.state().checked) return;
  p.self().setChecked();
  p.self().doActivate();
}

}

#endif
