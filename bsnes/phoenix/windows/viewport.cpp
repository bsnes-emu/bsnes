void Viewport::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height) {
  widget->window = CreateWindow(
    L"phoenix_window", L"",
    WS_CHILD | WS_VISIBLE | WS_DISABLED,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
}

uintptr_t Viewport::handle() {
  return (uintptr_t)widget->window;
}
