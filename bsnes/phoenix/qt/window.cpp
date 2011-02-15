void pWindow::append(Menu &menu) {
  qtMenu->addMenu(menu.p.qtMenu);
}

Geometry pWindow::frameGeometry() {
  if(window.state.fullScreen) return { 0, 0, OS::desktopWidth(), OS::desktopHeight() };
  return {
    window.state.geometry.x - settings.frameGeometryX,
    window.state.geometry.y - settings.frameGeometryY,
    window.state.geometry.width + settings.frameGeometryWidth,
    window.state.geometry.height + settings.frameGeometryHeight
  };
}

bool pWindow::focused() {
  return qtWindow->isActiveWindow() && !qtWindow->isMinimized();
}

Geometry pWindow::geometry() {
  if(window.state.fullScreen) {
    unsigned width = OS::desktopWidth(), height = OS::desktopHeight();
    if(window.state.menuVisible) height -= qtMenu->height();
    if(window.state.statusVisible) height -= qtStatus->height();
    return { 0, 0, width, height };
  }
  return window.state.geometry;
}

void pWindow::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) {
  QPalette palette;
  palette.setColor(QPalette::Window, QColor(red, green, blue));
  qtContainer->setPalette(palette);
  qtContainer->setAutoFillBackground(true);
}

void pWindow::setFrameGeometry(const Geometry &geometry) {
  window.state.geometry = {
    geometry.x + settings.frameGeometryX,
    geometry.y + settings.frameGeometryY,
    geometry.width - settings.frameGeometryWidth,
    geometry.height - settings.frameGeometryHeight
  };
  if(window.state.menuVisible) window.state.geometry.height -= qtMenu->height();
  if(window.state.statusVisible) window.state.geometry.height -= qtStatus->height();
  setGeometry(window.state.geometry);
}

void pWindow::setFocused() {
  qtWindow->raise();
  qtWindow->activateWindow();
}

void pWindow::setFullScreen(bool fullScreen) {
  if(fullScreen == false) {
    setResizable(window.state.resizable);
    qtWindow->showNormal();
    qtWindow->adjustSize();
  } else {
    qtLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    qtContainer->setFixedSize(OS::desktopWidth(), OS::desktopHeight());
    qtWindow->showFullScreen();
    if(window.state.statusVisible) setStatusVisible(true);  //work around for Qt/Xlib bug
  }
}

void pWindow::setGeometry(const Geometry &geometry_) {
  locked = true;
  Geometry geometry = geometry_;

  setResizable(window.state.resizable);
  qtWindow->move(geometry.x - settings.frameGeometryX, geometry.y - settings.frameGeometryY);
  qtWindow->adjustSize();

  geometry.x = geometry.y = 0;
  if(layout) layout->setGeometry(geometry);
  locked = false;
}

void pWindow::setLayout(Layout &layout) {
  this->layout = &layout;

  layout.p.parent = this;
  layout.setParent(window);

  Geometry geometry = window.state.geometry;
  geometry.x = geometry.y = 0;
  layout.setGeometry(geometry);
}

void pWindow::setMenuFont(Font &font) {
  qtMenu->setFont(*font.p.qtFont);
}

void pWindow::setMenuVisible(bool visible) {
  qtMenu->setVisible(visible);
  setGeometry(window.state.geometry);
}

void pWindow::setResizable(bool resizable) {
  if(resizable) {
    qtLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    qtContainer->setMinimumSize(window.state.geometry.width, window.state.geometry.height);
  } else {
    qtLayout->setSizeConstraint(QLayout::SetFixedSize);
    qtContainer->setFixedSize(window.state.geometry.width, window.state.geometry.height);
  }
  qtStatus->setSizeGripEnabled(resizable);
}

void pWindow::setStatusFont(Font &font) {
  qtStatus->setFont(*font.p.qtFont);
}

void pWindow::setStatusText(const string &text) {
  qtStatus->showMessage(QString::fromUtf8(text), 0);
}

void pWindow::setStatusVisible(bool visible) {
  qtStatus->setVisible(visible);
  setGeometry(window.state.geometry);
}

void pWindow::setTitle(const string &text) {
  qtWindow->setWindowTitle(QString::fromUtf8(text));
}

void pWindow::setVisible(bool visible) {
  locked = true;
  qtWindow->setVisible(visible);
  if(visible) {
    updateFrameGeometry();
    setGeometry(window.state.geometry);
  }
  locked = false;
}

pWindow::pWindow(Window &window) : window(window) {
  layout = 0;

  qtWindow = new QtWindow(*this);
  qtWindow->setWindowTitle(" ");

  qtLayout = new QVBoxLayout(qtWindow);
  qtLayout->setMargin(0);
  qtLayout->setSpacing(0);
  qtWindow->setLayout(qtLayout);

  qtMenu = new QMenuBar(qtWindow);
  if(OS::state->defaultFont) qtMenu->setFont(*OS::state->defaultFont->p.qtFont);
  qtMenu->setVisible(false);
  qtLayout->addWidget(qtMenu);

  qtContainer = new QWidget(qtWindow);
  qtContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  qtContainer->setVisible(true);
  qtLayout->addWidget(qtContainer);

  qtStatus = new QStatusBar(qtWindow);
  if(OS::state->defaultFont) qtStatus->setFont(*OS::state->defaultFont->p.qtFont);
  qtStatus->setSizeGripEnabled(true);
  qtStatus->setVisible(false);
  qtLayout->addWidget(qtStatus);

  setGeometry(window.state.geometry);
}

void pWindow::updateFrameGeometry() {
  if(window.state.fullScreen == false) for(unsigned n = 0; n < 100; n++) {
    if(qtWindow->geometry().x() > qtWindow->frameGeometry().x()) break;
    usleep(100);
    QApplication::processEvents();
  }
  QRect border = qtWindow->frameGeometry();
  QRect client = qtWindow->geometry();

  settings.frameGeometryX = client.x() - border.x();
  settings.frameGeometryY = client.y() - border.y();
  settings.frameGeometryWidth = border.width() - client.width();
  settings.frameGeometryHeight = border.height() - client.height();
}

void pWindow::QtWindow::closeEvent(QCloseEvent *event) {
  event->ignore();
  hide();
  if(self.window.onClose) self.window.onClose();
}

void pWindow::QtWindow::moveEvent(QMoveEvent *event) {
  if(self.locked == false && self.window.state.fullScreen == false && self.qtWindow->isVisible() == true) {
    self.window.state.geometry.x += event->pos().x() - event->oldPos().x();
    self.window.state.geometry.y += event->pos().y() - event->oldPos().y();
  }

  if(self.locked == false) {
    if(self.window.onMove) self.window.onMove();
  }
}

void pWindow::QtWindow::resizeEvent(QResizeEvent*) {
  if(self.locked == false && self.window.state.fullScreen == false && self.qtWindow->isVisible() == true) {
    self.window.state.geometry.width = self.qtContainer->geometry().width();
    self.window.state.geometry.height = self.qtContainer->geometry().height();
  }

  if(self.layout) {
    Geometry geometry = self.geometry();
    geometry.x = geometry.y = 0;
    self.layout->setGeometry(geometry);
  }

  if(self.locked == false) {
    if(self.window.onSize) self.window.onSize();
  }
}

QSize pWindow::QtWindow::sizeHint() const {
  unsigned width = self.window.state.geometry.width;
  unsigned height = self.window.state.geometry.height;
  if(self.window.state.menuVisible) height += self.qtMenu->height();
  if(self.window.state.statusVisible) height += self.qtStatus->height();
  return QSize(width, height);
}
