#if defined(Hiro_Action)

auto mAction::allocate() -> pObject* {
  return new pAction(*this);
}

//

auto mAction::remove() -> type& {
  if(auto menu = parentMenu()) menu->remove(*this);
  if(auto menuBar = parentMenuBar()) menuBar->remove((mMenu&)*this);
  #if defined(Hiro_PopupMenu)
  if(auto popupMenu = parentPopupMenu()) popupMenu->remove(*this);
  #endif
  return *this;
}

#endif
