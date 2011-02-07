void Window::create(unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  window->setWindowTitle(QString::fromUtf8(text));

  window->vlayout = new QVBoxLayout(window);
  window->vlayout->setMargin(0);
  window->vlayout->setSpacing(0);
  window->setLayout(window->vlayout);

  window->menuBar = new QMenuBar(window);
  window->menuBar->setVisible(false);
  window->vlayout->addWidget(window->menuBar);

  window->container = new QWidget(window);
  window->container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  window->container->setVisible(true);
  window->vlayout->addWidget(window->container);

  window->statusBar = new QStatusBar(window);
  window->statusBar->setSizeGripEnabled(false);
  window->statusBar->setVisible(false);
  window->vlayout->addWidget(window->statusBar);

  setGeometry(x, y, width, height);
}

void Window::setLayout(Layout &layout) {
  window->layout = &layout;

  Geometry geom = geometry();
  geom.x = geom.y = 0;
  layout.setParent(*this);
  layout.update(geom);
}

Geometry Window::geometry() {
  //QWidget::geometry() is not at all reliable
  if(window->fullscreen == false) return { window->x, window->y, window->width, window->height };
  return { 0, 0, OS::desktopWidth(), OS::desktopHeight() };
}

void Window::setGeometry(unsigned x, unsigned y, unsigned width, unsigned height) {
  window->x = x;
  window->y = y;
  window->width = width;
  window->height = height;

  window->move(x, y);
  window->adjustSize();
}

void Window::setDefaultFont(Font &font) {
  window->defaultFont = font.font;
  window->menuBar->setFont(*font.font);
}

void Window::setFont(Font &font) {
  window->statusBar->setFont(*font.font);
}

void Window::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) {
  QPalette palette;
  palette.setColor(QPalette::Window, QColor(red, green, blue));
  window->container->setPalette(palette);
  window->container->setAutoFillBackground(true);
}

void Window::setTitle(const string &text) {
  window->setWindowTitle(QString::fromUtf8(text));
}

void Window::setStatusText(const string &text) {
  window->statusBar->showMessage(QString::fromUtf8(text), 0);
}

void Window::setVisible(bool visible) {
  if(visible) {
    window->show();
    window->adjustSize();
  } else {
    window->hide();
  }
}

void Window::setMenuVisible(bool visible) {
  window->menuVisible = visible;
  if(visible) window->menuBar->show();
  else window->menuBar->hide();
}

void Window::setStatusVisible(bool visible) {
  window->statusVisible = visible;
  if(visible) window->statusBar->show();
  else window->statusBar->hide();
}

bool Window::focused() {
  return window->isActiveWindow() && !window->isMinimized();
}

bool Window::fullscreen() {
  return window->isFullScreen();
}

void Window::setFullscreen(bool fullscreen) {
  window->fullscreen = fullscreen;

  if(fullscreen == false) {
    window->showNormal();
    window->adjustSize();
  } else {
    window->showFullScreen();
  }
}

Window::Window() {
  window = new Window::Data(*this);
  window->defaultFont = 0;
}
