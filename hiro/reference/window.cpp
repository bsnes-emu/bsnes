namespace phoenix {

Window& pWindow::none() {
  static Window* window = nullptr;
  if(window == nullptr) window = new Window;
  return *window;
}

void pWindow::append(Layout& layout) {
}

void pWindow::append(Menu& menu) {
}

void pWindow::append(Widget& widget) {
}

bool pWindow::focused() {
  return false;
}

Geometry pWindow::frameMargin() {
  return {0, 0, 0, 0};
}

void pWindow::remove(Layout& layout) {
}

void pWindow::remove(Menu& menu) {
}

void pWindow::remove(Widget& widget) {
}

void pWindow::setBackgroundColor(Color color) {
}

void pWindow::setDroppable(bool droppable) {
}

void pWindow::setFocused() {
}

void pWindow::setFullScreen(bool fullScreen) {
}

void pWindow::setGeometry(Geometry geometry) {
}

void pWindow::setMenuFont(string font) {
}

void pWindow::setMenuVisible(bool visible) {
}

void pWindow::setModal(bool modal) {
}

void pWindow::setResizable(bool resizable) {
}

void pWindow::setStatusFont(string font) {
}

void pWindow::setStatusText(string text) {
}

void pWindow::setStatusVisible(bool visible) {
}

void pWindow::setTitle(string text) {
}

void pWindow::setVisible(bool visible) {
}

void pWindow::setWidgetFont(string font) {
}

void pWindow::constructor() {
}

void pWindow::destructor() {
}

}
