void Viewport::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height) {
  viewport->setParent(parent.window->container);
  viewport->setGeometry(x, y, width, height);
  viewport->show();
}

uintptr_t Viewport::handle() {
  return (uintptr_t)viewport->winId();
}

Viewport::Viewport() {
  viewport = new Viewport::Data(*this);
  widget->widget = viewport;
}
