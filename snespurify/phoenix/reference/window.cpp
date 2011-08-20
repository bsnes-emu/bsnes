void pWindow::append(Layout &layout) {
}

void pWindow::append(Menu &menu) {
}

void pWindow::append(Widget &widget) {
}

Color pWindow::backgroundColor() {
  return { 0, 0, 0, 255 };
}

bool pWindow::focused() {
  return false;
}

Geometry pWindow::frameMargin() {
  return { 0, 0, 0, 0 };
}

Geometry pWindow::geometry() {
  return { 0, 0, 0, 0 };
}

void pWindow::setBackgroundColor(const Color &color) {
}

void pWindow::setFocused() {
}

void pWindow::setFullScreen(bool fullScreen) {
}

void pWindow::setGeometry(const Geometry &geometry) {
}

void pWindow::setMenuFont(Font &font) {
}

void pWindow::setMenuVisible(bool visible) {
}

void pWindow::setResizable(bool resizable) {
}

void pWindow::setStatusFont(Font &font) {
}

void pWindow::setStatusText(const string &text) {
}

void pWindow::setStatusVisible(bool visible) {
}

void pWindow::setTitle(const string &text) {
}

void pWindow::setVisible(bool visible) {
}

void pWindow::setWidgetFont(Font &font) {
}

void pWindow::constructor() {
}
