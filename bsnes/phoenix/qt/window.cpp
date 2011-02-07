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

void Window::setResizable(bool resizable) {
  window->resizable = resizable;
  if(window->resizable) {
    window->vlayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    window->container->setMinimumSize(window->width, window->height);
  } else {
    window->vlayout->setSizeConstraint(QLayout::SetFixedSize);
    window->container->setFixedSize(window->width, window->height);
  }
}

Geometry Window::frameGeometry() {
  if(window->fullscreen) return { 0, 0, OS::desktopWidth(), OS::desktopHeight() };

  QRect rect = window->frameGeometry();
  unsigned x = rect.x();
  unsigned y = rect.y();
  unsigned width = rect.width();
  unsigned height = rect.height();
  if(x > 65535) x = 0;
  if(y > 65535) y = 0;

  return { x, y, width, height };
}

Geometry Window::geometry() {
  if(window->fullscreen) return { 0, 0, OS::desktopWidth(), OS::desktopHeight() };
  return { window->x, window->y, window->width, window->height };

  unsigned x = window->x;
  unsigned y = window->y;

  if(window->fullscreen == false) {
    QRect border = window->frameGeometry();
    QRect client = window->geometry();
    x += client.x() - border.x();
    y += client.y() - border.y();
    if(window->menuVisible) y += window->menuBar->height();
    if(x > 65535) x = 0;
    if(y > 65535) y = 0;
  }

  return { x, y, window->width, window->height };
}

void Window::setGeometry(unsigned x, unsigned y, unsigned width, unsigned height) {
  object->locked = true;

  QRect border = window->frameGeometry();
  QRect client = window->geometry();

  window->x = x;
  window->y = y;
  window->width = width;
  window->height = height;

  setResizable(window->resizable);
  window->move(x - (client.x() - border.x()), y - (client.y() - border.y()));
  window->adjustSize();

  object->locked = false;
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
  object->locked = true;

  if(visible) {
    window->show();
    //an unfortunate hack: before window is visible, geometry() == frameGeometry();
    //we must wait for frameGeometry() to correctly populate and then move window
    if(window->fullscreen == false) for(unsigned n = 0; n < 100; n++) {
      if(window->geometry().x() > window->frameGeometry().x()) break;
      usleep(100);
      QApplication::processEvents();
    }
    setGeometry(window->x, window->y, window->width, window->height);
  } else {
    window->hide();
  }

  object->locked = false;
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
    setResizable(window->resizable);
    window->showNormal();
    window->adjustSize();
  } else {
    window->vlayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    window->container->setFixedSize(OS::desktopWidth(), OS::desktopHeight());
    window->showFullScreen();
  }
}

Window::Window() {
  window = new Window::Data(*this);
  window->defaultFont = 0;
}
