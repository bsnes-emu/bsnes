auto mMenuSeparator::allocate() -> pObject* {
  return new pMenuSeparator(*this);
}
