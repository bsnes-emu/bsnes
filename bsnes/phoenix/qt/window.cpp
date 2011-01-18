void Window::create(unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  window->setWindowTitle(QString::fromUtf8(text));
  window->move(x, y);

  window->layout = new QVBoxLayout(window);
  window->layout->setAlignment(Qt::AlignTop);
  window->layout->setMargin(0);
  window->layout->setSpacing(0);
  window->layout->setSizeConstraint(QLayout::SetFixedSize);
  window->setLayout(window->layout);

  window->menuBar = new QMenuBar(window);
  window->menuBar->setVisible(false);
  window->layout->addWidget(window->menuBar);

  window->container = new QWidget(window);
  window->container->setFixedSize(width, height);
  window->container->setVisible(true);
  window->layout->addWidget(window->container);

  window->statusBar = new QStatusBar(window);
  window->statusBar->setSizeGripEnabled(false);
  window->statusBar->setVisible(false);
  window->layout->addWidget(window->statusBar);
}

Geometry Window::geometry() {
  return Geometry(window->x(), window->y(), window->container->width(), window->container->height());
}

void Window::setGeometry(unsigned x, unsigned y, unsigned width, unsigned height) {
  window->container->setFixedSize(width, height);
  window->move(x, y);
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
  window->setPalette(palette);
  window->setAutoFillBackground(true);
}

void Window::setTitle(const string &text) {
  window->setWindowTitle(QString::fromUtf8(text));
}

void Window::setStatusText(const string &text) {
  window->statusBar->showMessage(QString::fromUtf8(text), 0);
}

void Window::setMenuVisible(bool visible) {
  if(visible) window->menuBar->show();
  else window->menuBar->hide();
}

void Window::setStatusVisible(bool visible) {
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
  if(fullscreen == false) {
    window->showNormal();
  } else {
    window->showFullScreen();
  }

  //Qt returns negative coordinates for x,y immediately after setFullscreen(false)
  //wait for Qt to return sane values, or until timeout occurs
  Geometry geom;
  time_t startTime = time(0);
  do {
    OS::run();
    geom = geometry();
    if(startTime - time(0) > 3) break;
  } while((signed)geom.x < 0 || (signed)geom.y < 0);
}

Window::Window() {
  window = new Window::Data(*this);
  window->defaultFont = 0;
  widget->widget = window;
}
