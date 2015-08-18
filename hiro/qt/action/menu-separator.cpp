#if defined(Hiro_MenuSeparator)

namespace hiro {

auto pMenuSeparator::construct() -> void {
  qtMenuSeparator = new QAction(nullptr);
  qtMenuSeparator->setSeparator(true);

  if(auto parent = _parentMenu()) {
    parent->qtMenu->addAction(qtMenuSeparator);
  }

  if(auto parent = _parentPopupMenu()) {
    parent->qtPopupMenu->addAction(qtMenuSeparator);
  }
}

auto pMenuSeparator::destruct() -> void {
  delete qtMenuSeparator;
  qtMenuSeparator = nullptr;
}

}

#endif
