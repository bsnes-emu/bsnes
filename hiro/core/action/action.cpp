auto mAction::allocate() -> pObject* {
  return new pAction(*this);
}

//

auto mAction::remove() -> type& {
  if(auto menu = parentMenu()) menu->remove(*this);
  if(auto menuBar = parentMenuBar()) menuBar->remove((mMenu&)*this);
  if(auto popupMenu = parentPopupMenu()) popupMenu->remove(*this);
  return *this;
}
