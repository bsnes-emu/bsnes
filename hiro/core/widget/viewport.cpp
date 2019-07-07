#if defined(Hiro_Viewport)

auto mViewport::allocate() -> pObject* {
  return new pViewport(*this);
}

//

auto mViewport::handle() const -> uintptr_t {
  return signal(handle);
}

#endif
