#if defined(Hiro_HorizontalResizeGrip)

mHorizontalResizeGrip::mHorizontalResizeGrip() {
  image icon;
  icon.allocate(5, 15);
  for(uint y : range(icon.height())) {
    auto data = icon.data() + y * icon.pitch();
    icon.write(data, 0x00000000); data += icon.stride();
    icon.write(data, 0xff9f9f9f); data += icon.stride();
    icon.write(data, 0x00000000); data += icon.stride();
    icon.write(data, 0xff9f9f9f); data += icon.stride();
    icon.write(data, 0x00000000); data += icon.stride();
  }
  mCanvas::setIcon(icon);
  mCanvas::onMousePress([&](auto button) {
    if(button == Mouse::Button::Left && !state.timer.enabled()) {
      doActivate();
      state.offset = 0;
      state.origin = Mouse::position();
      state.timer.setEnabled();
    }
  });
  state.timer.setInterval(10).onActivate([&] {
    if(!Mouse::pressed(Mouse::Button::Left)) return (void)state.timer.setEnabled(false);
    auto position = Mouse::position();
    auto offset = position.x() - state.origin.x();
    if(offset != state.offset) doResize(offset), offset = state.offset;
  });
}

auto mHorizontalResizeGrip::doActivate() const -> void {
  if(state.onActivate) state.onActivate();
}

auto mHorizontalResizeGrip::doResize(int offset) const -> void {
  if(state.onResize) state.onResize(offset);
}

auto mHorizontalResizeGrip::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
  return *this;
}

auto mHorizontalResizeGrip::onResize(const function<void (int)>& callback) -> type& {
  state.onResize = callback;
  return *this;
}

#endif
