void pWindow::append(Layout &layout) {
  Geometry geometry = window.state.geometry;
  geometry.x = geometry.y = 0;
  layout.setGeometry(geometry);
}

void pWindow::append(Menu &menu) {
  if(window.state.menuFont != "") menu.p.setFont(window.state.menuFont);
  else menu.p.setFont("Sans, 8");
  qtMenu->addMenu(menu.p.qtMenu);
}

void pWindow::append(Widget &widget) {
  if(widget.state.font == "") {
    if(window.state.widgetFont != "") widget.p.setFont(window.state.widgetFont);
    else widget.p.setFont("Sans, 8");
  }
  widget.p.qtWidget->setParent(qtContainer);
  widget.setVisible(widget.visible());
}

Color pWindow::backgroundColor() {
  if(window.state.backgroundColorOverride) return window.state.backgroundColor;
  QColor color = qtWindow->palette().color(QPalette::ColorRole::Window);
  return { (uint8_t)color.red(), (uint8_t)color.green(), (uint8_t)color.blue(), (uint8_t)color.alpha() };
}

Geometry pWindow::frameMargin() {
  unsigned menuHeight = window.state.menuVisible ? settings->menuGeometryHeight : 0;
  unsigned statusHeight = window.state.statusVisible ? settings->statusGeometryHeight : 0;
  if(window.state.fullScreen) return { 0, menuHeight, 0, menuHeight + statusHeight };
  return {
    settings->frameGeometryX,
    settings->frameGeometryY + menuHeight,
    settings->frameGeometryWidth,
    settings->frameGeometryHeight + menuHeight + statusHeight
  };
}

bool pWindow::focused() {
  return qtWindow->isActiveWindow() && !qtWindow->isMinimized();
}

Geometry pWindow::geometry() {
  if(window.state.fullScreen) {
    unsigned menuHeight = window.state.menuVisible ? qtMenu->height() : 0;
    unsigned statusHeight = window.state.statusVisible ? qtStatus->height() : 0;
    return { 0, menuHeight, Desktop::size().width, Desktop::size().height - menuHeight - statusHeight };
  }
  return window.state.geometry;
}

void pWindow::remove(Layout &layout) {
}

void pWindow::remove(Menu &menu) {
  //QMenuBar::removeMenu() does not exist
  qtMenu->clear();
  for(auto &menu : window.state.menu) append(menu);
}

void pWindow::remove(Widget &widget) {
  //bugfix: orphan() destroys and recreates widgets (to disassociate them from their parent);
  //attempting to create widget again after QApplication::quit() crashes libQtGui
  if(qtApplication) widget.p.orphan();
}

void pWindow::setBackgroundColor(const Color &color) {
  QPalette palette;
  palette.setColor(QPalette::Window, QColor(color.red, color.green, color.blue, color.alpha));
  qtContainer->setPalette(palette);
  qtContainer->setAutoFillBackground(true);
  qtWindow->setAttribute(Qt::WA_TranslucentBackground, color.alpha != 255);
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
    qtContainer->setFixedSize(Desktop::size().width - frameMargin().width, Desktop::size().height - frameMargin().height);
    qtWindow->showFullScreen();
  }
}

void pWindow::setGeometry(const Geometry &geometry_) {
  locked = true;
  OS::processEvents();
  QApplication::syncX();
  Geometry geometry = geometry_, margin = frameMargin();

  setResizable(window.state.resizable);
  qtWindow->move(geometry.x - frameMargin().x, geometry.y - frameMargin().y);
  //qtWindow->adjustSize() fails if larger than 2/3rds screen size
  qtWindow->resize(qtWindow->sizeHint());
  qtWindow->setMinimumSize(1, 1);
  qtContainer->setMinimumSize(1, 1);

  for(auto &layout : window.state.layout) {
    geometry = geometry_;
    geometry.x = geometry.y = 0;
    layout.setGeometry(geometry);
  }
  locked = false;
}

void pWindow::setMenuFont(const string &font) {
  qtMenu->setFont(pFont::create(font));
  for(auto &item : window.state.menu) item.p.setFont(font);
}

void pWindow::setMenuVisible(bool visible) {
  qtMenu->setVisible(visible);
  setGeometry(window.state.geometry);
}

void pWindow::setModal(bool modal) {
  qtWindow->setWindowModality(modal ? Qt::ApplicationModal : Qt::NonModal);
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

void pWindow::setStatusFont(const string &font) {
  qtStatus->setFont(pFont::create(font));
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

void pWindow::setWidgetFont(const string &font) {
  for(auto &item : window.state.widget) {
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
  setMenuFont("Sans, 8");
  setStatusFont("Sans, 8");
}

void pWindow::destructor() {
  delete qtStatus;
  delete qtContainer;
  delete qtMenu;
  delete qtLayout;
  delete qtWindow;
}

void pWindow::updateFrameGeometry() {
  pOS::syncX();
  QRect border = qtWindow->frameGeometry();
  QRect client = qtWindow->geometry();

  settings->frameGeometryX = client.x() - border.x();
  settings->frameGeometryY = client.y() - border.y();
  settings->frameGeometryWidth = border.width() - client.width();
  settings->frameGeometryHeight = border.height() - client.height();

  if(window.state.menuVisible) {
    pOS::syncX();
    settings->menuGeometryHeight = qtMenu->height();
  }

  if(window.state.statusVisible) {
    pOS::syncX();
    settings->statusGeometryHeight = qtStatus->height();
  }

  settings->save();
}

void pWindow::QtWindow::closeEvent(QCloseEvent *event) {
  self.window.state.ignore = false;
  event->ignore();
  if(self.window.onClose) self.window.onClose();
  if(self.window.state.ignore == false) hide();
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

void pWindow::QtWindow::keyPressEvent(QKeyEvent *event) {
  Keyboard::Keycode sym = Keysym(event->nativeVirtualKey());
  if(sym != Keyboard::Keycode::None && self.window.onKeyPress) self.window.onKeyPress(sym);
}

void pWindow::QtWindow::keyReleaseEvent(QKeyEvent *event) {
  Keyboard::Keycode sym = Keysym(event->nativeVirtualKey());
  if(sym != Keyboard::Keycode::None && self.window.onKeyRelease) self.window.onKeyRelease(sym);
}

void pWindow::QtWindow::resizeEvent(QResizeEvent*) {
  if(self.locked == false && self.window.state.fullScreen == false && self.qtWindow->isVisible() == true) {
    self.window.state.geometry.width = self.qtContainer->geometry().width();
    self.window.state.geometry.height = self.qtContainer->geometry().height();
  }

  for(auto &layout : self.window.state.layout) {
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
  if(self.window.state.menuVisible) height += settings->menuGeometryHeight;
  if(self.window.state.statusVisible) height += settings->statusGeometryHeight;
  return QSize(width, height);
}
