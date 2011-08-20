void pWindow::append(Layout &layout) {
  layout.setParent(window);
  Geometry geometry = window.state.geometry;
  geometry.x = geometry.y = 0;
  layout.setGeometry(geometry);
}

void pWindow::append(Menu &menu) {
  if(window.state.menuFont) menu.p.qtMenu->setFont(*window.state.menuFont->p.qtFont);
  qtMenu->addMenu(menu.p.qtMenu);
}

void pWindow::append(Widget &widget) {
  if(!widget.state.font && window.state.widgetFont) {
    widget.setFont(*window.state.widgetFont);
  }
  widget.p.qtWidget->setParent(qtContainer);
  widget.setVisible(widget.state.visible);
}

Color pWindow::backgroundColor() {
  if(window.state.backgroundColorOverride) return window.state.backgroundColor;
  QColor color = qtWindow->palette().color(QPalette::ColorRole::Window);
  return { (uint8_t)color.red(), (uint8_t)color.green(), (uint8_t)color.blue(), (uint8_t)color.alpha() };
}

Geometry pWindow::frameMargin() {
  unsigned menuHeight = window.state.menuVisible ? qtMenu->height() : 0;
  unsigned statusHeight = window.state.statusVisible ? qtStatus->height() : 0;
  if(window.state.fullScreen) return { 0, menuHeight, 0, menuHeight + statusHeight };
  return {
    settings.frameGeometryX,
    settings.frameGeometryY + menuHeight,
    settings.frameGeometryWidth,
    settings.frameGeometryHeight + menuHeight + statusHeight
  };
}

bool pWindow::focused() {
  return qtWindow->isActiveWindow() && !qtWindow->isMinimized();
}

Geometry pWindow::geometry() {
  if(window.state.fullScreen) {
    unsigned menuHeight = window.state.menuVisible ? qtMenu->height() : 0;
    unsigned statusHeight = window.state.statusVisible ? qtStatus->height() : 0;
    return { 0, menuHeight, OS::desktopGeometry().width, OS::desktopGeometry().height - menuHeight - statusHeight };
  }
  return window.state.geometry;
}

void pWindow::setBackgroundColor(const Color &color) {
  QPalette palette;
  palette.setColor(QPalette::Window, QColor(color.red, color.green, color.blue));
  qtContainer->setPalette(palette);
  qtContainer->setAutoFillBackground(true);
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
    Geometry geometry = OS::desktopGeometry(), margin = frameMargin();
    qtLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    qtContainer->setFixedSize(geometry.width - margin.width, geometry.height - margin.height);
    qtWindow->showFullScreen();
  }
}

void pWindow::setGeometry(const Geometry &geometry_) {
  locked = true;
  Geometry geometry = geometry_, margin = frameMargin();

  setResizable(window.state.resizable);
  qtWindow->move(geometry.x - margin.x, geometry.y - margin.y);
  qtWindow->adjustSize();

  foreach(layout, window.state.layout) {
    geometry = geometry_;
    geometry.x = geometry.y = 0;
    layout.setGeometry(geometry);
  }
  locked = false;
}

void pWindow::setMenuFont(Font &font) {
  qtMenu->setFont(*font.p.qtFont);
  foreach(item, window.state.menu) item.p.setFont(font);
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

void pWindow::setWidgetFont(Font &font) {
  foreach(item, window.state.widget) {
    if(!item.state.font) item.setFont(font);
  }
}

void pWindow::constructor() {
  qtWindow = new QtWindow(*this);
  qtWindow->setWindowTitle(" ");

  qtLayout = new QVBoxLayout(qtWindow);
  qtLayout->setMargin(0);
  qtLayout->setSpacing(0);
  qtWindow->setLayout(qtLayout);

  qtMenu = new QMenuBar(qtWindow);
  qtMenu->setVisible(false);
  qtLayout->addWidget(qtMenu);

  qtContainer = new QWidget(qtWindow);
  qtContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  qtContainer->setVisible(true);
  qtLayout->addWidget(qtContainer);

  qtStatus = new QStatusBar(qtWindow);
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

  foreach(layout, self.window.state.layout) {
    Geometry geometry = self.geometry();
    geometry.x = geometry.y = 0;
    layout.setGeometry(geometry);
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
