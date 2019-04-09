#if defined(Hiro_Window)

namespace hiro {

auto pWindow::construct() -> void {
  qtWindow = new QtWindow(*this);
  qtWindow->setWindowTitle(" ");

  //if program was given a name, try and set the window taskbar icon to a matching pixmap image
  if(auto name = Application::state().name) {
    if(file::exists({Path::user(), ".local/share/icons/", name, ".png"})) {
      qtWindow->setWindowIcon(QIcon(QString::fromUtf8(string{Path::user(), ".local/share/icons/", name, ".png"})));
    } else if(file::exists({"/usr/local/share/pixmaps/", name, ".png"})) {
      qtWindow->setWindowIcon(QIcon(QString::fromUtf8(string{"/usr/local/share/pixmaps/", name, ".png"})));
    } else if(file::exists({"/usr/share/pixmaps/", name, ".png"})) {
      qtWindow->setWindowIcon(QIcon(QString::fromUtf8(string{"/usr/share/pixmaps/", name, ".png"})));
    }
  }

  qtLayout = new QVBoxLayout(qtWindow);
  qtLayout->setMargin(0);
  qtLayout->setSpacing(0);
  qtWindow->setLayout(qtLayout);

  qtMenuBar = new QMenuBar(qtWindow);
  qtMenuBar->setVisible(false);
  qtLayout->addWidget(qtMenuBar);

  qtContainer = new QWidget(qtWindow);
  qtContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  qtContainer->setVisible(true);
  qtLayout->addWidget(qtContainer);

  qtStatusBar = new QStatusBar(qtWindow);
  qtStatusBar->setSizeGripEnabled(true);
  qtStatusBar->setVisible(false);
  qtLayout->addWidget(qtStatusBar);

  setBackgroundColor(state().backgroundColor);
  setDroppable(state().droppable);
  setGeometry(state().geometry);
  setResizable(state().resizable);
  setTitle(state().title);
}

auto pWindow::destruct() -> void {
if(Application::state().quit) return;  //TODO: hack
  delete qtStatusBar;
  delete qtContainer;
  delete qtMenuBar;
  delete qtLayout;
  delete qtWindow;
}

auto pWindow::append(sMenuBar menuBar) -> void {
}

auto pWindow::append(sSizable sizable) -> void {
}

auto pWindow::append(sStatusBar statusBar) -> void {
}

auto pWindow::focused() const -> bool {
  return qtWindow->isActiveWindow() && !qtWindow->isMinimized();
}

auto pWindow::frameMargin() const -> Geometry {
  if(state().fullScreen) return {
    0, _menuHeight(),
    0, _menuHeight() + _statusHeight()
  };
  return {
    settings.geometry.frameX,
    settings.geometry.frameY + _menuHeight(),
    settings.geometry.frameWidth,
    settings.geometry.frameHeight + _menuHeight() + _statusHeight()
  };
}

auto pWindow::handle() const -> uintptr_t {
  return (uintptr_t)qtWindow->winId();
}

auto pWindow::monitor() const -> uint {
  //TODO
  return 0;
}

auto pWindow::remove(sMenuBar menuBar) -> void {
  //QMenuBar::removeMenu() does not exist
  //qtMenu->clear();
  //for(auto& menu : window.state.menu) append(menu);
}

auto pWindow::remove(sSizable sizable) -> void {
}

auto pWindow::remove(sStatusBar statusBar) -> void {
}

auto pWindow::setBackgroundColor(Color color) -> void {
  static auto defaultColor = qtContainer->palette().color(QPalette::Background);

  auto palette = qtContainer->palette();
  palette.setColor(QPalette::Background, CreateColor(color, defaultColor));
  qtContainer->setPalette(palette);
  qtContainer->setAutoFillBackground((bool)color);
  //translucency results are very unpleasant without a compositor; so disable for now
  //qtWindow->setAttribute(Qt::WA_TranslucentBackground, color && color.alpha() != 255);
}

auto pWindow::setDismissable(bool dismissable) -> void {
}

auto pWindow::setDroppable(bool droppable) -> void {
  qtWindow->setAcceptDrops(droppable);
}

auto pWindow::setEnabled(bool enabled) -> void {
}

auto pWindow::setFocused() -> void {
  qtWindow->raise();
  qtWindow->activateWindow();
}

auto pWindow::setFullScreen(bool fullScreen) -> void {
  lock();
  if(fullScreen) {
    windowedGeometry = state().geometry;
    qtLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    qtContainer->setFixedSize(Desktop::size().width() - frameMargin().width(), Desktop::size().height() - frameMargin().height());
    qtWindow->showFullScreen();
    state().geometry = Monitor::geometry(Monitor::primary());
  } else {
    setResizable(state().resizable);
    qtWindow->showNormal();
    qtWindow->adjustSize();
    self().setGeometry(windowedGeometry);
  }
  unlock();
}

auto pWindow::setGeometry(Geometry geometry) -> void {
  auto lock = acquire();
  Application::processEvents();
  #if HIRO_QT==4
  QApplication::syncX();
  #elif HIRO_QT==5
  QApplication::sync();
  #endif

  setResizable(state().resizable);
  qtWindow->move(geometry.x() - frameMargin().x(), geometry.y() - frameMargin().y());
  //qtWindow->adjustSize() fails if larger than 2/3rds screen size
  qtWindow->resize(qtWindow->sizeHint());
  qtContainer->setMinimumSize(1, 1);
  if(state().resizable) {
    setMaximumSize(state().maximumSize);
    setMinimumSize(state().minimumSize);
  } else {
    setMaximumSize(geometry.size());
    setMinimumSize(geometry.size());
  }
}

auto pWindow::setMaximized(bool maximized) -> void {
  //todo
}

auto pWindow::setMaximumSize(Size size) -> void {
  static auto maximumSize = qtWindow->maximumSize();

  if(!state().resizable) size = state().geometry.size();
  if(size) {
    qtWindow->setMaximumSize(size.width(), size.height() + _menuHeight() + _statusHeight());
  } else {
    qtWindow->setMaximumSize(maximumSize);
  }
}

auto pWindow::setMinimized(bool minimized) -> void {
  //todo
}

auto pWindow::setMinimumSize(Size size) -> void {
  if(!state().resizable) size = state().geometry.size();
  qtWindow->setMinimumSize(size.width(), size.height() + _menuHeight() + _statusHeight());
}

auto pWindow::setModal(bool modal) -> void {
  if(modal) {
    //windowModality can only be enabled while window is invisible
    setVisible(false);
    qtWindow->setWindowModality(Qt::ApplicationModal);
    setVisible(true);
    while(!Application::state().quit && state().modal) {
      if(Application::state().onMain) {
        Application::doMain();
      } else {
        usleep(20 * 1000);
      }
      Application::processEvents();
    }
    qtWindow->setWindowModality(Qt::NonModal);
  }
}

auto pWindow::setResizable(bool resizable) -> void {
  if(resizable) {
    qtLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  //qtContainer->setMinimumSize(state().geometry.width(), state().geometry.height());
  } else {
    qtLayout->setSizeConstraint(QLayout::SetFixedSize);
  //qtContainer->setFixedSize(state().geometry.width(), state().geometry.height());
  }
  qtStatusBar->setSizeGripEnabled(resizable);

  setMaximumSize(state().maximumSize);
  setMinimumSize(state().minimumSize);
}

auto pWindow::setTitle(const string& text) -> void {
  qtWindow->setWindowTitle(text ? QString::fromUtf8(text) : " ");
}

auto pWindow::setVisible(bool visible) -> void {
  auto lock = acquire();
  qtWindow->setVisible(visible);
  if(visible) {
    _updateFrameGeometry();
    setGeometry(state().geometry);
  }
}

auto pWindow::_append(mWidget& widget) -> void {
  if(auto self = widget.self()) {
    self->qtWidget->setParent(qtContainer);
  }
}

auto pWindow::_menuHeight() const -> uint {
  if(auto& menuBar = state().menuBar) {
    if(menuBar->visible()) return settings.geometry.menuHeight + _menuTextHeight();
  }
  return 0;
}

auto pWindow::_menuTextHeight() const -> uint {
  uint height = 0;
  if(auto& menuBar = state().menuBar) {
    for(auto& menu : menuBar->state.menus) {
      height = max(height, menu->font(true).size(menu->text()).height());
    }
  }
  return height;
}

auto pWindow::_statusHeight() const -> uint {
  if(auto& statusBar = state().statusBar) {
    if(statusBar->visible()) return settings.geometry.statusHeight + _statusTextHeight();
  }
  return 0;
}

auto pWindow::_statusTextHeight() const -> uint {
  uint height = 0;
  if(auto& statusBar = state().statusBar) {
    height = statusBar->font(true).size(statusBar->text()).height();
  }
  return height;
}

auto pWindow::_updateFrameGeometry() -> void {
  pApplication::synchronize();
  QRect border = qtWindow->frameGeometry();
  QRect client = qtWindow->geometry();

  settings.geometry.frameX = client.x() - border.x();
  settings.geometry.frameY = client.y() - border.y();
  settings.geometry.frameWidth = border.width() - client.width();
  settings.geometry.frameHeight = border.height() - client.height();

  if(qtMenuBar->isVisible()) {
    pApplication::synchronize();
    settings.geometry.menuHeight = qtMenuBar->height() - _menuTextHeight();
  }

  if(qtStatusBar->isVisible()) {
    pApplication::synchronize();
    settings.geometry.statusHeight = qtStatusBar->height() - _statusTextHeight();
  }
}

auto QtWindow::closeEvent(QCloseEvent* event) -> void {
  event->ignore();
  if(p.state().onClose) p.self().doClose();
  else p.self().setVisible(false);
  if(p.state().modal && !p.self().visible()) p.self().setModal(false);
}

auto QtWindow::moveEvent(QMoveEvent* event) -> void {
  if(!p.locked() && !p.state().fullScreen && p.qtWindow->isVisible()) {
    p.state().geometry.setPosition({
      p.state().geometry.x() + event->pos().x() - event->oldPos().x(),
      p.state().geometry.y() + event->pos().y() - event->oldPos().y()
    });
  }

  if(!p.locked()) {
    p.self().doMove();
  }
}

auto QtWindow::dragEnterEvent(QDragEnterEvent* event) -> void {
  if(event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

auto QtWindow::dropEvent(QDropEvent* event) -> void {
  if(auto paths = DropPaths(event)) p.self().doDrop(paths);
}

auto QtWindow::keyPressEvent(QKeyEvent* event) -> void {
//Keyboard::Keycode sym = Keysym(event->nativeVirtualKey());
//if(sym != Keyboard::Keycode::None && self.window.onKeyPress) self.window.onKeyPress(sym);

  if(p.state().dismissable && event->key() == Qt::Key_Escape) {
    if(p.state().onClose) p.self().doClose();
    else p.self().setVisible(false);
    if(p.state().modal && !p.self().visible()) p.self().setModal(false);
  }
}

auto QtWindow::keyReleaseEvent(QKeyEvent* event) -> void {
//Keyboard::Keycode sym = Keysym(event->nativeVirtualKey());
//if(sym != Keyboard::Keycode::None && self.window.onKeyRelease) self.window.onKeyRelease(sym);
}

auto QtWindow::resizeEvent(QResizeEvent* event) -> void {
  if(!p.locked() && !p.state().fullScreen && p.qtWindow->isVisible()) {
    p.state().geometry.setSize({
      p.qtContainer->geometry().width(),
      p.qtContainer->geometry().height()
    });
  }

  if(auto& sizable = p.state().sizable) {
    sizable->setGeometry(p.self().geometry().setPosition());
  }

  if(!p.locked()) {
    p.self().doSize();
  }
}

auto QtWindow::sizeHint() const -> QSize {
  uint width = p.state().geometry.width();
  uint height = p.state().geometry.height();
  height += p._menuHeight();
  height += p._statusHeight();
  return QSize(width, height);
}

}

#endif
