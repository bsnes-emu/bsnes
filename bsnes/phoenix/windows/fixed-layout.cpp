void FixedLayout::append(Widget &widget, unsigned x, unsigned y, unsigned width, unsigned height) {
  fixedLayout->widgets.append({ &widget, x, y, width, height });
}

void FixedLayout::create(Window &parent_) {
  SetParent(widget->window, parent_.widget->window);
  setGeometry(0, 0, 640, 480);

  foreach(control, fixedLayout->widgets) {
    SetParent(control.widget->widget->window, widget->window);
    control.widget->setGeometry(control.x, control.y, control.width, control.height);
  }
}

FixedLayout::FixedLayout() {
  fixedLayout = new FixedLayout::Data;
  widget->window = CreateWindow(
    L"phoenix_window", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE,
    0, 0, 64, 64,
    OS::os->nullWindow, 0, GetModuleHandle(0), 0
  );
}
