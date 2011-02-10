void Window::append(Menu &menu) {
  menu.menu->parent = this;
  if(window->defaultFont) menu.menu->setFont(*window->defaultFont);
  window->menuBar->addMenu(menu.menu);
}

void Window::setLayout(Layout &layout) {
  window->layout = &layout;

  Geometry geom = geometry();
  geom.x = geom.y = 0;
  layout.setParent(*this);
  layout.setGeometry(geom);
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
  return {
    window->x - settings.frameGeometryX, window->y - settings.frameGeometryY,
    window->width + settings.frameGeometryWidth, window->height + settings.frameGeometryHeight
  };
}

Geometry Window::geometry() {
  if(window->fullscreen) return { 0, 0, OS::desktopWidth(), OS::desktopHeight() };
  return { window->x, window->y, window->width, window->height };
}

void Window::setFrameGeometry(signed x, signed y, unsigned width, unsigned height) {
  setGeometry(
    x + settings.frameGeometryX, y + settings.frameGeometryY,
    width - settings.frameGeometryWidth, height - settings.frameGeometryHeight
  );
}

void Window::setGeometry(signed x, signed y, unsigned width, unsigned height) {
  object->locked = true;

  window->x = x;
  window->y = y;
  window->width = width;
  window->height = height;

  setResizable(window->resizable);
  window->move(x - settings.frameGeometryX, y - settings.frameGeometryY);
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
    updateFrameGeometry();
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

  setGeometry(128, 128, 256, 256);
}

//

void Window::updateFrameGeometry() {
  //Qt does not even attempt to cache frameGeometry, so if this is called before
  //a window is visible onscreen, it is equivalent to geometry(). Without proper
  //frameGeometry, phoenix's set(Frame)Geometry functions will not work
  //properly. Attempting to detect frame geometry after the window is visible
  //would require moving a visible window, causing a slight jitter effect.

  //This routine is called every time a new window is shown, and attempts to
  //cache frame geometry for the next set(Frame)Geometry call. The information
  //is stored to disk, so that hopefully a phoenix window will never jitter more
  //than once.

  //The information is constantly updated after each window show to detect theme
  //changes that may adjust frame geometry.

  if(window->fullscreen == false) {
    for(unsigned n = 0; n < 100; n++) {
      if(window->geometry().x() > window->frameGeometry().x()) break;
      usleep(100);
      QApplication::processEvents();
    }
  }
  QRect border = window->frameGeometry();
  QRect client = window->geometry();

  settings.frameGeometryX = client.x() - border.x();
  settings.frameGeometryY = client.y() - border.y();
  settings.frameGeometryWidth = border.width() - client.width();
  settings.frameGeometryHeight = border.height() - client.height();
}
