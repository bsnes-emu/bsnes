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

  _setState();
}

auto pMenuRadioItem::destruct() -> void {
  delete qtMenuRadioItem;
  delete qtActionGroup;
  qtMenuRadioItem = nullptr;
  qtActionGroup = nullptr;
}

auto pMenuRadioItem::setChecked() -> void {
  _setState();
}

auto pMenuRadioItem::setGroup(sGroup group) -> void {
  _setState();
}

auto pMenuRadioItem::setText(const string& text) -> void {
  _setState();
}

auto pMenuRadioItem::_setState() -> void {
  if(auto group = state().group) {
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
  p.state().checked = true;
  p.self().doActivate();
}

}

#endif
