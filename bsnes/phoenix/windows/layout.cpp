void Layout::setParent(Window &parent) {
  layout->parent = &parent;
}

void Layout::append(Widget &child) {
  SetParent(child.widget->window, layout->parent->window->window);
  SendMessage(
    child.widget->window, WM_SETFONT,
    (WPARAM)(layout->parent->window->defaultFont ? layout->parent->window->defaultFont : OS::os->proportionalFont), 0
  );
}

Layout::Layout() {
  layout = new Layout::Data;
  layout->parent = 0;
}
