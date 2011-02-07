void Layout::setParent(Window &parent) {
  layout->parent = &parent;
}

void Layout::setParent(Window &parent, Widget &child) {
  SetParent(child.widget->window, parent.widget->window);

  SendMessage(child.widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : OS::os->proportionalFont), 0);
}

Layout::Layout() {
  layout = new Layout::Data;
  layout->parent = 0;
}
