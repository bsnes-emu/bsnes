#if defined(Hiro_MenuSeparator)

auto mMenuSeparator::allocate() -> pObject* {
  return new pMenuSeparator(*this);
}

#endif
