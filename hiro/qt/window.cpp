namespace phoenix {

void pWindow::append(Layout& layout) {
  Geometry geometry = window.state.geometry;
  geometry.x = geometry.y = 0;
  layout.setGeometry(geometry);
}

void pWindow::append(Menu& menu) {
  if(window.state.menuFont != "") menu.p.setFont(window.state.menuFont);
  else menu.p.setFont(Font::sans(8));
  qtMenu->addMenu(menu.p.qtMenu);
}

void pWindow::append(Widget& widget) {
  if(widget.font().empty() && !window.state.widgetFont.empty()) {
    widget.setFont(window.state.widgetFont);
  }
  if(widget.font().empty()) widget.p.setFont(Font::sans(8));
  if(GetParentWidget(&widget)) {
    widget.p.qtWidget->setParent(GetParentWidget(&widget)->p.container(widget));
  } else {
    widget.p.qtWidget->setParent(qtContainer);
  }
  widget.setVisible(widget.visible());
}

Geometry pWindow::frameMargin() {
  unsigned menuHeight = window.state.menuVisible ? settings->geometry.menuHeight : 0;
  unsigned statusHeight = window.state.statusVisible ? settings->geometry.statusHeight : 0;
  if(window.state.fullScreen) return {0, menuHeight, 0, menuHeight + statusHeight};
  return {
    settings->geometry.frameX,
    settings->geometry.frameY + menuHeight,
    settings->geometry.frameWidth,
    settings->geometry.frameHeight + menuHeight + statusHeight
  };
}

bool pWindow::focused() {
  return qtWindow->isActiveWindow() && !qtWindow->isMinimized();
}

void pWindow::remove(Layout& layout) {
}

void pWindow::remove(Menu& menu) {
  //QMenuBar::removeMenu() does not exist
  qtMenu->clear();
  for(auto& menu : window.state.menu) append(menu);
}

void pWindow::remove(Widget& widget) {
  //orphan() destroys and recreates widgets (to disassociate them from their parent);
  //attempting to create widget again after QApplication::quit() crashes libQtGui
  if(qtApplication) widget.p.orphan();
}

void pWindow::setBackgroundColor(Color color) {
  QPalette palette;
  palette.setColor(QPalette::Background, QColor(color.red, color.green, color.blue /*, color.alpha */));
  qtContainer->setPalette(palette);
  qtContainer->setAutoFillBackground(true);
  //translucency results are very unpleasant without a compositor; so disable for now
  //qtWindow->setAttribute(Qt::WA_TranslucentBackground, color.alpha != 255);
}

void pWindow::setDroppable(bool droppable) {
  qtWindow->setAcceptDrops(droppable);
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

void pWindow::setGeometry(Geometry geometry) {
  lock();
  Application::processEvents();
  QApplication::syncX();

  setResizable(window.state.resizable);
  qtWindow->move(geometry.x - frameMargin().x, geometry.y - frameMargin().y);
  //qtWindow->adjustSize() fails if larger than 2/3rds screen size
  qtWindow->resize(qtWindow->sizeHint());
  if(window.state.resizable) {
    //required to allow shrinking window from default size
    qtWindow->setMinimumSize(1, 1);
    qtContainer->setMinimumSize(1, 1);
  }

  for(auto& layout : window.state.layout) {
    geometry.x = geometry.y = 0;
    layout.setGeometry(geometry);
  }
  unlock();
}

void pWindow::setMenuFont(string font) {
  qtMenu->setFont(pFont::create(font));
  for(auto& item : window.state.menu) item.p.setFont(font);
}

void pWindow::setMenuVisible(bool visible) {
  qtMenu->setVisible(visible);
  setGeometry(window.state.geometry);
}

void pWindow::setModal(bool modal) {
  if(modal == true) {
    //windowModality can only be enabled while window is invisible
    setVisible(false);
    qtWindow->setWindowModality(Qt::ApplicationModal);
    setVisible(true);
    while(window.state.modal) {
      Application::processEvents();
      if(Application::main) {
        Application::main();
      } else {
        usleep(20 * 1000);
      }
    }
    qtWindow->setWindowModality(Qt::NonModal);
  }
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

void pWindow::setStatusFont(string font) {
  qtStatus->setFont(pFont::create(font));
}

void pWindow::setStatusText(string text) {
  qtStatus->showMessage(QString::fromUtf8(text), 0);
}

void pWindow::setStatusVisible(bool visible) {
  qtStatus->setVisible(visible);
  setGeometry(window.state.geometry);
}

void pWindow::setTitle(string text) {
  qtWindow->setWindowTitle(QString::fromUtf8(text));
}

void pWindow::setVisible(bool visible) {
  lock();
  qtWindow->setVisible(visible);
  if(visible) {
    updateFrameGeometry();
    setGeometry(window.state.geometry);
  }
  unlock();
}

void pWindow::setWidgetFont(string font) {
}

void pWindow::constructor() {
  qtWindow = new QtWindow(*this);
  qtWindow->setWindowTitle(" ");

  //if program was given a name, try and set the window taskbar icon to a matching pixmap image
  if(applicationState.name.empty() == false) {
    if(file::exists({"/usr/share/pixmaps/", applicationState.name, ".png"})) {
      qtWindow->setWindowIcon(QIcon(string{"/usr/share/pixmaps/", applicationState.name, ".png"}));
    } else if(file::exists({"/usr/local/share/pixmaps/", applicationState.name, ".png"})) {
      qtWindow->setWindowIcon(QIcon(string{"/usr/local/share/pixmaps/", applicationState.name, ".png"}));
    } else if(file::exists({userpath(), ".local/share/icons/", applicationState.name, ".png"})) {
      qtWindow->setWindowIcon(QIcon(string{userpath(), ".local/share/icons/", applicationState.name, ".png"}));
    }
  }

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
  setMenuFont(Font::sans(8));
  setStatusFont(Font::sans(8));

  QColor color = qtWindow->palette().color(QPalette::ColorRole::Window);
  window.state.backgroundColor = Color((uint8_t)color.red(), (uint8_t)color.green(), (uint8_t)color.blue(), (uint8_t)color.alpha());
}

void pWindow::destructor() {
  delete qtStatus;
  delete qtContainer;
  delete qtMenu;
  delete qtLayout;
  delete qtWindow;
}

void pWindow::updateFrameGeometry() {
  pApplication::syncX();
  QRect border = qtWindow->frameGeometry();
  QRect client = qtWindow->geometry();

  settings->geometry.frameX = client.x() - border.x();
  settings->geometry.frameY = client.y() - border.y();
  settings->geometry.frameWidth = border.width() - client.width();
  settings->geometry.frameHeight = border.height() - client.height();

  if(window.state.menuVisible) {
    pApplication::syncX();
    settings->geometry.menuHeight = qtMenu->height();
  }

  if(window.state.statusVisible) {
    pApplication::syncX();
    settings->geometry.statusHeight = qtStatus->height();
  }

  settings->save();
}

void pWindow::QtWindow::closeEvent(QCloseEvent* event) {
  event->ignore();
  if(self.window.onClose) self.window.onClose();
  else self.window.setVisible(false);
  if(self.window.state.modal && !self.window.visible()) self.window.setModal(false);
}

void pWindow::QtWindow::moveEvent(QMoveEvent* event) {
  if(!self.locked() && self.window.state.fullScreen == false && self.qtWindow->isVisible() == true) {
    self.window.state.geometry.x += event->pos().x() - event->oldPos().x();
    self.window.state.geometry.y += event->pos().y() - event->oldPos().y();
  }

  if(!self.locked() && self.window.onMove) self.window.onMove();
}

void pWindow::QtWindow::dragEnterEvent(QDragEnterEvent* event) {
  if(event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

void pWindow::QtWindow::dropEvent(QDropEvent* event) {
  lstring paths = DropPaths(event);
  if(paths.empty()) return;
  if(self.window.onDrop) self.window.onDrop(paths);
}

void pWindow::QtWindow::keyPressEvent(QKeyEvent* event) {
  Keyboard::Keycode sym = Keysym(event->nativeVirtualKey());
  if(sym != Keyboard::Keycode::None && self.window.onKeyPress) self.window.onKeyPress(sym);
}

void pWindow::QtWindow::keyReleaseEvent(QKeyEvent* event) {
  Keyboard::Keycode sym = Keysym(event->nativeVirtualKey());
  if(sym != Keyboard::Keycode::None && self.window.onKeyRelease) self.window.onKeyRelease(sym);
}

void pWindow::QtWindow::resizeEvent(QResizeEvent*) {
  if(!self.locked() && self.window.state.fullScreen == false && self.qtWindow->isVisible() == true) {
    self.window.state.geometry.width = self.qtContainer->geometry().width();
    self.window.state.geometry.height = self.qtContainer->geometry().height();
  }

  for(auto& layout : self.window.state.layout) {
    Geometry geometry = self.window.state.geometry;
    geometry.x = geometry.y = 0;
    layout.setGeometry(geometry);
  }

  if(!self.locked() && self.window.onSize) self.window.onSize();
}

QSize pWindow::QtWindow::sizeHint() const {
  unsigned width = self.window.state.geometry.width;
  unsigned height = self.window.state.geometry.height;
  if(self.window.state.menuVisible) height += settings->geometry.menuHeight;
  if(self.window.state.statusVisible) height += settings->geometry.statusHeight;
  return QSize(width, height);
}

}
