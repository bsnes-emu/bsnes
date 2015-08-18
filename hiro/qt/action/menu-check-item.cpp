#if defined(Hiro_MenuCheckItem)

namespace hiro {

auto pMenuCheckItem::construct() -> void {
  qtMenuCheckItem = new QtMenuCheckItem(*this);
  qtMenuCheckItem->setCheckable(true);
  qtMenuCheckItem->connect(qtMenuCheckItem, SIGNAL(triggered()), SLOT(onToggle()));

  if(auto parent = _parentMenu()) {
    parent->qtMenu->addAction(qtMenuCheckItem);
  }

  if(auto parent = _parentPopupMenu()) {
    parent->qtPopupMenu->addAction(qtMenuCheckItem);
  }

  _setState();
}

auto pMenuCheckItem::destruct() -> void {
  delete qtMenuCheckItem;
  qtMenuCheckItem = nullptr;
}

auto pMenuCheckItem::setChecked(bool checked) -> void {
  _setState();
}

auto pMenuCheckItem::setText(const string& text) -> void {
  _setState();
}

auto pMenuCheckItem::_setState() -> void {
  qtMenuCheckItem->setChecked(state().checked);
  qtMenuCheckItem->setText(QString::fromUtf8(state().text));
}

auto QtMenuCheckItem::onToggle() -> void {
  p.state().checked = isChecked();
  p.self().doToggle();
}

}

#endif
