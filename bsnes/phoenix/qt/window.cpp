void Window::create(unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  window->setWindowTitle(QString::fromUtf8(text));
  window->move(x, y);

  window->vlayout = new QVBoxLayout(window);
  window->vlayout->setAlignment(Qt::AlignTop);
  window->vlayout->setMargin(0);
  window->vlayout->setSpacing(0);
  window->vlayout->setSizeConstraint(QLayout::SetFixedSize);
  window->setLayout(window->vlayout);

  window->menuBar = new QMenuBar(window);
  window->menuBar->setVisible(false);
  window->vlayout->addWidget(window->menuBar);

  window->container = new QWidget(window);
  window->container->setFixedSize(width, height);
  window->container->setVisible(true);
  window->vlayout->addWidget(window->container);

  window->statusBar = new QStatusBar(window);
  window->statusBar->setSizeGripEnabled(false);
  window->statusBar->setVisible(false);
  window->vlayout->addWidget(window->statusBar);
}

void Window::setLayout(Layout &layout) {
  window->layout = &layout;
  layout.create(*this);
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
  window->container->setPalette(palette);
  window->container->setAutoFillBackground(true);
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
    window->vlayout->setSizeConstraint(QLayout::SetFixedSize);
    window->showNormal();
  } else {
    window->vlayout->setSizeConstraint(QLayout::SetNoConstraint);
    window->container->setFixedSize(OS::desktopWidth(), OS::desktopHeight());
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

  if(fullscreen == false) {
    window->layout->setGeometry(0, 0, geometry().width, geometry().height);
  } else {
    window->layout->setGeometry(0, 0, OS::desktopWidth(), OS::desktopHeight());
  }
}

Window::Window() {
  window = new Window::Data(*this);
  window->defaultFont = 0;
  widget->widget = window;
}
